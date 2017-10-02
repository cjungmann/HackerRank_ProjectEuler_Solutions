# HackerRank Project Euler #12 Highly Divisible Triangular Number

I got a solution to
[this problem](https://www.hackerrank.com/contests/projecteuler/challenges/euler012/problem)
almost immediately, writing an entire program before compiling to a single, easily-fixed
error.  I fixed the error, and my program worked on all cases but #6 and #7, which failed
only because of the time limit.  This program, for the most part, remains as `triangular.c`.

## Timeout Problems

To improve efficiency, I tried the optimizations that worked in previous projects, mainly
ideas to avoid scanning a linked list for answers.  This area of the program runs in
`walk_trivisors()`.

After these attempts proved unsuccessful, I used my forced-seg-fault technique to collect
some hints.  I learned that no matter how efficient everything else was, it was the number
of time `count_divisors()` was called that determined whether or not the program finished
in time.  In fact, even on my local computer, it took several seconds to find the triangular
number for over 1000 divisors (the limit set by the project).

## Reluctant Resort to Discussion

I had no more ideas, so I looked at the Discussions and found the solution for finding the
number of divisors was to apply the prime number factors to a formula as described in this
[discussion thread](https://www.hackerrank.com/contests/projecteuler/challenges/euler012/forum/comments/191509).

The beginning of the solution was to use a list of prime numbers to factor the number.

## A Sinkhold of Distraction

While I could have copied a prime numbers list, I thought it would be more elegant to generate
it on the fly.  Having several times previously needed a prime numbers list, I decided it was
time to package the primes list generator.  Since efficiency required that the recursive
function reference several values outside the function block scope for stack memory conservation,
I decided it was time to jump to C++, where I could use a class scope of static variables with a
static member functions.  This way, the function could run with no local variable except the
single parameter holding a pointer to the tail-end of a linked-list of prime numbers.

Well, this took longer than I would have liked because of language challenges that arose from
the fact the the class definition and the static data members had to be declared in the same
source file.  Sadly, I wasn't able to complete encapsulate the class.

## Debugging Errors in C++ Version: Switchable Output

I got it to work after a while, but the new version was failing test cases #1, #2, #4, and #5.
I struggled with this for a while, but eventually decided that I should compare the results
of the `triangular.c` solution that worked except for time out, with the results of the new
`triangular.cpp` that was fast, but occasionally incorrect.

I decided to make typedef a function pointer to a printing function.  Each program could
then set define two functions that matched the typedef, one that printed, and the other that
does nothing.  Although I didn't do it for this project, I certainly will in the future,
control which of the printing functions is used by a command line argument.  No argument
uses the non-printing print function for HackerRank to run.

Using this method, I had both programs run the same stdin values (1 100) to print the output.
I diffed the files to see that I had several incorrect divisor calculations in the c++ version.
I studied the formula more carefully and corrected my incorrect assumptions.  Now the solution
worked!
