#ifndef __CMD_LS_H_
#define __CMD_LS_H_

#include "cmd_handle.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#define SZ_LS_NAME 64
#define SZ_LS_PERMISSION 10
#define SZ_LS_TIME 32
#define SZ_LS_LINK_CONTENT 64

#define DEBUG
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

typedef struct file_attribute {
  struct stat f_attr_stat_info; // 保留系统原来的结构

  char f_attr_type;     // 文件类型
  char f_attr_uname[SZ_LS_NAME];// user name
  char f_attr_gname[SZ_LS_NAME];// group name
  char f_attr_mtime[SZ_LS_TIME]; // 最后一次修改时间
  char f_attr_permission[SZ_LS_PERMISSION];// 权限
  char f_attr_name[SZ_LS_NAME];
  char f_attr_link_content[SZ_LS_LINK_CONTENT];
} file_attr_t;
int cmd_ls_execute(cmd_t *pcmd);
int cmd_list_directory(const char *dirpath);
int get_file_attr(struct file_attribute *pattr, const char *path, const char *filename, bool islink);
void show_file_attributes(struct file_attribute *pattr);
void make_path_ls(char *path, const char *dirpath, const char *filename); //合成具体文件的路径
int get_file_type_ls(struct file_attribute *pattr); // 获取文件类型
int get_file_permission(struct file_attribute *pattr); // 获取文件权限
void get_file_uname(struct file_attribute *pattr); // 获取用户名
void get_file_gname(struct  file_attribute *pattr); // 获取用户组名
void get_file_last_modify_time(struct file_attribute *pattr);
#endif
