#include <stdio.h>

#include "pi.h"

void chudnovskyCalculatePi(uint64_t n_digits) {
    chudnovskyOpenMPInit();
    
    uint64_t terms = (uint64_t)(ceil((double)n_digits / 14.181647462) + 1);
    uint64_t p = (n_digits + N_GUARD_DIGITS);

    mpz_t scale, N, sqrt_scaled, K, pi, numerator;
    mpz_inits(scale, N, sqrt_scaled, K, pi, numerator, NULL);

    // Scale = 10^p
    mpz_ui_pow_ui(scale, 10, p);

    // 10^2p
    mpz_mul(N, scale, scale);

    // N = 10005 * 10^2p
    mpz_mul_ui(N, N, 10005);

    // Sqrt(N)
    mpz_sqrt(sqrt_scaled, N);

    // K = 426880 * sqrt(N)
    mpz_mul_ui(K, sqrt_scaled, 426880);

    mpz_clears(scale, N, sqrt_scaled, NULL);
    
    ChudnovskyResult result;
    chudnovskyResultAlloc(&result);

    chudnovskyMP(0, terms, &result);

    mpz_mul(numerator, K, result.Q);
    mpz_tdiv_q(pi, numerator, result.T);

    mpz_t ten_guard;
    mpz_init(ten_guard);

    // ten_guard = 10^N_GUARD_DIGITS
    mpz_ui_pow_ui(ten_guard, 10, N_GUARD_DIGITS);

    // pi = pi / 10^N_GUARD_DIGITS  (drop guard digits)
    mpz_tdiv_q(pi, pi, ten_guard);

    mpz_clear(ten_guard);

    printf("\n");

    printf("PI DIGITS START\n");
    gmp_printf("%Zd\n", pi);
    printf("PI DIGITS END\n");

    mpz_clears(numerator, K, NULL);
    chudnovskyResultDealloc(&result);
}