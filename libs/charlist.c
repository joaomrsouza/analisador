#include <stdio.h>
#include <stdlib.h>
#include "charlist.h"

struct char_list
{
  char info;
  CharList *prox;
};

CharList *clst_create()
{
  return NULL;
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

  // Inserir no final da lista
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

CharList *clst_search(CharList *l, char info)
{
  CharList *lAux = l;
  while (lAux != NULL)
  {
    if (lAux->info == info)
      return lAux;
    lAux = lAux->prox;
  }
  return lAux; // NULL
}

void clst_print(CharList *l)
{
  CharList *lAux = l;
  while (lAux != NULL)
  {
    printf("%c\n", lAux->info);
    lAux = lAux->prox;
  }
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

CharList *clst_remove(CharList *l, char info)
{
  if (l != NULL)
  {
    CharList *lAux = l->prox;
    if (l->info == info)
    {
      free(l);
      return lAux;
    }
    else
    {
      CharList *lAnt = l;
      while (lAux != NULL)
      {
        if (lAux->info == info)
        {
          lAnt->prox = lAux->prox;
          free(lAux);
          break;
        }
        else
        {
          lAnt = lAux;
          lAux = lAux->prox;
        }
      }
    }
  }
  return l;
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
