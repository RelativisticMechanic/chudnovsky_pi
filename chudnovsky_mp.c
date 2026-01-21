#include <stdio.h>
#include <stdlib.h>

#include "chudnovsky.h"

uint64_t N_chudnovsky_threads = 1;

void chudnovskyOpenMPInit(void)
{
#ifdef _OPENMP
    int procs = omp_get_num_procs();
    int max_threads = omp_get_max_threads();

    if(procs * 2 < max_threads)
    {
        N_chudnovsky_threads = procs * 2;
    }
    else
    {
        N_chudnovsky_threads = max_threads;
    }

    omp_set_dynamic(0);  // disable dynamic teams
    omp_set_num_threads(N_chudnovsky_threads);

    printf("[OpenMP Init] OpenMP enabled\n"
            "  omp_get_num_procs()   = %d\n"
            "  omp_get_max_threads() = %d\n"
            "  using threads         = %ld\n",
            procs, max_threads, N_chudnovsky_threads);
#else
    printf("[OpenMP Init] Not compiled with OpenMP. Single-threaded run.\n");
    N_chudnovsky_threads = 1;
#endif
}

void chudnovskyMP(uint64_t a, uint64_t b, ChudnovskyResult* out)
{
    uint64_t n;
    int T;

    if(b <= a)
    {
        setChudnovskyIdentity(out);
        return;
    }

    n = b - a;
    T = N_chudnovsky_threads;

    if(T == 1 || n < 100)
    {
        chudnovskyBS(a, b, out);
    }

    if((uint64_t)T > n) T = (int)n;
    if(T < 1) T = 1;

    ChudnovskyResult* partials = (ChudnovskyResult*)calloc((size_t)T, sizeof(ChudnovskyResult));
    uint64_t* as = (uint64_t*)calloc((size_t)T, sizeof(uint64_t));
    uint64_t* bs = (uint64_t*)calloc((size_t)T, sizeof(uint64_t));

    if(!partials || !as || !bs)
    {
        printf("[ERROR] Failed to allocate memory for thread data.");
        return;
    }

    for(int i = 0; i < T; i++)
    {
        chudnovskyResultAlloc(&partials[i]);
        setChudnovskyIdentity(&partials[i]);
    }

    uint64_t base, rem;
    uint64_t cur = a;

    base = n / (uint64_t)T;
    rem = n % (uint64_t)T;
    
    for(int i = 0; i < T; i++)
    {
        uint64_t len = base + ((uint64_t)i > rem ? 1 : 0);
        as[i] = cur;
        bs[i] = cur + len;
        cur += len;
    }
#ifdef _OPENMP
    #pragma omp parallel for schedule(static)
#endif
    for(int i = 0; i < T; i++)
    {
        uint64_t a_i = as[i];
        uint64_t b_i = bs[i];

        if(a_i < b_i)
        {
            chudnovskyBS(a_i, b_i, &partials[i]);
        }
    }

    /* Merge, again using a binary partitioning scheme */
    int skip = 1;

    while (skip < T) {
        // Merge blocks that are skip apart: i and i+skip
        // Step by 2*skip so we merge disjoint pairs
        for (int i = 0; i + skip < T; i += 2 * skip) {

            ChudnovskyResult tmp;
            chudnovskyResultAlloc(&tmp);
            setChudnovskyIdentity(&tmp);

            // tmp = merge(partials[i], partials[i+skip])
            chudnovskyMerge(&partials[i], &partials[i + skip], &tmp);

            // partials[i] = tmp
            setChudnovsky2Chudnovsky(&partials[i], &tmp);

            // Free tmp
            chudnovskyResultDealloc(&tmp);

            // Free the right operand because it is now absorbed
            chudnovskyResultDealloc(&partials[i + skip]);

            // Optional safety: re-init the cleared slot to identity,
            // so accidental reuse won’t crash.
            // chudnovskyResultAlloc(&partials[i + skip]);
            // setChudnovskyIdentity(&partials[i + skip]);
        }

        skip *= 2;
    }


    setChudnovsky2Chudnovsky(out, &partials[0]);
}