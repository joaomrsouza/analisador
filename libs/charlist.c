#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "charlist.h"

CharList *clst_create()
{
  return NULL;
}

CharList *clst_create_from_string(char str[])
{
  CharList *charList = clst_create();
  for (int i = 0; i < strlen(str); i++)
  {
    charList = clst_insert(charList, str[i]);
  }
  return charList;
}

CharList *clst_clear(CharList *l)
{
  clst_free(l);
  return clst_create();
}

int clst_empty(CharList *l)
{
  return l == NULL;
}

CharList *clst_insert(CharList *l, char info)
{
  CharList *p = (CharList *)malloc(sizeof(CharList));
  if (p == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }

  p->info = info;
  p->prox = NULL;

  if (l == NULL)
  {
    return p;
  }

  CharList *lAux = l;
  while (lAux->prox != NULL)
  {
    lAux = lAux->prox;
  }
  lAux->prox = p;
  return l;
}

void clst_print_inline(CharList *l)
{
  CharList *lAux = l;
  while (lAux != NULL)
  {
    printf("%c", lAux->info);
    lAux = lAux->prox;
  }
}

void clst_free(CharList *l)
{
  CharList *lProx;
  while (l != NULL)
  {
    lProx = l->prox;
    free(l);
    l = lProx;
  }
}

int clst_length(CharList *l)
{
  int count = 0;
  CharList *aux = l;
  while (aux != NULL)
  {
    count++;
    aux = aux->prox;
  }
  return count;
}

char *clst_to_string(CharList *l)
{
  if (l == NULL)
    return NULL;

  int length = clst_length(l);
  char *str = (char *)malloc(sizeof(char) * (length + 1));
  if (str == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }

  CharList *lAux = l;
  for (int i = 0; i < length; i++)
  {
    str[i] = lAux->info;
    lAux = lAux->prox;
  }
  str[length] = '\0';
  return str;
}

CharList *clst_remove_last(CharList *l)
{
  if (l == NULL)
    return NULL;

  if (l->prox == NULL)
  {
    free(l);
    return NULL;
  }

  CharList *lAux = l;
  while (lAux->prox->prox != NULL)
  {
    lAux = lAux->prox;
  }
  free(lAux->prox);
  lAux->prox = NULL;
  return l;
}