#include <stdio.h>
#include <ctype.h>

/* Declarações globais */
/* Variáveis */
int charClass;
char lexeme[100]; // TODO: transformar em lista de tamanho dinâmico
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Classes de caracteres */
#define CLASS_LETTER 0
#define CLASS_DIGIT 1
#define CLASS_DOT 2
#define CLASS_QUOTE 3
#define CLASS_VALID_STRING_CHAR 4
#define CLASS_UNKNOWN 99

/* Códigos de tokens */
#define TK_COMMAND 100
#define TK_STRING 200
#define TK_FACTOR 300
#define TK_SEMICOLON 400
#define TK_LPAREN 401
#define TK_RPAREN 402
#define TK_ADD_OP 500
#define TK_SUB_OP 501
#define TK_MULT_OP 503
#define TK_DIV_OP 504

/* Declarações de Funções */
void addChar();
void getChar();
void getStringChar();
void getNonBlank();
int lex();
int lookup(char ch);

int main(int argc, char *argv[])
{
  if (argc < 1)
  {
    printf("Passe o arquivo para processar via argumento. Exemplo: input.txt\n");
    return 1;
  }

  if ((in_fp = fopen(argv[1], "r")) == NULL)
    printf("ERROR - cannot open %s \n", argv[1]);
  else
  {
    getChar();
    do
    {
      lex();
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

/********************************************************/
/* getStringChar - uma função para obter o próximo caractere da entrada
para aceitar qualquer caractere em strings, com exceção de " e quebra de linha */
void getStringChar()
{
  if ((nextChar = getc(in_fp)) != EOF)
  {
    if (nextChar == '"')
      charClass = CLASS_QUOTE;
    else if (nextChar == '\n')
    {
      // TODO: Throw error
      printf("Erro");
    }
    else
      charClass = CLASS_VALID_STRING_CHAR;
  }
  else
    charClass = EOF;
}

/********************************************************/
/* lex - um analisador léxico simples para expressões
aritméticas */
int lex()
{
  lexLen = 0;
  getNonBlank();
  switch (charClass)
  {
  /* Reconhecer comandos */
  case CLASS_LETTER:
    addChar();
    getChar();
    while (charClass == CLASS_LETTER || charClass == CLASS_DOT)
    {
      addChar();
      getChar();
    }
    nextToken = TK_COMMAND;
    break;
  /* Reconhecer strings */
  case CLASS_QUOTE:
    addChar();
    getStringChar();
    while (charClass == CLASS_VALID_STRING_CHAR)
    {
      addChar();
      getStringChar();
    }
    if (charClass == CLASS_QUOTE)
    {
      addChar();
      getChar();
      nextToken = TK_STRING;
    }
    else
    {
      // TODO: Throw error
      printf("Erro");
    }
    break;
  /* Reconhecer fatores */
  case CLASS_DIGIT:
    addChar();
    getChar();
    while (charClass == CLASS_DIGIT || charClass == CLASS_DOT)
    {
      addChar();
      getChar();
    }
    nextToken = TK_FACTOR;
    break;
  /* Parênteses e operadores */
  case CLASS_UNKNOWN:
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
  printf("token: %d, lexeme: %s\n", nextToken, lexeme);
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
  case ';':
    addChar();
    nextToken = TK_SEMICOLON;
    break;
  case '(':
    addChar();
    nextToken = TK_LPAREN;
    break;
  case ')':
    addChar();
    nextToken = TK_RPAREN;
    break;
  case '+':
    addChar();
    nextToken = TK_ADD_OP;
    break;
  case '-':
    addChar();
    nextToken = TK_SUB_OP;
    break;
  case '*':
    addChar();
    nextToken = TK_MULT_OP;
    break;
  case '/':
    addChar();
    nextToken = TK_DIV_OP;
    break;
  default:
    addChar();
    nextToken = EOF;
    break;
  }
  return nextToken;
}