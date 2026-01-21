export CC=gcc
export CFLAGS="-g -Wall -O2 -c -fopenmp"
export LIBS="-lgmp -fopenmp"

$CC $CFLAGS chudnovsky.c -o chudnovsky.o 
$CC $CFLAGS main.c -o main.o
$CC $CFLAGS pi.c -o pi.o
$CC $CFLAGS chudnovsky_mp.c -o chudnovsky_mp.o

$CC main.o chudnovsky.o chudnovsky_mp.o pi.o -o cnov $LIBS

rm *.o