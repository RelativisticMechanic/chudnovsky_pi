#include "chudnovsky.h"

void chudnovskyBS(uint64_t a, uint64_t b, ChudnovskyResult* out) {

    if (b - a == 1) {
        uint64_t k = a;
        if(k == 0)
        {
            setChudnovskyIdentity(out);
            return;
        }
        else {
            // P = (6k-5)(2k-1)(6k-1)
            mpz_t t1, t2;
            mpz_inits(t1, t2, NULL);

            setMpzToU64(out->P, (6*k - 5));
            mpz_mul_ui(out->P, out->P, (unsigned long)(2*k - 1));
            mpz_mul_ui(out->P, out->P, (unsigned long)(6*k - 1));

            // Q = k^3 * C3
            // out->Q = k*k*k*C3
            setMpzToU64(out->Q, k);
            mpz_mul_ui(out->Q, out->Q, (unsigned long)k);
            mpz_mul_ui(out->Q, out->Q, (unsigned long)k);
            mpz_mul_ui(out->Q, out->Q, (unsigned long)C3);

            // T = P * (A + B*k)
            // t1 = B*k
            setMpzToU64(t1, B);
            mpz_mul_ui(t1, t1, (unsigned long)k);

            // t2 = A + B*k
            setMpzToU64(t2, A);
            mpz_add(t2, t2, t1);

            // T = P * t2
            mpz_mul(out->T, out->P, t2);

            // If k odd, T = -T
            if (k & 1ULL)
                mpz_neg(out->T, out->T);

            mpz_clears(t1, t2, NULL);
            return;
        }
    }
    else {
        uint64_t m = (a + b) / 2;

        ChudnovskyResult left_r, right_r;
        chudnovskyResultAlloc(&left_r);
        chudnovskyResultAlloc(&right_r);

        chudnovskyBS(a, m, &left_r);
        chudnovskyBS(m, b, &right_r);

        chudnovskyMerge(&left_r, &right_r, out);

        chudnovskyResultDealloc(&left_r);
        chudnovskyResultDealloc(&right_r);
    }
}

/*
    Merge two binary partitions.
*/
void chudnovskyMerge(const ChudnovskyResult* L, const ChudnovskyResult* R, ChudnovskyResult* out)
{
    // out.P = L.P * R.P
    mpz_mul(out->P, L->P, R->P);

    // out.Q = L.Q * R.Q
    mpz_mul(out->Q, L->Q, R->Q);

    // out.T = L.T * R.Q + L.P * R.T
    mpz_t tmp1, tmp2;
    mpz_init(tmp1);
    mpz_init(tmp2);

    mpz_mul(tmp1, L->T, R->Q);   // tmp1 = L.T * R.Q
    mpz_mul(tmp2, L->P, R->T);   // tmp2 = L.P * R.T
    mpz_add(out->T, tmp1, tmp2); // out.T = tmp1 + tmp2

    mpz_clear(tmp1);
    mpz_clear(tmp2);
}