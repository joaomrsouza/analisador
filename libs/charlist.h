typedef struct char_list
{
  char info;
  struct char_list *prox;
} CharList;

// Cria uma lista de char vazia.
CharList *clst_create();
// Cria uma lista de char a partir de uma string.
CharList *clst_create_from_string(char str[]);
// Limpa a lista de char.
CharList *clst_clear(CharList *l);
// Verifica se a lista de char está vazia.
int clst_empty(CharList *l);
// Insere um char no fim da lista de char.
CharList *clst_insert(CharList *l, char info);
// Imprime a lista de char em uma linha.
void clst_print_inline(CharList *l);
// Libera a memória alocada para a lista de char.
void clst_free(CharList *l);
// Retorna o tamanho da lista de char.
int clst_length(CharList *l);
// Retorna a lista de char como uma string.
char *clst_to_string(CharList *l);
// Remove o último char da lista de char.
CharList *clst_remove_last(CharList *l);