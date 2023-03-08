#include "cmd_handle.h"
#include "cmd_cp.h"
#include "cmd_ls.h"
int cmd_execute(char *cmd_str)
{
  cmd_t command;
  if(NULL == cmd_str) return -1;
  init_command_struct(&command); // 初始化结构体
#ifdef DEBUG
//  printf("[DEBUG]: cmd_str<%s>\n",cmd_str);
  print_command_struct(&command);
#endif
  int ret = cmd_parse(cmd_str, &command); // 解析命令字符串
  if(ret == -1) return -1;
#ifdef DEBUG
  print_command_struct(&command);
#endif
  ret = cmd_dispatch(&command);
  if (ret == -1) return -1;
  return 0;
}
// 初始化命令
void init_command_struct(cmd_t *pcmd)
{
  memset(pcmd->cmd_name,0, SZ_NAME);
  for (int i = 0; i < SZ_COUNT; i++) {
    memset(pcmd->cmd_arg_list[i],0,SZ_ARG);
  }
  pcmd->cmd_arg_count = 0;
}

// 打印调试
void print_command_struct(cmd_t *pcmd)
{
  printf("----------------------\n");
  printf("[DEBUG] cmd name< %s >\n", pcmd->cmd_name);
  printf("[DEBUG] cmd arg count< %d >\n", pcmd->cmd_arg_count);
  printf("[DEBUG] cmd arg list : ");
  for (int i = 0; i < pcmd->cmd_arg_count; i++) {
    printf("%s",pcmd->cmd_arg_list[i]);
  }
  printf("\n----------------------\n");
}

// "cp 1.txt 2.txt"
int cmd_parse(char *cmd_srt, cmd_t *pcmd) {
  if(cmd_srt ==NULL || pcmd ==NULL) return -1;
  char *p_cmd_name = NULL;
  char *p_cmd_arg = NULL;
  int index = 0;
  p_cmd_name = strtok(cmd_srt," ");
#ifdef DEBUG
  printf("[DEBUG] cmd name : < %s> \n", p_cmd_name);
#endif
  strcpy(pcmd->cmd_name, p_cmd_name);
  while (1) {
    p_cmd_arg = strtok(NULL," ");// 获取分割之后的字符串的地址
    if(p_cmd_arg == NULL) break;
    strcpy(pcmd->cmd_arg_list[index++],p_cmd_arg); // 将字符串拷贝到结构体中
    pcmd->cmd_arg_count = index; // 参数个数存储
  }
  return 0;
}

int cmd_dispatch(cmd_t *pcmd)
{
  if (pcmd ==NULL) return -1;
  if (strcmp(pcmd->cmd_name,"ls") == 0) {
//    调用ls 命令处理模块
      cmd_ls_execute(pcmd);
#ifdef DEBUG
    printf("ls handle.\n");
#endif
  } else if (strcmp(pcmd->cmd_name,"cp") == 0) {
#ifdef DEBUG
    printf("cp command handle.\n");
#endif
    // 调用CP命令处理模块
    cmd_cp_execute(pcmd);
  }
  return 0;
}