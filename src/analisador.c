/*
  # Analisador Léxico e Sintático simples para Javascript
  # Disciplina: Paradigmas de Linguagens de Programação
  # Professor: Antonio Josefran de Oliveira Bastos

  ## Descrição
  Para o seguinte programa foram utilizadas algumas bibliotecas padrões do C, como stdlib.h, stdio.h, ctype.h e string.h.
  Também foi necessário a implementação de algumas estruturas de dados como uma lista encadeada de caracteres (CharList) e uma árvore de listas encadeadas (ClstTree).
  A lista encadeada de caracteres foi utilizada para armazenar os lexemas e fazer com que os mesmos não possuam limitação de tamanho.
  A árvore de listas encadeadas foi utilizada para armazenar a árvore de análise sintática para que fosse impressa da forma correta.
  Ambas as implementações foram de autoria da equipe.

  ## Equipe
  - Breno Wesley Teixeira Marques - 494627
  - João Marcos Rocha Souza - 521459
  - Manuel Davi Silveira Rios - 499197
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "clsttree.h"

// Declarações globais
// Variáveis
ClstTree *tree = NULL; // Árvore de análise sintática
int charClass;         // Classe de caracteres do caractere atual
CharList *lexeme;      // Lexema atual
char nextChar;         // Próximo caractere da entrada
int token;             // Código do token atual
int nextToken;         // Próximo código de token
FILE *file;            // Arquivo de dados de entrada

// Constantes
const int DEBUG = 0;

// Classes de caracteres
#define CLASS_LETTER 0
#define CLASS_DIGIT 1
#define CLASS_DOT 2
#define CLASS_QUOTE 3
#define CLASS_VALID_STRING_CHAR 4
#define CLASS_UNKNOWN 99

// Códigos de tokens
#define TK_COMMAND 100
#define TK_STRING 200
#define TK_NUMBER 300
#define TK_SEMICOLON 400
#define TK_LPAREN 401
#define TK_RPAREN 402
#define TK_COMMA 403
#define TK_ADD_OP 500
#define TK_SUB_OP 501
#define TK_MULT_OP 503
#define TK_DIV_OP 504

// Funções dos modos de execução
void terminal_mode();
void file_mode(char str[]);

// Funções do analisador léxico
void lex_add_char();
void lex_get_char();
void lex_get_string_char();
void lex_get_non_blank();
int lex_lexeme();
int lex_lookup(char ch);

// Funções do analisador sintático
void syn_program();
void syn_stmt_list();
void syn_stmt();
void syn_command();
void syn_expr();
void syn_string();
void syn_term();
void syn_factor();

// Funções auxiliares
void error(char str[], int fatal);

// ======================
// Programa
// ======================

int main(int argc, char *argv[])
{

  // Se não for informado o arquivo de entrada, inicia no modo terminal
  if (argc < 2)
  {
    printf("Iniciando no modo terminal...\n");
    terminal_mode();
    return 0;
  }

  printf("Iniciando no modo arquivo...\n");
  file_mode(argv[1]);

  return 0;
}

// ======================
// Modos de execução
// ======================

// Modo terminal
void terminal_mode()
{
  // Lê a entrada do usuário e coloca em um arquivo temporário para ser processado
  // Limitações:
  // - Não lê entradas com espaços ou quebras de linha
  // - Limitado a 100 caracteres
  // - Não deleta o arquivo temporário

  char input[100];
  while (1)
  {
    printf("Digite uma expressao: ");
    scanf("%s", input);
    file = fopen("temp.txt", "w+");
    fprintf(file, "%s", input);
    fclose(file);
    file_mode("temp.txt");
  }
}

// Modo arquivo
void file_mode(char filename[])
{
  // Abrir o arquivo de dados de entrada e processa seu conteúdo
  if ((file = fopen(filename, "r")) == NULL)
    error(strcat("Nao foi possivel abrir o arquivo ", filename), 1);

  // Cria a char list para o lexema, obtém o primeiro caractere e inicia o analisador léxico
  lexeme = clst_create();
  lex_get_char();
  do
  {
    lex_lexeme();  // Analisa o lexema
    syn_program(); // Analisa a sintaxe
  } while (nextToken != EOF);
  fclose(file);

  // Imprime a árvore de análise sintática
  clstt_pretty_print(tree);
}

// ======================
// Analisador léxico
// ======================

// Obtém o próximo caractere da entrada e determina sua classe de caracteres
void lex_get_char()
{
  if ((nextChar = getc(file)) != EOF)
  {
    if (isalpha(nextChar))
      charClass = CLASS_LETTER;
    else if (isdigit(nextChar))
      charClass = CLASS_DIGIT;
    else if (nextChar == '"')
      charClass = CLASS_QUOTE;
    else if (nextChar == '.')
      charClass = CLASS_DOT;
    else
      charClass = CLASS_UNKNOWN;
  }
  else
    charClass = EOF;
}

// Obtém o próximo caractere da entrada para aceitar qualquer caractere em strings, com exceção de " e quebra de linha
void lex_get_string_char()
{
  if ((nextChar = getc(file)) != EOF)
  {
    if (nextChar == '"')
      charClass = CLASS_QUOTE;
    else if (nextChar == '\n')
    {
      error("Houve uma quebra de linha antes do fechamento da string.", 0);
    }
    else
      charClass = CLASS_VALID_STRING_CHAR;
  }
  else
    charClass = EOF;
}

// Analisador léxico que implementa um diagrama de estados
int lex_lexeme()
{
  // Insere o lexema na árvore se ela existir
  if (tree != NULL)
  {
    if (DEBUG)
    {
      printf("lex_lexeme >> ");
      clst_print_inline(lexeme);
      printf("\n");
    }
    clstt_insert_child(tree, clst_create_from_string(clst_to_string(lexeme)));
  }

  // Limpa o último lexema e obtém o próximo caractere não branco
  lexeme = clst_clear(lexeme);
  lex_get_non_blank();

  // Classifica para onde o estado deve transitar
  switch (charClass)
  {
  // Reconhecer comandos
  case CLASS_LETTER:
    lex_add_char();
    lex_get_char();
    while (charClass == CLASS_LETTER || charClass == CLASS_DOT)
    {
      lex_add_char();
      lex_get_char();
    }
    nextToken = TK_COMMAND;
    break;

  // Reconhecer strings
  case CLASS_QUOTE:
    lex_add_char();
    lex_get_string_char();
    while (charClass == CLASS_VALID_STRING_CHAR)
    {
      lex_add_char();
      lex_get_string_char();
    }
    if (charClass == CLASS_QUOTE)
    {
      lex_add_char();
      lex_get_char();
      nextToken = TK_STRING;
    }
    else
      error("Uma string nao foi fechada corretamente com aspas duplas.", 0);
    break;

  // Reconhecer números
  case CLASS_DIGIT:
    lex_add_char();
    lex_get_char();
    while (charClass == CLASS_DIGIT || charClass == CLASS_DOT)
    {
      lex_add_char();
      lex_get_char();
    }
    nextToken = TK_NUMBER;
    break;

  // Parênteses e operadores
  case CLASS_UNKNOWN:
    lex_lookup(nextChar);
    lex_get_char();
    break;

  // Fim do arquivo
  case EOF:
    nextToken = EOF;
    lexeme = clst_insert(lexeme, 'E');
    lexeme = clst_insert(lexeme, 'O');
    lexeme = clst_insert(lexeme, 'F');
    break;
  }

  return nextToken;
}

// Chama lex_get_char até que ela retorne um caractere diferente de espaço em branco
void lex_get_non_blank()
{
  while (isspace(nextChar))
    lex_get_char();
}

// Adiciona nextChar ao lexeme
void lex_add_char()
{
  lexeme = clst_insert(lexeme, nextChar);
}

// Processa operadores e parênteses e retornar o token
int lex_lookup(char ch)
{
  switch (ch)
  {
  case ';':
    lex_add_char();
    nextToken = TK_SEMICOLON;
    break;
  case '(':
    lex_add_char();
    nextToken = TK_LPAREN;
    break;
  case ')':
    lex_add_char();
    nextToken = TK_RPAREN;
    break;
  case ',':
    lex_add_char();
    nextToken = TK_COMMA;
    break;
  case '+':
    lex_add_char();
    nextToken = TK_ADD_OP;
    break;
  case '-':
    lex_add_char();
    nextToken = TK_SUB_OP;
    break;
  case '*':
    lex_add_char();
    nextToken = TK_MULT_OP;
    break;
  case '/':
    lex_add_char();
    nextToken = TK_DIV_OP;
    break;
  default:
    lex_add_char();
    nextToken = EOF;
    break;
  }
  return nextToken;
}

// ======================
// Analisador sintático
// ======================

/*
  # BNF Completo

  <program> -> <stmt_list>
  <stmt_list> -> <stmt> {; <stmt_list>}
  <stmt> -> <command> | <string> | <expr>
  <command> -> console.log(<expr> | <string>)
  <expr> -> <term> {(+ | -) <term>}
  <term> -> <factor> {(* | /) <factor>}
  <factor> -> <number>

  <string> -> Qualquer cadeia de caracteres entre aspas duplas
  <number> -> Qualquer número inteiro ou real
*/

// Analisa as cadeias da regra de produção <program>
// <program> -> <stmt_list>
void syn_program()
{
  tree = clstt_create_node(clst_create_from_string("<program>")); // Cria a raiz da árvore
  if (DEBUG)
    printf("Current tree <program>\n");

  syn_stmt_list();
}

// Analisa as cadeias da regra de produção <stmt_list>
// <stmt_list> -> <stmt> {; <stmt_list>}
void syn_stmt_list()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<stmt_list>")); // Cria um nó filho
  if (DEBUG)
    printf("Current tree <stmt_list>\n");

  syn_stmt();

  while (nextToken == TK_SEMICOLON)
  {
    lex_lexeme();
    syn_stmt();
  }

  tree = tree->father; // Volta para o pai

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// Analisa as cadeias da regra de produção <stmt>
// <stmt> -> <command> | <string> | <expr>
void syn_stmt()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<stmt>"));
  if (DEBUG)
    printf("Current tree <stmt>\n");

  if (nextToken == TK_COMMAND)
    syn_command();
  else if (nextToken == TK_STRING)
    syn_string();
  else
    syn_expr();

  tree = tree->father;

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// Analisa as cadeias da regra de produção <command>
// <command> -> console.log(<expr> | <string>)
void syn_command()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<command>"));
  if (DEBUG)
    printf("Current tree <command>\n");

  if (nextToken == TK_COMMAND)
    lex_lexeme();
  else
    error("Esperava um comando.", 0);

  if (nextToken == TK_LPAREN)
    lex_lexeme();
  else
    error("Esperava um parentese esquerdo.", 0);

  if (nextToken == TK_STRING)
    syn_string();
  else
    syn_expr();

  if (nextToken == TK_RPAREN)
    lex_lexeme();
  else
    error("Esperava um parentese direito.", 0);

  tree = tree->father;

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// Analisa as cadeias da regra de produção <string>
// <string> -> "*"
void syn_string()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<string>"));
  if (DEBUG)
    printf("Current tree <string>\n");

  if (nextToken == TK_STRING)
    lex_lexeme();
  else
    error("Esperava uma string.", 0);

  tree = tree->father;

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// Analisa as cadeias da regra de produção <expr>
// <expr> -> <term> {(+ | -) <term>}
void syn_expr()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<expr>"));
  if (DEBUG)
    printf("Current tree <expr>\n");

  syn_term();

  while (nextToken == TK_ADD_OP || nextToken == TK_SUB_OP)
  {
    lex_lexeme();
    syn_term();
  }

  tree = tree->father;

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// Analisa as cadeias da regra de produção <term>
// <term> -> <factor> {(* | /) <factor>}
void syn_term()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<term>"));
  if (DEBUG)
    printf("Current tree <term>\n");

  syn_factor();

  while (nextToken == TK_MULT_OP || nextToken == TK_DIV_OP)
  {
    lex_lexeme();
    syn_factor();
  }

  tree = tree->father;

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// Analisa as cadeias da regra de produção <factor>
// <factor> -> <number>
void syn_factor()
{
  tree = clstt_insert_child(tree, clst_create_from_string("<factor>"));
  if (DEBUG)
    printf("Current tree <factor>\n");

  if (nextToken == TK_NUMBER)
    lex_lexeme();
  else
    error("Esperava um numero.", 0);

  tree = tree->father;

  if (DEBUG)
  {
    printf("Uplevel to tree ");
    clst_print_inline(tree->charList);
    printf("\n");
  }
}

// ======================
// Funções auxiliares
// ======================

// Imprime uma mensagem de erro e sai do programa
void error(char str[], int fatal)
{
  if (fatal)
    printf("FATAL ");
  printf("ERROR: %s\n", str);
  if (fatal)
  {
    system("pause");
    exit(1);
  }
}