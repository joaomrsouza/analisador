#include <stdio.h>
#include <ctype.h>
#include "charlist.h"
#include <string.h>

void validateArgs(int argc, char *argv[]);

int main(int argc, char *argv[])
{

  for (int i = 0; i < argc; i++)
  {
    printf("%s\n", argv[i]);
  }

  validateArgs(argc, argv);

  return 0;
}

void validateArgs(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("nao informado tudo");
    return;
  }

  char fileFlag = 'f';
  char modeFlag = 'm';

  if (strlen(argv) != 2 || argv[1][0] != '-')
  {
    printf("use uma flag");
  }

  if (argv[1][1] != fileFlag || argv[1][1] != modeFlag)
  {
    printf("flag invalida");
  }

  if (strlen(argv[2]) == 0)
  {
    printf("informe o valor da flag");
  }
}