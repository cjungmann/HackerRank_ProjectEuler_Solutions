// -*- compile-command: "gcc -ggdb -o primes primes.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define PTYPE long long unsigned
#define PTOK "%llu"


long long int factor(long long int temp)
{
   while(temp%2==0)
      temp/=2;

   if(temp==1)
      return 2;

   long long int i;
   for(i=3;i<=sqrt(temp);i+=2)
   {
      if(temp%i==0)
      {
         temp/=i;

         // This, after i+=2, starts the loop again at 3:
         i=1;
      }
   }
   
   if(temp>2)
      return temp;

   else return i-2;
}


// Prototype structure to allow for linked-list of prime values.
typedef struct Prime Prime;

// Callback functions to be called recursively to maintain the stack-based list
// of calculated prime numbers until the program terminates.
typedef void (*max_prime_user)(PTYPE maxprime);
typedef void(*Continue)(void);

// For testing and demonstration before and after submitting to HackerRank:
int g_verbose = 0;

// Global variables to hold submitted values for which to calculate max prime factors
// so a callback function can recursively retrieve and process the values before the
// the stack-based prime number list goes out of scope.
PTYPE *g_list_vals = NULL;
int g_count_vals = 0;
int g_index_vals = 0;
int g_count_primes = 1;

// Linked-list structure
struct Prime
{
   PTYPE val;
   Prime *next;
};

// Root prime-number list in global space:
Prime pbase = {2,NULL};

// Global callback function null until the function is defined:
Continue g_continue = NULL;

// Prototype required because extend_primes_list() is called by get_max_prime()
// and also calls get_max_prime() to keep stack-based prime list intact.
void get_max_prime(PTYPE val, Prime *start, max_prime_user f);

PTYPE psqrt(PTYPE v)
{
   return floor(sqrt(v))+1;
}

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
      printf("\nPrinting list of primes:\n");
      while (p)
      {
         printf("%4d: "PTOK"\n", ++count, p->val);
         p = p->next;
      }
   }
}

/** Tests if any prime value is a factor of val. */
bool has_any_current_factor(PTYPE val)
{
   Prime *p = &pbase;
   while (p)
   {
      if (val % p->val == 0)
         return true;
      p = p->next;
   }
   
   return false;
}

/**
 * Finds the next prime number and adds it to the linked-list.  Continues recursion.
 *
 * Call this function if the current list of prime numbers insufficient to test
 * the current value.  It calculates the next prime number, adding it to the list,
 * then calls the max_prime_user function to see if the new number is sufficient
 * to return a max prime factor.
 */
void extend_primes_list(PTYPE val, Prime *last, max_prime_user f)
{
   // Only consider odd numbers, cut comparisons in half:
   PTYPE i = last->val+1;
   if (i%2==0)
      ++i;

   PTYPE limit = psqrt(val*2);

   for (; i<=limit; i+=2)
   {
      if (!has_any_current_factor(i))
      {
         ++g_count_primes;
         Prime newprime = {i, NULL};
         last->next = &newprime;
         get_max_prime(val, &pbase, f);
         break;
      }
   }
}

/**
 * Entry-point of the primary task of the program.
 *
 * The function main() will have prepared the global variables needed to
 * run this function.
 */
void get_max_prime(PTYPE val, Prime *start, max_prime_user f)
{
   PTYPE maxprime = 0;
   PTYPE limit = psqrt(val);

   assert(start);

   Prime *p = start;

   // Scan to last in prime pointer list
   while (p->next && p->val<=limit)
   {
      if (val % p->val == 0)
         maxprime = p->val;

      p = p->next;
   }

   if (p->val < limit)
      extend_primes_list(val, p, f);
   else if (val % p->val == 0)
      (*f)(p->val);
   else
      (*f)(maxprime?maxprime:val);
}


// Callback for found max prime factor
void useprime(PTYPE maxfactor)
{
   if (g_verbose)
      printf("The max prime factor for "PTOK" is ", g_list_vals[g_index_vals-1]);

   printf(PTOK"\n", maxfactor);
   (*g_continue)();
}

// Callback to continue with next value in list of ints to process
void run_func(void)
{
   if (g_index_vals < g_count_vals)
      get_max_prime(g_list_vals[g_index_vals++], &pbase, useprime);
   // only print, if requested, after all values have been processed
   else if (g_verbose)
      print_epilogue();
}

void print_help(void)
{
   printf("Enter a list of numbers, one per line.\n\n"
          "The first number says how many numbers follow.\n\n"
          "Then the following numbers are unsigned integers\n"
          "for which the maximum prime factor should be\n"
          "calculated.\n\n"
          "The program will begin processing when the last\n"
          "number has been entered.\n\n"
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
      printf("%lld\n", factor(g_list_vals[i]));
   }

   /* g_continue = run_func; */
   /* (*g_continue)(); */

   free(g_list_vals);

   return 0;
}
