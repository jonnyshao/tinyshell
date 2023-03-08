#ifndef __CMD_LS_H_
#define __CMD_LS_H_

#include "cmd_handle.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SZ_LS_NAME 64
#define SZ_LS_PERMISSION 10
#define SZ_LS_TIME 32
#define SZ_LS_LINK_CONTENT 64

#define DEBUG

struct file_attribute
{
  struct stat f_attr_stat_info; // 保留系统原来的结构

  char f_attr_type;     // 文件类型
  char f_attr_uname[SZ_LS_NAME];// user name
  char f_attr_gname[SZ_LS_NAME];// group name
  char f_attr_mtime[SZ_LS_TIME]; // 最后一次修改时间
  char f_attr_permission[SZ_LS_PERMISSION];// 权限
  char f_attr_name[SZ_LS_NAME];
  char f_attr_link_content[SZ_LS_LINK_CONTENT];
};
int cmd_ls_execute(cmd_t* pcmd);
int cmd_list_directory(const char *dirpath);
#endif
