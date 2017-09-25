# HackerRank Project Euler #8 Largest Product in a Series

My first attempt passed the simple test case, but failed every other case.  There were two
factors that caused the failure, one obvious that I shouldn't have missed, and one more subtle
that took some more time to figure out.

## Problem 1: Prepare to Accept Up To 1000 Digits

I started by using a `long long unsigned` to hold the number value.  That is clearly a mistake
when the specs say that the length value could be up to 1000, meaning that the number would also
have 1000 digits.  I should have been reading into a string buffer right away, as even the longest
unsigned integer value (18446744073709551615) is only 20 digits long (base 10).

## Problem 2: `scanf` Reading Enter key

Even after I changed to scan the characters of the number, I still had bad results.  In fact,
the simple tests failed as well.  After trying to understand why the zero-th character was
nonsense, I googled the `scanf` function and found a warning that it reads any character, even
the whitespaces that other `scanf` formats skip.  I changed my loop to ignore non-digit characters
when scanning the number, and then everything worked.
