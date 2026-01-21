export CC=gcc
export CFLAGS="-g -Wall -O2 -c -fopenmp"
export LIBS="-lgmp -fopenmp"

$CC $CFLAGS main.c -o main.o
$CC $CFLAGS chudnovsky.c -o chudnovsky.o 
$CC $CFLAGS chudnovsky_pi.c -o chudnovsky_pi.o
$CC $CFLAGS chudnovsky_mp.c -o chudnovsky_mp.o

$CC main.o chudnovsky.o chudnovsky_mp.o chudnovsky_pi.o -o cnov $LIBS

rm *.o