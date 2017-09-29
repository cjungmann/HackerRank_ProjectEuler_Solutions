# HackerRank Project Euler #11 Largest Product in Grid

This project was much easier than the last one, with no real tricky programming required.

My solution is set up a bit differently from the others in that the same test can be run
in two different ways, with the file name as an argument or redirected into stdin.  The
file `testgrid.txt` is a copy of the test grid on [HackerRank](https://www.hackerrank.com/contests/projecteuler/challenges/euler011)
and it was used to test the code before uploading.


## Test Run with STDIN

This is how HackerRank runs the program, so it has only official output:

~~~sh
$ gridproduct testgrid.txt
~~~

## Test Run with Filename Argument

The program detects this method and prints additional information as an aid for debugging.
I used the method to confirm that I was performing the expected number of product calculations,
and was able to fix quickly fix failed result by fixing conditions that determined which cells
 were being considered.

~~~sh
$ gridproduct testgrid.txt
~~~
