#include <stdio.h>
#include <stdlib.h>
#include "clsttree.h"

ClstTree *clstt_create_node(CharList *charList)
{
  ClstTree *node = (ClstTree *)malloc(sizeof(ClstTree));
  if (node == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }
  node->numChildren = 0;
  node->father = NULL;
  node->children = NULL;
  node->charList = charList;
  return node;
}

ClstTree *clstt_insert_child(ClstTree *father, CharList *childCharList)
{
  if (father == NULL)
  {
    printf("Father is NULL!\n");
    exit(1);
  }

  ClstTree *child = clstt_create_node(childCharList);

  if (father->numChildren == 0)
    father->children = malloc(sizeof(ClstTree *));
  else
    father->children = realloc(father->children, sizeof(ClstTree *) * (father->numChildren + 1));

  if (father->children == NULL)
  {
    printf("Not enough memory!\n");
    exit(1);
  }

  child->father = father;
  father->children[father->numChildren++] = child;
  return child;
}

void clstt_pretty_print(ClstTree *node)
{
  static CharList *indentation = NULL;
  static int iteration = 0;

  if (node == NULL)
    return;

  clst_print_inline(node->charList);
  printf("\n");

  if (node->numChildren == 0)
    return;

  if (iteration > 0)
    indentation = clst_insert(indentation, '|');

  int lastFatherNode = node->father != NULL && node->father->children[node->father->numChildren - 1] == node;

  if (lastFatherNode)
    indentation = clst_remove_last(indentation);

  for (int i = 0; i < node->numChildren; i++)
  {

    CharList *iAux = indentation;
    while (iAux != NULL)
    {
      if (iAux->info == '|')
        printf("%c    ", 124); // 124(│)
      else
        printf("     ");
      iAux = iAux->prox;
    }

    int nextWillBeLeaf = i == node->numChildren - 1;

    if (nextWillBeLeaf)
    {
      indentation = clst_insert(indentation, ' ');
      printf("%c%c%c> ", 192, 196, 196); // 192(└) 196(─)
    }
    else
      printf("%c%c%c> ", 195, 196, 196); // 195(├) 196(─)

    iteration++;
    clstt_pretty_print(node->children[i]);

    if (nextWillBeLeaf && !lastFatherNode)
      indentation = clst_remove_last(indentation);
  }
  if (iteration > 0)
    indentation = clst_remove_last(indentation);
}
