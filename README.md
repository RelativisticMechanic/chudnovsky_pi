# Chudnovsky's Calculation for Digits of Pi

This is a program in C that implements Chudnovsky's algorithm for calculation of digits of pi.

The program implements the binary splitting algorithm which makes it much faster.

It also uses OpenMP to paralleize chunks, and merges them (again via a pair-wise merge to speed things up).

Dependencies:

- GNU Multi-Precision Library (libgmp)
- OpenMP

Simply build with

```bash
./build.sh
```

Run with

```
./cnov [number of digits to compute, must be greater than 15]
```
