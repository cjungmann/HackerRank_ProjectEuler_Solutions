// -*- compile-command: "gcc -ggdb -o palin palin.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

int max_factor = 0;

/**
 * Return the integer value of `len` characters of `test`.
 *
 * Using `len` instead of 0 for two reasons:
 * - The test cases are not zero-terminated
 * - Sometimes we need to get the integer value of the left-most three characters.
 */
int toint(char *test, int len)
{
   int val = 0;
   for (int i=0; i<len; ++i)
   {
      val *= 10;
      val += test[i]-'0';
   }

   return val;
}

/**
 * Unsafely converts an int to a non-zero-terminated string.
 *
 * It is assumed that the code that calls this function understands the importance
 * of protecting the string buffer from overruns.   We can't test because the strings
 * are not zero-terminated, so we can't test the length, and val might be <1,000 
 * or <1,000,000 at different times, so we can't test against an integer limit.
 *
 * Just be careful.
 */
void tostr(char *target, int val)
{
   int pos = floor(log10(val));
   char *p = &target[pos];
   while (p >= target)
   {
      *p = (char)(val % 10) + '0';
      val /= 10;
      --p;
   }
}

/**
 * Make numeric palindrome with the current string, returning new palindrome integer value.
 *
 * Reverse-copies first three to the last three positions of the string.
 */
int mirror_left_to_right(char *cand)
{
   cand[5] = cand[0];
   cand[4] = cand[1];
   cand[3] = cand[2];
   return toint(cand,6);
}

/**
 * Modifies string argument appropriately and returns the integer value it represents.
 *
 * Finds a palindrome value that is the maximum value that is also less than the original value.
 */
int convert_to_max_palindrome(char *test)
{
   char tcopy[6];
   memcpy(tcopy, test, 6);

   int start = toint(test,6);
   int trial = mirror_left_to_right(tcopy);

   if (trial >= start)
   {
      tostr(tcopy, toint(tcopy,3)-1);
      trial = mirror_left_to_right(tcopy);
   }

   assert(trial < start);

   memcpy(test, tcopy, 6);
   return trial;
}

int get_factor_up(int reference, int start)
{
   for (int i=start; i<1000; ++i)
      if (reference % i ==0)
         return i;
   return 0;
}

int get_factor_down(int reference, int start)
{
   for (int i=start; i>99; --i)
      if (reference % i ==0)
         return i;
   return 0;
}


int get_max_two_factor_palindrome(char *test)
{
   // Get starting value before modification
   int startval = toint(test,6);

   int factor = 0;
   int curpal = convert_to_max_palindrome(test);
   int left = toint(test,3);

   while (left>99)
   {
      // Square root as first candidate:
      int trial = floor(sqrt(curpal));

      // Step through candidates in most appropriate direction:
      if (trial < max_factor)
         factor = get_factor_up(curpal, trial);
      else
         factor = get_factor_down(curpal, trial);

      if (factor)
         return curpal;
      else
      {
         // Make next smallest palindrome and run trial again:
         tostr(test, --left);
         curpal = mirror_left_to_right(test);
      }
   }

   return 0;
}

int main(void)
{
   // Calculate limiting factor once for all at beginning:
   max_factor = floor(sqrt(999999));

   int count;
   scanf("%d", &count);

   char buff[7];
   char *tests = (char*)malloc(count * 6 * sizeof(char));

   // collect tests
   for (int i=0; i<count; ++i)
   {
      scanf("%6s", buff);
      memcpy(&tests[i*6], buff, 6);
   }

   // run tests
   for (int i=0; i<count; ++i)
      printf("%d\n", get_max_two_factor_palindrome(&tests[i*6]));

   free(tests);

   return 0;
}
        
