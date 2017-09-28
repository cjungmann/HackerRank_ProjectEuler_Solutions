// -*- compile-command: "gcc -ggdb -o sumprimes sumprimes.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#include <time.h>  // for random number seed

#define PTYPE long long int
#define PMAX LLONG_MAX
#define PTOK "%lld"


// Prototype structure to allow for linked-list of prime values.
typedef struct Prime Prime;

// Global variables to hold submitted values for which to calculate max prime factors
// so a callback function can recursively retrieve and process the values before the
// the stack-based prime number list goes out of scope.
PTYPE *g_list_vals = NULL;
PTYPE *g_list_answers = NULL;
int *g_list_indexes = NULL;
int g_index_index = 0;
long int g_count_vals = 0;
long int g_index_vals = 0;
long int g_count_primes = 1;
long int g_highest_required = 0;

// Linked-list structure
struct Prime
{
   PTYPE val;
   PTYPE total;
   Prime *next;
};

// Root prime-number list in global space:
Prime pbase = {2,2,NULL};

/** For detective work on obscured test cases: conditionally force SEGFAULT to reveal details. */
void trigger_segfault(void) { *((int*)0)=0; }

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
void show_results(void)
{
   /* printf("Collected "PTOK" prime numbers for "PTOK" bytes\n", */
   /*        g_count_primes, */
   /*        g_count_primes*sizeof(Prime)); */

   for (PTYPE i=0; i<g_count_vals; ++i)
      printf(PTOK"\n", g_list_answers[i]);
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
 * Recursion to build a primes list.  Refers to global variables to minimize stack usage;
 */
void build_primes_list(Prime *last)
{
   // Only consider odd numbers, cut comparisons in half:
   PTYPE i = last->val+2;
   if (i%2==0)
      --i;

   PTYPE pcur;

   // Don't loop, recursion will process incremented n values
   if (last->val >= g_highest_required)
      show_results();
   else
   {
      for (; i<PMAX; i+=2)
      {
         if (!has_any_current_factor(i))
         {
            Prime newprime = {i,last->total+i,NULL};
            last->next = &newprime;

            // Just for yucks, see how many we have:
            ++g_count_primes;

            for (;
                 g_index_index<g_count_vals && (pcur=g_list_vals[g_list_indexes[g_index_index]]) <= i;
                 ++g_index_index)
            {
               g_list_answers[g_list_indexes[g_index_index]] = pcur==i?newprime.total:last->total;
            }

            build_primes_list(&newprime);

            break;
         }
      }
   }
}

typedef PTYPE (*PGetter)(void);

PTYPE get_stdin(void)
{
   PTYPE val;
   scanf(PTOK, &val);
   if (val<=0)
      trigger_segfault();
   
   return val;
}



PTYPE get_rando(void)
{
   static unsigned int seed = 0;
   if (!seed)
   {
      seed = time(NULL);
      srand(seed);
   }

   PTYPE rval = rand() % 100000 + 1;
   printf(PTOK"\n", rval);

   return rval;
}

int index_sorter(const void* left, const void* right)
{
   return g_list_vals[*((int*)left)] - g_list_vals[*((int*)right)];
}


/**
 * To run the program against an arbitrary long list of values, call the
 * program with an integer parameter.  The program will generate list of 
 * suitable random numbers to run the test.  It will output the values
 * as well as the answers, so if the output is directed to a file, the
 * file contents can be truncated to leave a list of input values that can
 * be submitted as a file to repeat the test.
 *
 * For example, to run a test with 10,000 values,
 *
 * ~~~sh
 * $ ./sumprimes 10000
 * ~~~
 *
 * To save the generated numbers,
 *
 * ~~~sh
 * $ ./sumprimes 10000 > output.txt
 * ~~~
 *
 * To repeat the test, open *output.txt* with a text editor, go to line
 * 10,000, and erase to the end of the file from the blank line after the
 *  list of numbers.  Then add `10000` on the first line of the file and save.
 *
 * Run the repeat test with,
 * ~~~sh
 * $ ./sumprimes < output.txt
 * ~~~
 */
int main(int argc, char** argv)
{
   PGetter pgetter = get_stdin;
   bool cmdline = false;

   if (argc>1)
   {
      g_count_vals = atoi(argv[1]);
      cmdline = true;
   }

   if (cmdline)
      pgetter = get_rando;
   else
      scanf("%ld", &g_count_vals);

   g_list_vals = (PTYPE*)malloc(g_count_vals*sizeof(PTYPE));
   g_list_answers = (PTYPE*)malloc(g_count_vals*sizeof(PTYPE));
   g_list_indexes = (int*)malloc(g_count_vals*sizeof(int));
   memset((void*)g_list_answers, 0, g_count_vals*sizeof(PTYPE));
   for (long int i=0; i<g_count_vals; ++i)
   {
      g_list_indexes[i] = i;

      g_list_vals[i] = (*pgetter)();
      if (g_list_vals[i]>g_highest_required)
         g_highest_required = g_list_vals[i];
   }

   // Get the first prime higher than the highest requested prime
   ++g_highest_required;

   qsort(g_list_indexes, g_count_vals, sizeof(int), index_sorter);

   if (cmdline)
   {
      printf("\n%ld random numbers done, sorted list to optimize:\n\n", g_count_vals);

      for (int i=0; i<g_count_vals; ++i)
         printf(PTOK"\n", g_list_vals[g_list_indexes[i]]);

      printf("\nList generated and sorted, results calculated along with primes:\n\n");
   }

   /* g_highest_required = 1000000; */

   build_primes_list(&pbase);

   if (cmdline)
      printf("The number of primes <= %ld is %ld\n", g_highest_required-1, g_count_primes);

   free(g_list_indexes);
   free(g_list_answers);
   free(g_list_vals);

   return 0;
}
