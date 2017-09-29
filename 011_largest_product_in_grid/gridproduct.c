// -*- compile-command: "gcc -ggdb -o gridproduct gridproduct.c -lm" -*-

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>

int cells[20][20];

#define PTYPE long int
#define PTOK "%ld"

int products = 0;

int count_right = 0;
int count_down = 0;
int count_down_left = 0;
int count_down_right = 0;

/**
 * The number of products expected should be:
 * products               right: 20 rows * 17 cells = 340
 * products                down: 17 rows * 20 cells = 340
 * products  diagonal down-left: 17 rows * 17 cells = 289
 * products diagonal down-right: 17 rows * 17 cells = 289
 *                        total products calculated: 1258
 */

void read_cells(FILE *str)
{
   for (PTYPE r=0; r<20; ++r)
      for (PTYPE c=0; c<20; ++c)
         fscanf(str, "%d", &cells[r][c]);
}

void record_products(int inc_down, int inc_right)
{
   ++products;
   if (inc_down)
   {
      if (inc_right < 0)
         ++count_down_left;
      else if (inc_right > 0)
         ++count_down_right;
      else
         ++count_down;
   }
   else if (inc_right>0)
      ++count_right;
}

void report_products(void)
{
   printf("         right: %4d\n"
          "     down-left: %4d\n"
          "          down: %4d\n"
          "    down-right: %4d\n"
          "               ----\n"
          "total products: %4d\n",
          count_right,
          count_down_left,
          count_down,
          count_down_right,
          products);
}

PTYPE get_product(PTYPE row, PTYPE cell, PTYPE inc_down, PTYPE inc_right, PTYPE count)
{
   assert(row<20);
   assert(cell<20);

   if (count==0)
      record_products(inc_down, inc_right);

   PTYPE val = cells[row][cell];
   if(count<3)
      val *= get_product(row+inc_down, cell+inc_right, inc_down, inc_right, count+1);
   return val;
}

PTYPE get_max_from_cell(PTYPE row, PTYPE cell)
{
   PTYPE product, max = 0;

   // If room below, get products straight down, and, if room, down-left and down-right
   if (row<17)
   {
      // Every cell (row<16) can get down:
      max = get_product(row,cell,1,0,0);

      // if room, diagonal down-left:
      if (cell > 2)
      {
         product = get_product(row,cell,1,-1,0);
         if (product > max)
            max = product;
      }

      // If room, diagonal down-right:
      if (cell < 17)
      {
         // get right, to side or down-diagonal
         product = get_product(row,cell,1,1,0);
         if (product > max)
            max = product;
      }
   }

   // If room, get straight right
   if (cell < 17)
   {
      // get right, to side or down-diagonal
      product = get_product(row,cell,0,1,0);
      if (product > max)
         max = product;
   }

   return max;
}

PTYPE get_max_in_row(PTYPE row)
{
   PTYPE cell, max = 0;
   for (PTYPE c=0; c<20; ++c)
   {
      cell = get_max_from_cell(row,c);
      if (cell > max)
         max = cell;
   }

   return max;
}

int main(int argc, char** argv)
{
   if (argc>1)
   {
      FILE *file = fopen(argv[1], "r");
      read_cells(file);
      fclose(file);
   }
   else
      read_cells(stdin);

   PTYPE row, max = 0;
   for (PTYPE r=0; r<20; ++r)
   {
      row = get_max_in_row(r);
      if (row > max)
         max = row;
   }

   printf(PTOK"\n", max);

   if (argc>1)
      report_products();


   return 0;
}
