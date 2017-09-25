// -*- compile-command: "gcc -ggdb -o pickproduct pickproduct.c -lm" -*-

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

typedef struct TCase TCase;

struct TCase {
   char* string;
   unsigned int length;
   unsigned int series;
};

PTYPE get_series_val(char *buff, unsigned len)
{
   PTYPE rval = buff[0];
   for (unsigned i=1; i<len; ++i)
      rval *= (PTYPE)buff[i];
   return rval;
}

void print_factors(char *buff, unsigned len)
{
   PTYPE total = (PTYPE)buff[0];
   unsigned v = (unsigned)buff[0];
   printf("%u", v);
   for (unsigned i=1; i<len; ++i)
   {
      unsigned v = (unsigned)buff[i];
      printf(" x %u", v);
      total *= (PTYPE)buff[i];
   }

   printf(" = "PTOK"\n", total);
}

PTYPE get_largest_product(char *buff, unsigned length, unsigned series)
{
   PTYPE maxproduct = 0;
   unsigned limit = length - series + 1;
   for (unsigned i=0; i<limit; ++i)
   {
      PTYPE val = get_series_val(&buff[i], series);
      if (val > maxproduct)
      {
         /* print_factors(&buff[i], series); */
         maxproduct = val;
      }
   }

   return maxproduct;
}

PTYPE process_test_case(TCase *tcase)
{
   PTYPE rval = 0;
   if (tcase->length)
      rval = get_largest_product(tcase->string, tcase->length, tcase->series);
   return rval;
}

void print_tcase(TCase *tcase)
{
   printf("Printing tcase.\n");
   for (unsigned i=0; i<tcase->length; ++i)
      printf("%c", tcase->string[i]+'0');
   printf("\n");
}

int main(void)
{
   int cases;
   scanf("%d",&cases);
   if (cases)
   {
      size_t len = cases * sizeof(TCase);
      TCase *tcases = (TCase*)malloc(len);
      memset((void*)tcases, 0, len);


      for (int i=0; i<cases; ++i)
      {
         TCase *tcase = &tcases[i];

         scanf("%u %u", &tcase->length, &tcase->series);

         if (tcase->length)
         {
            size_t slen = tcase->length;
            char *buff = tcase->string = (char*)malloc(slen);

            for (int i=0; i<slen; ++i)
            {
               char c;
               scanf("%c", &c);

               // Skip non-numeric characters:
               if (c<'0' || c>'9')
               {
                  --i;
                  continue;
               }

               buff[i] = c - '0';
            }
         }
      }

      for (int i=0; i<cases; ++i)
      {
         TCase* tcase = &tcases[i];
         if (tcase->length)
            printf(PTOK"\n", process_test_case(tcase));
         else
            printf("0\n");
      }

      // Free malloced memory:
      for (int i=0; i<cases; ++i)
         if (tcases[i].string)
            free(tcases[i].string);

      free(tcases);
   }
}

