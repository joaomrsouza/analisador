# analisador

## Executando

1. Abra a pasta com o VSCode
2. Instale as seguintes extensões:
   - [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
   - [Trigger Task on Save](https://marketplace.visualstudio.com/items?itemName=Gruntfuggly.triggertaskonsave)
3. Configure o caminho padrão do compilador (GCC)

```bash
"C_Cpp.default.compilerPath": "C:/.../MinGW/bin/g++.exe"
```

4. Abra cada arquivo .c da pasta libs e salve (Ctrl + S). Tenha certeza de que a task de compilação foi executada com sucesso (é pra ser gerado um arquivo .o para cada arquivo .c).
5. Abra o arquivo principal (analisador.c) e execute a task de teste via comando (F1 -> tasks: run test task).
6. O resultado será exibido no terminal.

## Problemas

### File not found

Se o programa não encontrar o arquivo, execute o programa via terminal externo (cmd) com o comando:

```bash
  ./analisador.exe input.txt
```

## Debug

### Configuração

Tenha certeza de que o arquivo ./.vscode/launch.json esteja com o caminho correto do debugger. O caminho padrão é:

```bash
  "miDebuggerPath": "C:/.../MinGW/bin/gdb32.exe",
```

### Executando

Para debugar um arquivo, basta abrir o arquivo e pressionar F5. O arquivo de configuração já está pronto.
