#ifndef CHUDNOVSKY_H
#define CHUDNOVSKY_H

#include <stdint.h>
#include <math.h>
#include <gmp.h>

#ifdef _OPENMP
  #include <omp.h>
#endif

#define A 13591409
#define B 545140134
#define C 640320
#define C3 262537412640768000ULL / 24

#define N_GUARD_DIGITS 50

typedef struct {
    mpz_t P;
    mpz_t Q;
    mpz_t T;
} ChudnovskyResult;

static inline void setMpzToU64(mpz_t a, uint64_t v)
{
    mpz_set_ui(a, (unsigned long)v);
} 

static inline void chudnovskyResultAlloc(ChudnovskyResult* r)
{
    mpz_init(r->P);
    mpz_init(r->Q);
    mpz_init(r->T);
}

static inline void chudnovskyResultDealloc(ChudnovskyResult* r)
{
    mpz_clear(r->P);
    mpz_clear(r->Q);
    mpz_clear(r->T);
}


static inline void setChudnovskyIdentity(ChudnovskyResult* out) {
    setMpzToU64(out->P, 1);
    setMpzToU64(out->Q, 1);
    setMpzToU64(out->T, A);
}

static inline void setChudnovsky2Chudnovsky(ChudnovskyResult* c1, ChudnovskyResult* c2)
{
    mpz_set(c1->P, c2->P);
    mpz_set(c1->Q, c2->Q);
    mpz_set(c1->T, c2->T);
}

void chudnovskyResultAlloc(ChudnovskyResult* r);
void chudnovskyResultDealloc(ChudnovskyResult* r);

void chudnovskyBS(uint64_t a, uint64_t b, ChudnovskyResult* out);
void chudnovskyMerge(const ChudnovskyResult* L, const ChudnovskyResult* R, ChudnovskyResult* out);

void chudnovskyOpenMPInit(void);
void chudnovskyMP(uint64_t a, uint64_t b, ChudnovskyResult* out);

void chudnovskyCalculatePi(uint64_t n_digits);
#endif