# HackerRank Project Euler #10 Summation of Primes

This [HackerRank Challenge](https://www.hackerrank.com/contests/projecteuler/challenges/euler010)
seems similar to previous prime number manipulations.  I started by copying the source code from 
[Project #7](../007_10001st_prime), a solution that determines the highest prime position required
from the list of tests and then returns to use the collection to satisfy the request list.

I will save the initial modification, but while it works for 100,000, (10^5), it fails for the
1,000,000 (10^6) limit in the problem description. By fails, I mean even on my computer that
supposedly has fewer restrictions than the contest environment.  Also note that the first
solution works for the online test cases 0-5, failing for test cases 6 and 7.

## First Modification: Collect Primes To Upper Limit

As I am writing the README, I realize one problem is that Project 7 collected a set number of
prime numbers, while this project uses prime values up to and including the request number.
My first attempt to fix the failure will be to change how the `build_primes_list()` terminates,
so it checks the value of the latest prime number rather than the number of primes numbers
collected.

