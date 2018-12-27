/*
 * This Makefile is free software; the Free Software Foundation
 * gives unlimited permission to copy, distribute and modify it.
 */

#include <stdio.h>

int main(int argc, char** argv)
{
   while(!feof(stdin)) {
      float c1, c2, c3;
      scanf("%f %f %f", &c1, &c2, &c3);
      printf("%g %g %g\n", c1 + 0.1, c2 + 0.2, c3 + 0.35); 
   }
   return 0;
}
