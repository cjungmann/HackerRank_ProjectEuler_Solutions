// -*- compile-command: "g++ -ggdb -std=c++11 -o triangular triangular.cpp -lm" -*-

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>   // for memset
#include <assert.h>
using namespace std;

struct Prime {unsigned long val; Prime* next; };

typedef void(*void_ulong)(unsigned long);
typedef bool(*bool_ulong)(unsigned long);
typedef void(*void_Prime)(Prime*);

/**
 * Use GetPrimes::by_value() or GetPrimes::by_count() to create a collection of prime numbers.
 *
 * This class is only used for its static data and function members.  The reason for
 * this is that the function that build the list is recursive, and I wanted the function
 * to take only a single argument, the next data node of the linked list of prime numbers.
 *
 * Since the recursion could be many thousands of levels deep, there is a benefit to saving
 * stack memory by limiting the parameter list.
 *
 * The progress of the collection is tracked by static class members, providing all
 * instances of the recursive function access to those data.
 *
 * Use GetPrimes::by_value() to get primes up to and including the number passed,
 * use GetPrimes::by_count() to get as many primes as the number passed.
 */
class GetPrimes
{
public:
   static void_ulong s_found;
   static bool_ulong s_continue;
   static void_Prime s_call_when_done;

private:
   static Prime s_podd;
   static Prime s_base;

   static unsigned long s_primes_found;

   static bool is_prime(unsigned long val)
   {
      Prime *p = &s_base;
      unsigned long limit = floor(sqrt(val))+1;
      while (p && p->val <= limit)
      {
         if (val % p->val == 0)
            return false;
         p = p->next;
      }
      return true;
   }

   static void primes_build_list(Prime* last = &s_podd)
   {
      assert(last!=&s_base);

      unsigned long start = last->val+1;
      if ((*s_continue)(start))
      {
         for (unsigned long i=last->val+2; (*s_continue)(i); ++i)
         {
            if (is_prime(i))
            {
               ++s_primes_found;
               Prime newprime = {i, NULL};
               last->next = &newprime;
               (*s_found)(i);
               primes_build_list(&newprime);

               // Job done when a prime is found
               break;
            }
         }
      }
      else
         s_call_when_done(&s_base);


   }

public:
   static unsigned long s_primes_needed;
private:
   static void until_count_found_prime(unsigned long prime)   { ; }
   static bool until_count_continue(int unsigned long number) { return s_primes_found<s_primes_needed; }

public:
   static unsigned long s_prime_cur;
   static unsigned long s_prime_min;
private:
   static void until_value_found_prime(unsigned long prime)   { s_prime_cur=prime; }
   static bool until_value_continue(int unsigned long number) { return s_prime_cur<s_prime_min; }

public:
   /**
    * Get @p count primes numbers, call @p callback when the collection is complete.
    */
   static void by_count(unsigned long count, void_Prime callback)
   {
      s_primes_needed = count;
      s_primes_found = 2;

      s_found = until_count_found_prime;
      s_continue = until_count_continue;
      s_call_when_done = callback;

      primes_build_list();
   }

   /**
    * Create a collection of @p count primes numbers, call @p callback when the collection is complete.
    */
   static void by_value(unsigned long value, void_Prime callback)
   {
      s_prime_min = value;
      s_prime_cur = 3;

      s_found = until_value_found_prime;
      s_continue = until_value_continue;
      s_call_when_done = callback;

      primes_build_list();
   }

   static unsigned long primes_found(void) { return s_primes_found; }
   static Prime* const get_base(void)      { return &s_base; }

   
};

void_ulong GetPrimes::s_found = nullptr;
bool_ulong GetPrimes::s_continue = nullptr;
void_Prime GetPrimes::s_call_when_done = nullptr;

unsigned long GetPrimes::s_primes_found;

unsigned long GetPrimes::s_primes_needed;
unsigned long GetPrimes::s_prime_cur;
unsigned long GetPrimes::s_prime_min;

Prime GetPrimes::s_podd = { 3, nullptr };
Prime GetPrimes::s_base = { 2, &GetPrimes::s_podd };

/** End of Primes Code **/

int g_count_cases = 0;
int *g_cases = nullptr;
unsigned long *g_answers = nullptr;
unsigned long g_max_found_factors = 0;
int g_max_required_divisors = 0;

typedef void(*Print_Row)(int rows, unsigned long numbers, unsigned long divisors);
void imp_print_row(int rows,
                   unsigned long numbers,
                   unsigned long divisors)      { printf("%5d %6lu %3lu\n", rows, numbers, divisors); }
void imp_print_row_skip(int rows,
                        unsigned long numbers,
                        unsigned long divisors) { ; }
Print_Row print_row = imp_print_row;



unsigned long get_divisors_count(int value)
{
   // One is a special case, with only one factor.
   if (value==1)
      return 1;

   // Everything else has at least two
   unsigned long rval = 1;
   int limit = value;

   Prime *p = GetPrimes::get_base();
   while (p && p->val <= limit)
   {
      int prime = p->val;
      int power = 0;
      while (value >= prime && value % prime == 0)
      {
         ++power;
         value /= p->val;
      }

      rval *= power+1;
      
      p = p->next;
   }
   return rval;
}

void print_primes(Prime *base)
{
   Prime *p = base;
   int count = 0;
   while (p)
   {
      cout << ++count << p->val << endl;
      p = p->next;
   }
}

void print_answers(void)
{
   for (int i=0; i<g_count_cases; ++i)
      cout << g_answers[i] << endl;
}

void update_answers(unsigned long numbers, unsigned long divisors)
{
   for (int i=0; i<g_count_cases; ++i)
      if (g_answers[i]==0 && divisors > g_cases[i])
         g_answers[i] = numbers;
}

void scan(void)
{
   int rows = 1;
   unsigned long numbers = 1;
   unsigned long divisors = 1;

   while (divisors <= g_max_required_divisors)
   {
      divisors = get_divisors_count(numbers);

      (*print_row)(rows, numbers, divisors);

      if (divisors > g_max_found_factors)
      {
         g_max_found_factors = divisors;
         update_answers(numbers, divisors);
      }
   
      numbers += ++rows;
   }
}


void use_primes(Prime *base)
{
   scan();
}


int main()
{
   cin>>g_count_cases;

   if (g_count_cases)
   {
      g_cases = new int[g_count_cases];
      if (g_cases)
      {
         for (int i=0; i<g_count_cases; ++i)
         {
            cin>>g_cases[i];
            if (g_cases[i] > g_max_required_divisors)
               g_max_required_divisors = g_cases[i];
         }

         g_answers = new unsigned long[g_count_cases];
         if (g_answers)
         {
            std::memset(g_answers, 0, sizeof(g_answers));

            GetPrimes::by_value(10000, use_primes);

            print_answers();
            
            delete [] g_answers;
         }
         delete [] g_cases;
      }
   }

   return 0;
}
