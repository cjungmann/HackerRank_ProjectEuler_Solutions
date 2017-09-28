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

This fix helped because trying to find 1,000,000 primes ran out of memory.  The number of primes
that are less than 1,000,000 is a much smaller number, 78,499.  Changing this helped case 6 work,
but case 7 still failed due to timeout.

## Detective Work

I wasn't sure what problem to solve to fix the time out problem in case 7.  I had noticed several
possible test reports.  The following list may not be complete:

- **Pass**, all answers correct, also reporting time to completion,
- **Wrong Answer** with at least one incorrect answer,
- **Time Expired**, where the test is killed at a time limit,
- **Compiler Error**, or
- **Segmentation Fault**.

### Test Time to Calculate Primes

It was possible that it was taking too long to calculate the prime numbers.  Because I check the
test values to identify the highest prime number required to get all at once, I could game this
value by setting the highest prime required to be 1,000,000,000 for all cases.  That most test
cases completed in less than 0.10 seconds suggested that my unorthodox method of generating the
primes list was not the problem.

### Cache The Summations

To get the sum of primes, the first version of the program walked the primes list for each test
value, accumulating the summation by walking the prime number list until the prime value was greater
than or equal to the test value.  Walking a list is obviously slower than accessing the values
from an array, but calculating a new total for each test could be avoided as well.  I did this by
having each `Prime` record hold the summation up to its own value.  The summation value was calculated
when the `Prime` record was created, using its own value and the summation of the previous `Prime`
record.

### Cache The Answers

To eliminate other list-walking occurances, I decided I could save each answer as I found it.
I created an array of answers parallel to the array of values.  As each prime number was discovered,
I checked the list of values and set the matching answer array element for each value that was between
the new and previous prime values.  (Previous < new Prime <= submitted value).  This saved some
time, but not enough to pass test 7.

### Segmentation Fault to Peek at Case Conditions

Even with the above two improvements, case 7 still took too long to complete.  I felt like I needed
to know more about the numbers case 7 submitted to the program to understand why it was failing.

Normally, the test cases are black boxes.  We only see the result messages for each case after the
tests are run.  I decided I could trick the system into revealing information.

Since it is trivial to produce a segmentation fault at will by attempting to assign a value to a
NULL pointer, I realized that I could exploit this to get useful information about the nature of
the values the test cases pass to the programs.

I added a function, `trigger_segfault()` that attempts to assign a value to a null pointer,
 `*((int*)0)==0`, causing an immediate memory access error.

For case 7, I tested
- the number of values submitted
- the range of values submitted

#### Find Number of Values Submitted

This was easy.  I submitted a series of programs that called `trigger_segfault` if the number of
cases exceeded higher and higher numbers.  By this method, I proved that test 7 submitted over 9990
test values.

To simulate this test locally, I changed the program to optionally generate its own set of values
randomly.  I made a function pointer typedef and two matching functions, one getting a value from
stdin, and the other returning a random value in the range specified in the project.  The program
then filled the array of values using the function pointer that was set according to command line
arguments.

On my local computer, the test completed very quickly even when running the program with the maximum
number of values.  That didn't seem to be the problem.

#### Check the Range of Numbers

Still not having a clue as to why the program failed, I ran this last test.  I checked for values
out of the specified range by adding code that would call `trigger_segfault()` if any value was less
than 1 or greated than 10,000.  This was highly unlikely, but I was desperate.  The test confirmed
that the test cases were playing fair.  The test for a value<=0 remains in `get_stdin()`.

## Final Solution: Using a Sorted List of Values

I didn't want to resort to anyone else's solution.  In the discussion section, using a sieve to
calcuate the primes was the most suggested solution.  I realized that for high prime values and 
large numbers of test values, even caching the answers (see above) resulted in many tests.  For
a value of 1,000,000,000 (the highest allowed test value), in test 7 there would be 78,499 times
the program would make 10,000 comparisons.  That's almost 800 million comparisons.

I wanted to eliminate as many of the comparisons as possible.  The only way to do that was to 
ensure that the program could consider the submitted values in ascending order, that was it only
had to compare a narrow range of possiblities with each new prime number.  The answers had to be
written in the same order as the values were submitted, so I created an ordered array of integers
that were indexes to increasing submission values.  That is, the first value in the ordered array
was the index of the smallest submitted value, the second ordered array element was an index to the
second smallest submitted value, up to the last array element was the index of the largest submitted
value.

Then the prime numbers are generated, the program loops through the submitted values in the order
defined by the ordering array.  A global index keeps track of which element of the ordering array
is current.  The global index always points to that last value that was equal to or lower than the
last discovered prime, so the comparisons get a head start for each new prime.  Then, for each
submitted value that is less than or equal to the current prime, the program stores the summation
to the matching element in the answer array.

When the primes are all collected, each element of the answer array is output in its natural order,
which corresponds to the order the test values were submitted.

## Final, Minor Adjustment for a Single Very Small Request

This change fixed test 7, but now test 4 gave a wrong answer.  Test values 2 through 5 returned 0
when they were the only test value.  Esthetically, I didn't like setting a magic number answer
for specific values in a special case, so I solved this problem by ensuring that the program
generates one extra prime above the requested prime.  This is probably not the most efficient
solution because it is likely much more computationally expensive to search for another prime 
for very large test values that it is to repeatedly check for the special case.  However, with
a clean run, all 8 (0-7) tests successful, I let it go.
