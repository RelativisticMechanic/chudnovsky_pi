# Chudnovsky's $\pi$ Calculation - An Overview

You may have heard about teams breaking records for calculating the trillionth digit of $\pi$. In this write up, we're going to talk about the Chudnovsky's Formula, it has been used consistently to break records for finding the digits of $\pi$, with the current one (as of 2026) standing at 202 trillion digits.

Based on Ramanujan's expansion for $\pi$, following is Chudnovsky's formula for calculating $\pi$:

$$ \frac{1}{\pi} = 12 \sum_{k=0}^{\infty}{\frac{(-1)^k (6k)! (545140134k + 13591409)}{(3k)!(k!)^3(640320)^{3k+3/2}}} $$

Those are some massive numbers. Oh, and would you look at those factorials!

In most programming languages the regular data types are limited to a CPU-defined size (usually 64-bits on modern systems), so these large calculations have to be done using a "big integer library". We represent large numbers as structures in memory broken into smaller numbers. There's a lot of math involved in multiplication of big numbers (even fast Fourier transforms get involved, don't ask how!), so we'll leave that to the experts, i.e., we'll use GNU's libGMP (GNU Multi-Precision).

The problem with this formula at first glance is that we have to compute factorials for every term. In addition, there is a big integer division for every term (big integer divisions are intensive to do).

## Simplification #1: Defining Constants

Define: 

$$ A = 13591409 $$
$$ B = 545140134 $$
$$ C = 640320 $$

$$ \frac{1}{\pi} = 12 \sum_{k=0}^{\infty}{\frac{(-1)^k (A + Bk)}{(C)^{3k+3/2}}\frac{(6k)!}{(3k)!(k!)^3}} $$

Right away we can take $C^3$ in and factor $\sqrt{C}^3$ out.

$$ \frac{1}{\pi} = \frac{12}{\sqrt{C^3}} \sum_{k=0}^{\infty}{\frac{(-1)^k (A + Bk)}{(C^3)^{k}}\frac{(6k)!}{(3k)!(k!)^3}} $$

## Simplification #2: The Constant Term

We note:

$$ (640320)^3 = 12^2 \times (426880)^2 \times 10005 $$

This turns the equation into:

$$ \frac{426880\sqrt{10005}}{\pi} = \sum_{k=0}^{\infty}{\frac{(-1)^k (A + Bk)}{(C^3)^{k}}\frac{(6k)!}{(3k)!(k!)^3}} $$

## Simpliciation #3: The Factorial Term

$$ \frac{426880\sqrt{10005}}{\pi} = \sum_{k=0}^{\infty}{\frac{(-1)^k (A + Bk)}{(C^3)^{k}}\frac{(6k)!}{(3k)!(k!)^3}} $$

Define:

$$ R_{k} = \frac{(6k)!}{(3k)!(k!)^3}$$

We note that:

$$ \frac{R_{k}}{R_{k-1}} = \frac{(6k-1)(2k -1)(6k-5)}{k^3} $$

This is amazing. It allows us to calculate the factorial part as procedural multiplication.

Thus, we define:

$$ P_{k} = \frac{(6k-1)(2k -1)(6k-5)}{C^3 k^3} $$

With 

$$ P_{0} = 1 $$

But how do we actually use this? 

## Simplication #4: Binary Partitioning

The sum can be written in this notation:

$$ \frac{426880\sqrt{10005}}{\pi} = \sum_{k=0}^{\infty}{{(A + Bk)}\prod_{i=0}^{k}{(-1)^k P_{i}}} $$