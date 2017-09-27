// -*- compile-command: "gcc -ggdb -o sumprimes sumprimes.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define PTYPE long unsigned
#define PMAX ULONG_MAX
#define PTOK "%lu"


// Prototype structure to allow for linked-list of prime values.
typedef struct Prime Prime;

// Global variables to hold submitted values for which to calculate max prime factors
// so a callback function can recursively retrieve and process the values before the
// the stack-based prime number list goes out of scope.
PTYPE *g_list_vals = NULL;
long int g_count_vals = 0;
long int g_index_vals = 0;
long int g_count_primes = 1;
long int g_highest_required = 0;

// Linked-list structure
struct Prime
{
   PTYPE val;
   Prime *next;
};

// Root prime-number list in global space:
Prime pbase = {2,NULL};

PTYPE psqrt(PTYPE v) { return floorl(sqrtl(v))+1; }

/** Tests if any prime value is a factor of val. */
bool has_any_current_factor(PTYPE val)
{
   PTYPE limit = psqrt(val);
   Prime *p = &pbase;
   while (p && p->val<limit)
   {
      if (val % p->val == 0)
         return true;
      p = p->next;
   }
   
   return false;
}

/** Print out the final results. */
void show_results(PTYPE *arr)
{
   for (PTYPE i=0; i<g_count_vals; ++i)
      printf(PTOK"\n", arr[i]);
}

PTYPE get_sum_of_primes(long int limit)
{
   PTYPE sum = 0;

   Prime *p = &pbase;
   for (PTYPE pos=1; p->val<=limit; ++pos, p=p->next)
      sum += p->val;

   return sum;
}

/**
 * Make an array of primes values, with the requested primes at the indexes of
 * prime number indexes.
 */
void process_tests(void)
{
   /* printf("Collected "PTOK" prime numbers.\n", g_count_primes); */

   PTYPE *arr = (PTYPE*)malloc(g_count_vals*sizeof(PTYPE));
   for (PTYPE i=0; i<g_count_vals; ++i)
      arr[i] = get_sum_of_primes(g_list_vals[i]);

   show_results(arr);

   free(arr);
}

/**
 * Recursion to build a primes list.  Refers to global variables to minimize stack usage;
 */
void build_primes_list(Prime *last)
{
   // Only consider odd numbers, cut comparisons in half:
   PTYPE i = last->val+2;
   if (i%2==0)
      --i;

   // Don't loop, recursion will process incremented n values
   if (last->val >= g_highest_required)
      process_tests();
   else
   {
      for (; i<PMAX; i+=2)
      {
         if (!has_any_current_factor(i))
         {
            // Just for yucks, see how many we have:
            ++g_count_primes;

            Prime newprime = {i,NULL};
            last->next = &newprime;
            build_primes_list(&newprime);

            break;
         }
      }
   }
}

int main(void)
{
   scanf("%ld", &g_count_vals);

   g_list_vals = (PTYPE*)malloc(g_count_vals*sizeof(PTYPE));
   for (long int i=0; i<g_count_vals; ++i)
   {
      scanf(PTOK, &g_list_vals[i]);
      if (g_list_vals[i]>g_highest_required)
         g_highest_required = g_list_vals[i];
   }

   build_primes_list(&pbase);

   free(g_list_vals);

   return 0;
}
