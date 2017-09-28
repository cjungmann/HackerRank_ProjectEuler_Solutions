# HackerRank Project Euler #9 Special Pythagoran Triplet

This [HackerRank Challenge](https://www.hackerrank.com/contests/projecteuler/challenges/euler009)
should be pretty easy, but the problem is that a brute strength approach takes
too long to solve case #5.  At the suggestion of in the discussion page, I combined
what we know about the numbers to calculate one leg's value, given another leg value.

The source code includes a function called `derive_b_c_from_a()` that explains
the steps for turning the two equations into a single equation getting leg b from
leg a.  The values of a and b are used to calculate hypotenuse c from the sum equation.

With a, b, and c values that satisfy the sum equation, the program checks if a, b, and c
are a pythagorean triplet and proceeds with the easier part of the challenge.