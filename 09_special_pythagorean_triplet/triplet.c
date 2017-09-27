// -*- compile-command: "gcc -ggdb -o triplet triplet.c -lm" -*-

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

int psqr(int *p) { return *p * *p; }
int vsqr(int p) { return p*p; }


/**
 * Make an equation of b in terms of a (with constant value N):
 *
 * Make alias of C in terms of the other values:
 * N = a + b + c
 * c = N-a-b
 *
 * Replace c in the pythagorean equation:
 * a*a + b*b = (N-a-b)(N-a-b)
 *
 * Multipy right sidefactors 
 * a*a + b*b = N*N - Na - Nb - Na + a*a + ab - Nb + ab + b*b
 * Combine right side values
 * a*a + b*b = N*N - 2Na - 2Nb + a*a + 2ab + b*b
 * Subtract a*a and b*b from both sides
 * 0 = N*N - 2Na - 2Nb + 2ab
 * Move b-factored values to left side
 * 2Nb - 2ab = N*N - 2Na
 * Divide both sides by b, now there's only one b value in equation
 * 2N - 2a = (N*N - 2Na)/b
 * Divide both sides by (N*N - 2Na) to isolate b value on right side
 * (2N - 2a)/(N*N - 2Na) = 1/b
 * Reciprocal both sides to simplify b on right
 * (N*N - 2Na)/(2N - 2a) = b
 *
 * Use the final equation to calculate the b value.
 * Calculate c from c = N-a-b
 *
 * Return the b and c values to the calling function in the
 * int* parameters.
 */
void derive_b_c_from_a(int N, int a, int* b, int* c)
{
   *b = (N*N - 2*N*a) / ( 2*N - 2*a);
   *c = N-a-*b;
}

bool is_pythagorean_triplet(int a, int b, int c)
{
   return a*a + b*b == c*c;
}

int solve_case(int N)
{
   int a, b, c;
   int limit = N/3;
   int max = -1;
   for (int a=3; a<limit; ++a)
   {
      derive_b_c_from_a(N,a,&b,&c);
      if (is_pythagorean_triplet(a,b,c))
      {
         int product = a * b * c;
         if (product > max)
            max = product;

         /* printf("%5d * %5d * %5d = %6d\n",a,b,c,product); */
      }
   }
   return max;
}

int main(void)
{
   int count;
   scanf("%u", &count);

   if (count)
   {
      int *cases = (int*)malloc(count*sizeof(int));
      for (int i=0; i<count; ++i)
         scanf("%d", &cases[i]);

      for (int i=0; i<count; ++i)
         printf("%d\n", solve_case(cases[i]));
   }
   return 0;
}
