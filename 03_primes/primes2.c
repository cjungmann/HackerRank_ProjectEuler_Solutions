// -*- compile-command: "gcc -ggdb -o primes2 primes2.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>


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


int main(int argc, char** argv)
{
   int count_vals;
   scanf("%d", &count_vals);

   long int* list_vals = (long int*)malloc(count_vals*sizeof(long int));

   for (int i=0; i<count_vals; ++i)
      scanf("%ld", &list_vals[i]);

   for (int i=0; i<count_vals; ++i)
      printf("%lld\n", factor(list_vals[i]));

   free(list_vals);

   return 0;
}
