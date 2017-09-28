// -*- compile-command: "gcc -ggdb -o smallest_multiple smallest_multiple.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

#define RTYPE unsigned int
#define RMAX UINT_MAX
#define RTOK "%u"

RTYPE process_test(RTYPE top_factor)
{
   // Handle unique test for 1, from which nothing can be subtracted.
   if (top_factor==1)
      return 1;
   else
   {
      RTYPE penultimate_factor = top_factor-1;
      RTYPE result = top_factor * penultimate_factor;

      for (; result<RMAX; result+=top_factor)
      {
         bool failed = false;
         for (RTYPE i=penultimate_factor; i>1; --i)
         {
            if (result % i)
            {
               failed = true;
               break;
            }
         }

         if (!failed)
            return result;
      }
   }

   return 0;
}

int main(void)
{
   int count;
   scanf("%d",&count);

   int *tests = (int*)malloc(count*sizeof(int));
   for (int i=0; i<count; ++i)
      scanf("%d", &tests[i]);

   for (int i=0; i<count; ++i)
      printf(RTOK"\n", process_test(tests[i]));

   free(tests);


   return 0;
}
