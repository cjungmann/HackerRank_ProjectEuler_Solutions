// -*- compile-command: "gcc -ggdb -o triangular_c triangular.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define PTYPE long
#define PTOK "%ld"

typedef struct TriVisor TriVisor;

struct TriVisor
{
   PTYPE rows;
   PTYPE numbers;
   PTYPE divisors;
   TriVisor *next;
};

typedef void(*Print_Row)(PTYPE rows, PTYPE numbers, PTYPE divisors);

void imp_print_row(PTYPE rows, PTYPE numbers, PTYPE divisors)
{
   printf("%5ld %6ld %3ld\n", rows, numbers, divisors);
}

void imp_print_row_skip(int rows, unsigned long numbers, unsigned long divisors) 
{
   ;
}

Print_Row print_row = imp_print_row_skip;



int* g_tcases = NULL;
PTYPE* g_tanswers = NULL;
int  g_tcount = 0;

PTYPE g_line = 0;
PTYPE g_triid = 0;

PTYPE g_min_divisors;

TriVisor g_root = {1,1,1,NULL};

void crash(void) { *((int*)0) = 0; }

PTYPE get_first_greater_than(int divisors)
{
   TriVisor *tv = &g_root;
   while (tv)
   {
      if (tv->divisors > divisors)
         return tv->numbers;
      else
         tv = tv->next;
   }
   return 0;
}


void show_tris_table(void)
{
   TriVisor *t = &g_root;
   printf(" rows  numbers divisors\n");
   while(t)
   {
      printf(" %4ld   %6ld   %ld\n", t->rows, t->numbers, t->divisors);
      t = t->next;
   }
}


void report_cases(PTYPE *arr, int count)
{
   for (int i=0; i<count; ++i)
      printf(PTOK"\n", arr[i]);
}

PTYPE count_divisors(PTYPE numbers)
{
   PTYPE divisors = 0;
   for(PTYPE factor=1, limit=numbers; factor<limit; ++factor)
   {
      if (numbers % factor==0)
      {
         limit = numbers / factor;
         if (limit > factor)
            divisors += 2;
         else if (limit==factor)
            ++divisors;
      }
   }
   return divisors;
}

void walk_trivisors(PTYPE rows, PTYPE numbers)
{
   PTYPE divisors;
   do
   {
      divisors = count_divisors(numbers);

      // Change whether this prints or not by changing which function print_row points to:
      // print_row = imp_print_row to print, print_row = imp_print_row_skip for no printing.
      (*print_row)(rows, numbers, divisors);

      for (int i=0; i<g_tcount; ++i)
         if (g_tcases[i]<divisors && g_tanswers[i]==0)
            g_tanswers[i] = numbers;
      
      numbers += ++rows;
   }
   while (divisors < g_min_divisors);
}

void read_cases(int* arr, int count)
{
   g_min_divisors = 0;
   for (int i=0; i<count; ++i)
   {
      scanf("%d", &arr[i]);
      if (arr[i] > g_min_divisors)
         g_min_divisors = arr[i];
   }

   // we need at least one more than the highest number requested
   ++g_min_divisors;

   walk_trivisors(1,1);
}

int main(int argc, char** argv)
{
   scanf("%d", &g_tcount);
   if (g_tcount)
   {
      g_tcases = (int*)malloc(g_tcount*sizeof(int));
      if (g_tcases)
      {
         g_tanswers = (PTYPE*)malloc(g_tcount*sizeof(PTYPE));
         if (g_tanswers)
         {
            memset(g_tanswers, 0, g_tcount*sizeof(PTYPE));

            read_cases(g_tcases, g_tcount);
            report_cases(g_tanswers, g_tcount);
            
            free(g_tanswers);
         }
         free(g_tcases);
      }
   }
   
   return 0;
}
