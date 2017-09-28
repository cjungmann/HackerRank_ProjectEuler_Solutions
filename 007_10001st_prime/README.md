# HackerRank Project Euler #7: 10001st Prime

I started with the code I produced for [#3 Largest Prime Factor](../03_primes/README.md),
making modifications to solve wrong answers as I thought of them.

## First Improvement, Conserve Stack Space

My first efforts went to solve a possible problem with stack overflow.  Since I'm using
the recursion/stack-based linked list, I thought the wrong answer may have been a stack
overflow message.

I made several changes:

- Calculated all necessary primes at once by tracking the largest request input
  while scanning the inputs, then passing the maximum value to `build_primes_list()`.
- `build_primes_list()` was modified from `extend_primes_list()` from the old project,
  but I moved two of the parameters to global scope to keep those values off of the
  stack.  Thus, `build_primes_list()` has a single argument, the end of the linked
  list.

## Second Improvement: Increase Efficiency for Speed

I created an array of integers parallel to the input requests.  I added function
`index_prime_values()` to create this parallel index and populate it with the
prime values at the requested positions.  I scanned the linked list once, and for
each position, I checked the list of requests for request at the current position.
This avoided repeated walks through the potentially many linked prime values.

These changes solved one of the test case wrong answers, but I still had two or
three wrong answers.

## Third Solution, Analyse Available Information

Why was this last?  When I looked at the completion times for the several successful
tests, I noticed that all finished within .01 seconds, even the wrong answers.  The
problem was not speed or stack overflow (at least not any longer).  I realized that
the problem was unexpected inputs.

I was making two errors:
- There was no response for a single request.
- My program returned 0 for duplicate requests after the first request.  That is,
  if the inputs requested three times for position 10, the first would be correct,
  but the second and third requests for position 10 would return 0.

## With the above fixed, I passed the test.  Yay!


