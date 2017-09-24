// -*- compile-command: "gcc -ggdb -o primes primes.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define PTYPE long long unsigned
#define PMAX ULLONG_MAX
#define PTOK "%llu"


// Prototype structure to allow for linked-list of prime values.
typedef struct Prime Prime;

// For testing and demonstration before and after submitting to HackerRank:
int g_verbose = 0;

// Global variables to hold submitted values for which to calculate max prime factors
// so a callback function can recursively retrieve and process the values before the
// the stack-based prime number list goes out of scope.
PTYPE *g_list_vals = NULL;
int g_count_vals = 0;
int g_index_vals = 0;
int g_count_primes = 1;
int g_primes_required = 0;

// Linked-list structure
struct Prime
{
   PTYPE val;
   Prime *next;
};

// Root prime-number list in global space:
Prime pbase = {2,NULL};

PTYPE psqrt(PTYPE v) { return floor(sqrt(v))+1; }

/** For verbose setting to show list of calculated prime numbers. */
void print_epilogue(void)
{
   if (g_verbose==1)
   {
      printf("\nNumber of primes found: %d\n", g_count_primes);
   }
   else if (g_verbose==2)
   {
      int count = 0;
      Prime *p = &pbase;
      while (p)
      {
         printf("%3d: "PTOK"\n", ++count, p->val);
         p = p->next;
      }
   }
}

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
void show_primes(PTYPE *arr)
{
   for (PTYPE i=0; i<g_count_vals; ++i)
      printf(PTOK"\n", arr[i]);
}

/**
 * Make an array of primes values, with the requested primes at the indexes of
 * prime number indexes.
 */
void index_prime_values(void)
{
   PTYPE *arr = (PTYPE*)malloc(g_count_vals*sizeof(PTYPE));
   Prime *p = &pbase;
   for (PTYPE pos=1; pos<=g_primes_required; ++pos, p=p->next)
   {
      for (PTYPE ndx=0; ndx<g_count_vals; ++ndx)
      {
         if (g_list_vals[ndx]==pos)
            arr[ndx] = p->val;
      }
   }
   show_primes(arr);

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
   if (g_count_primes==g_primes_required)
      index_prime_values();
   if (g_count_primes < g_primes_required)
   {
      for (; i<PMAX; i+=2)
      {
         if (!has_any_current_factor(i))
         {
            ++g_count_primes;

            Prime newprime = {i,NULL};
            last->next = &newprime;
            build_primes_list(&newprime);

            break;
         }
      }
      if (g_verbose)
         printf("Oops: exceeded type limit before finding all the primes.\n");
   }
}

void print_help(void)
{
   printf("Enter a list of numbers, one per line.\n\n"
          "The first number says how many numbers follow.\n\n"
          "Then the following numbers are unsigned integers\n"
          "which are the requested positions in the list of\n"
          "prime numbers.\n\n"
          "The program will begin processing when the last\n"
          "number has been entered.\n\n"
          "The v or V argument will produce a verbose output, with\n"
          "extra information at completion of the program.\n"
          "-v shows the count of prime numbers found when done.\n"
          "-V lists enumerated prime numbers when done.\n"
          "Both -v and -V will use sentences to display the prime numbers.\n"
      );
}

int main(int argc, char** argv)
{
   if (argc>1 && argv[1][0]=='-')
   {
      switch(argv[1][1])
      {
         case 'h':
            print_help();
            return 0;
         case 'v':
            g_verbose = 1;
            break;
         case 'V':
            g_verbose = 2;
            break;
         default:
            break;
      }
   }

   scanf("%d", &g_count_vals);

   g_list_vals = (PTYPE*)malloc(g_count_vals*sizeof(PTYPE));
   for (int i=0; i<g_count_vals; ++i)
   {
      scanf(PTOK, &g_list_vals[i]);
      if (g_list_vals[i]>g_primes_required)
         g_primes_required = g_list_vals[i];
   }

   build_primes_list(&pbase);

   if (g_verbose)
      print_epilogue();

   free(g_list_vals);

   return 0;
}
