#include <stdio.h>
#include "malloc.h"

int main()
{
  int *p, *p2, *p3;
  p = calloc(1, 1000000);
  p = realloc(p, 2000000);
  p2 = calloc(1, 1000);
  p3 = calloc(1, 100);
  free(p2);
  p3 = realloc(p3, 500);
  p2 = malloc(600);
  free(p);
  free(p2);
  free(p3);
  printf("lel\n");
  return 0;
}
