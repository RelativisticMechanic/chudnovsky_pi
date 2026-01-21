#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "chudnovsky.h"


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: Use Chudnovsky to determine n digits of pi\ncnov <digits to compute>\ne.g:\ncnov 10000 [gives 10,000 digits of pi]");
        return -1;
    }

    uint64_t n = atoi(argv[1]);

    if(n < 15)
    {
        printf("Minimum digits required 15!");
        return -1;
    }

    chudnovskyCalculatePi(n);
    return 0;
}