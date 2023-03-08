#include <stdio.h>
#include <string.h>
#include "cmd_handle.h"
#define CMD_SIZE 64


int main()
{
  char command[CMD_SIZE] = {0};
  while (1){
    printf("TinyShell>");
    fgets(command,CMD_SIZE,stdin); // "ls -l"
    command[strlen(command) -1] = '\0';
    // 'exit':退出标识
    if(strncmp(command,"exit",4) ==0) {
      printf("GoodBye\n");
      break;
    }
    cmd_execute(command);
  }
  return 0;
}

