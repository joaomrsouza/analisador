#include <stdio.h>
#include <ctype.h>
#include "charlist.h"

int main()
{
  // if ('\n' == '\n')
  // {
  //   printf("true");
  // }
  // else
  // {
  //   printf("false");
  // }
  CharList *l = clst_create();
  l = clst_insert(l, 'a');
  l = clst_insert(l, 'b');
  l = clst_insert(l, 'c');
  clst_print(l);
  printf("\n");
  clst_free(l);

  return 0;
}