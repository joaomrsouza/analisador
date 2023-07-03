#include <stdio.h>
#include <stdlib.h>
#include "clsttree.h"

void print_tree(ClstTree *root);
void pretty_print_tree(ClstTree *root, int last);

int main()
{
  ClstTree *root = clstt_create_node(clst_create_from_string("abc"));
  ClstTree *child1 = clstt_insert_child(root, clst_create_from_string("123"));
  ClstTree *child2 = clstt_insert_child(root, clst_create_from_string("987"));

  ClstTree *child11 = clstt_insert_child(child1, clst_create_from_string("teste11"));
  clstt_insert_child(child11, clst_create_from_string("teste111"));
  clstt_insert_child(child11, clst_create_from_string("teste112"));

  ClstTree *child12 = clstt_insert_child(child1, clst_create_from_string("teste12"));
  clstt_insert_child(child12, clst_create_from_string("teste121"));

  clstt_insert_child(child1, clst_create_from_string("teste13"));
  clstt_insert_child(child2, clst_create_from_string("teste21"));
  clstt_insert_child(child2, clst_create_from_string("teste22"));
  clstt_insert_child(child2, clst_create_from_string("teste23"));

  // print_tree(root);
  // printf("\n\n\n");
  // pretty_print_tree(root, 0);
  clstt_pretty_print(root);

  return 0;
}

void print_tree(ClstTree *root)
{
  if (root == NULL)
  {
    return;
  }

  clst_print_inline(root->charList);
  printf("\n");

  int i;
  for (i = 0; i < root->numChildren; i++)
  {
    print_tree(root->children[i]);
  }
}

void pretty_print_tree(ClstTree *node, int last)
{
  static int indentation_level = -1;

  if (node == NULL)
    return;

  clst_print_inline(node->charList);
  printf("\n");

  if (node->numChildren == 0)
    return;

  indentation_level++;
  for (int i = 0; i < node->numChildren; i++)
  {
    for (int j = 0; j < indentation_level; j++)
    {
      if (last)
        printf("     ");
      else
        printf("|    ");
    }
    printf("+--> ");
    pretty_print_tree(node->children[i], i == node->numChildren - 1);
    if (i == node->numChildren - 1)
    {
      for (int j = 0; j < indentation_level; j++)
        if (last)
          printf("     ");
        else
          printf("|    ");
      printf("\n");
    }
  }
  indentation_level--;
}