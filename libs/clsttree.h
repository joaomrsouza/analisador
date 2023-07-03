#include "charlist.h"

typedef struct clst_tree
{
  CharList *charList;
  int numChildren;
  struct clst_tree **children;
  struct clst_tree *father;
} ClstTree;

// Cria uma árvore de lista de char vazia.
ClstTree *clstt_create_node(CharList *charList);
// Insere um filho na árvore de lista de char com a lista de char informada.
ClstTree *clstt_insert_child(ClstTree *father, CharList *childCharList);
// Imprime a árvore de lista de char.
void clstt_pretty_print(ClstTree *node);