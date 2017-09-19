# HackerRank Project Euler #3: Largest Prime Factor

This project contains two source files, both solving the same
problem at [Project Euler #3: Largest Prime Factor](https://www.hackerrank.com/contests/projecteuler/challenges/euler003).

There are two solutions in this project.  I think the first project, despite failing
the test, is more interesting because it is an example of building a stack-based
linked list using recursion and callback functions.

- **primes.c**, which failed the test, illustrates how to build a linked list
  of prime numbers on the stack with recursion, callback functions, and function
  pointers.  My thought was to build the list once (adding to it when necessary),
  and use it to solve several prime factor problems.

  When searching for large numbers first, this version shows the benefit of
  preserving the lists for follow-on searches.  However, the test failed because
  it took too long for some of the test cases.   Though I don't know how to confirm
  this (because I don't know how to view the test cases), I think the failures came
  from very large numbers.

- **primes2.c** uses a suggested prime factor function that is relatively easy
  to understand.  This source file was passed the HackerRank test.

In both files, the "make" instructions are in a EMACS compiler variable at the
top of the source file.  It can be copied to the command line to build the file.
The make line includes an important setting, the `-lm` linker command to link
in the math library for the `sqrt` function.