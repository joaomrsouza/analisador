/* front.c - um analisador léxico e analisador sintático
simples para expressões aritméticas simples */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* Exec mode */
#define EM_RAW_PRINT 0
#define EM_PRETTY_PRINT 1
#define CURRENT_EM EM_PRETTY_PRINT
int already_printed_lexeme = 0;
int indentation_level = -1;

/* Declarações globais */
/* Variáveis */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Declarações de Funções */
void addChar();
void getChar();
void getNonBlank();
int lex();
int lookup(char ch);
void expr();
void term();
void factor();
void error();
void pretty_print(char *str[]);

/* Classes de caracteres */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Códigos de tokens */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/******************************************************/
/* função principal */
int main()
{
  /* Abrir o arquivo de dados de entrada e processar seu
  conteúdo */
  if ((in_fp = fopen("front.in", "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
  else
  {
    getChar();
    do
    {
      already_printed_lexeme = 1;
      lex();
      expr();
    } while (nextToken != EOF);
  }
  return 0;
}

/*******************************************************/
/* getChar - uma função para obter o próximo caractere da entrada
e determinar sua classe de caracteres */
void getChar()
{
  if ((nextChar = getc(in_fp)) != EOF)
  {
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else
      charClass = UNKNOWN;
  }
  else
    charClass = EOF;
}

/********************************************************/
/* lex - um analisador léxico simples para expressões
aritméticas */
int lex()
{

  if (CURRENT_EM == EM_PRETTY_PRINT && !already_printed_lexeme)
  {
    already_printed_lexeme = 1;
    indentation_level++;
    pretty_print(lexeme);
    indentation_level--;
  }
  already_printed_lexeme = 0;
  lexLen = 0;
  getNonBlank();
  switch (charClass)
  {
  /* Reconhecer identificadores */
  case LETTER:
    addChar();
    getChar();
    while (charClass == LETTER || charClass == DIGIT)
    {
      addChar();
      getChar();
    }
    nextToken = IDENT;
    break;
  /* Reconhecer literais inteiros */
  case DIGIT:
    addChar();
    getChar();
    while (charClass == DIGIT)
    {
      addChar();
      getChar();
    }
    nextToken = INT_LIT;
    break;
  /* Parênteses e operadores */
  case UNKNOWN:
    lookup(nextChar);
    getChar();
    break;
  /* Fim do arquivo */
  case EOF:
    nextToken = EOF;
    lexeme[0] = 'E';
    lexeme[1] = 'O';
    lexeme[2] = 'F';
    lexeme[3] = 0;
    break;
  } /* Fim do switch */
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);

  return nextToken;
} /* Fim da função lex */

/*******************************************************/
/* getNonBlank - uma função para chamar getChar até que ela
retorne um caractere diferente de espaço em
branco */
void getNonBlank()
{
  while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* addChar - uma função para adicionar nextChar ao
vetor lexeme */
void addChar()
{
  if (lexLen <= 98)
  {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
    printf("Error - lexeme is too long \n");
}

/******************************************************/
/* lookup - uma função para processar operadores e parênteses
e retornar o token */
int lookup(char ch)
{
  switch (ch)
  {
  case '(':
    addChar();
    nextToken = LEFT_PAREN;
    break;
  case ')':
    addChar();
    nextToken = RIGHT_PAREN;
    break;
  case '+':
    addChar();
    nextToken = ADD_OP;
    break;
  case '-':
    addChar();
    nextToken = SUB_OP;
    break;
  case '*':
    addChar();
    nextToken = MULT_OP;
    break;
  case '/':
    addChar();
    nextToken = DIV_OP;
    break;
  case '=':
    addChar();
    nextToken = ASSIGN_OP;
    break;
  default:
    addChar();
    nextToken = EOF;
    break;
  }
  return nextToken;
}

/* expr
Analisa sintaticamente cadeias na linguagem gerada pela
regra:
<expr> -> <term> {(+ | -) <term>}
*/
void expr()
{
  indentation_level++;
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Enter <expr>\n");
  if (CURRENT_EM == EM_PRETTY_PRINT)
    pretty_print("<expr>");
  /* Analisa sintaticamente o primeiro termo */
  term();
  /* Desde que o próximo token seja + ou -, obtenha o próximo
  token e analise sintaticamente o próximo termo */
  while (nextToken == ADD_OP || nextToken == SUB_OP)
  {
    lex();
    term();
  }
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Exit <expr>\n");
  if (CURRENT_EM == EM_PRETTY_PRINT)
    pretty_print("</expr>");
  indentation_level--;
} /* Fim da função expr */

/* term
Analisa sintaticamente cadeias na linguagem gerada pela
regra:
<term> -> <factor> {(* | /) <factor>)
*/
void term()
{
  indentation_level++;
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Enter <term>\n");
  if (CURRENT_EM == EM_PRETTY_PRINT)
    pretty_print("<term>");
  /* Analisa sintaticamente o primeiro termo */
  factor();
  /* Desde que o próximo token seja + ou -, obtenha o próximo
  token e analise sintaticamente o próximo termo */
  while (nextToken == MULT_OP || nextToken == DIV_OP)
  {
    lex();
    factor();
  }
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Exit <term>\n");
  if (CURRENT_EM == EM_PRETTY_PRINT)
    pretty_print("</term>");
  indentation_level--;
} /* Fim da função term */

/* factor
Analisa sintaticamente cadeias na linguagem gerada pela
regra:
<factor> -> id | int_constant | (<expr)
*/
void factor()
{
  indentation_level++;
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Enter <factor>\n");
  if (CURRENT_EM == EM_PRETTY_PRINT)
    pretty_print("<factor>");
  /* Determina qual RHS */
  if (nextToken == IDENT || nextToken == INT_LIT)
    /* Obtém o próximo token */
    lex();
  /* Se a RHS é (<expr>), chame lex para passar o parêntese
  esquerdo, chame expr e verifique pelo parêntese
  direito */
  else
  {
    if (nextToken == LEFT_PAREN)
    {
      lex();
      expr();
      if (nextToken == RIGHT_PAREN)
        lex();
      else
        error();
    } /* Fim do if (nextToken == ... */
    /* Não era um identificador, um literal inteiro ou um
    parêntese esquerdo */
    else
      error();
  } /* Fim do else */
  if (CURRENT_EM == EM_RAW_PRINT)
    printf("Exit <factor>\n");
  if (CURRENT_EM == EM_PRETTY_PRINT)
    pretty_print("</factor>");
  indentation_level--;
} /* Fim da função factor */

void error()
{
  printf("Error\n");
  exit(1);
}

void pretty_print(char *str[])
{
  for (int i = 0; i < indentation_level; i++)
  {
    printf("  ");
  }
  printf("%s\n", str);
}