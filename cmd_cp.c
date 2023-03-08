#include "cmd_cp.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
int cmd_cp_execute(cmd_t *pcmd) {
  if (pcmd == NULL) return -1;
  int ret;
#ifdef DEBUG
  print_command_struct(pcmd);
#endif
  cp_file_info_t file_info;
//  解析路径并存储到 cp_file_info_t 结构
  ret = cmd_cp_parse_path(&file_info, pcmd);
  if (ret == -1) return -1;
  ret = cmd_cp_parse_type(&file_info);
  if (ret == -1) return -1;
  ret = cmd_cp_dispatch(&file_info);
  if (ret == -1) return -1;
  return 0;
}
int cmd_cp_parse_type(cp_file_info_t *file_info) {
  enum file_type ftype;
  ftype = get_file_type(file_info->src_path);
  if (ftype == FT_ERROR || ftype == FT_UNKNOWN) return -1;
  file_info->src_ftype = ftype;
#ifdef DEBUG
  if (file_info->src_ftype == FT_FILE) printf("FILE.\n");
  else if (file_info->src_ftype == FT_DIR) printf("Dir.\n");
#endif
  return 0;
}
// cp 1.txt 2.txt cmd_arg_list[0]: "1.txt" cmd_arg_list[1]: "2.txt"
int cmd_cp_parse_path(cp_file_info_t *file_info, cmd_t *pcmd) {
  if (file_info == NULL || pcmd == NULL) return -1;
  strcpy(file_info->src_path, pcmd->cmd_arg_list[0]);
  strcpy(file_info->dest_path, pcmd->cmd_arg_list[1]);
#ifdef DEBUG
  printf("[DEBUG] src path < %s >\n", file_info->src_path);
  printf("[DEBUG] dest path < %s >\n", file_info->dest_path);
#endif
  return 0;
}

enum file_type get_file_type(const char *path) {
  int ret;
  struct stat statbuf;
  ret = stat(path, &statbuf);
  if (ret == -1) {
    perror("stat():");
    return FT_ERROR;
  }
//  是否为目录
  if (S_ISDIR(statbuf.st_mode)) return FT_DIR;
  else if (S_ISREG(statbuf.st_mode)) return FT_FILE;
  return FT_UNKNOWN;
}
int cmd_cp_dispatch(cp_file_info_t *file_info) {
  if (file_info->src_ftype == FT_FILE) {  // cp 1.txt ./xxx/1.txt
    printf(" cp file. \n");
    return cmd_cp_file(file_info->src_path, file_info->dest_path);
  }else if (file_info->src_ftype == FT_DIR){
    printf(" cp dir. \n");
    return cmd_cp_directory(file_info->src_path, file_info->dest_path);
  }
  return 0;
}
int cmd_cp_file(const char *src, const char *dest) {
  if (src == NULL || dest == NULL) return -1;
  FILE *fp_src = NULL, *fp_dest = NULL;
  fp_src = fopen(src, "r");
  if (fp_src == NULL) {
    perror("[ERROR] src fopen(): ");
  }
  fp_dest = fopen(dest, "w+");
  if (fp_dest == NULL) {
    perror("[ERROR] dest fopen(): ");
  }
  size_t rbytes = 0, wbytes = 0;
  char buffer[SZ_BUFFER] = {0};
  while (1) {
    rbytes = fread(buffer, sizeof(char), SZ_BUFFER, fp_src);
    if (rbytes > 0) {
      wbytes = fwrite(buffer, sizeof(char), rbytes, fp_dest);
      if (wbytes != rbytes) {
        perror("[ERROR] fwrite(): ");
        return -1;
      }
    } else {
      break;
    }
  }
  fclose(fp_src);
  fclose(fp_dest);
  return 0;
}
int cmd_cp_directory(const char *src, const char *dest) {
  int ret;
  DIR *pdir = NULL;
  struct dirent *pdirent = NULL;
  cp_file_info_t file_info;
//  创建目标目录
  ret = mkdir(dest,0777);
  if (ret == -1){
    perror("[DEBUG] mkdir(): ");
    return -1;
  }
//  打开源目录
  pdir = opendir(src);
  if(pdir == NULL){
    perror("[ERROR] opendir(): ");
    return -1;
  }
//  遍历目录
  while (1) {
    pdirent = readdir(pdir);
    if(pdirent == NULL) break;
    if (strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") ==0) continue;
//    路径合成
    make_path(&file_info,src,dest,pdirent->d_name);
#ifdef DEBUG
    printf("[DEBUG]: src path < %s >\n", file_info.src_path);
    printf("[DEBUG]: dest path < %s >\n", file_info.dest_path);
#endif
//    获取文件类型
    file_info.src_ftype = get_file_type(file_info.src_path);
    if(file_info.src_ftype == FT_DIR) {
      cmd_cp_directory(file_info.src_path, file_info.dest_path);
    }else if(file_info.src_ftype == FT_FILE) {
      cmd_cp_file(file_info.src_path, file_info.dest_path);
    }
  }
  closedir(pdir);
  return 0;
}
// cp test test1
void make_path(cp_file_info_t *file_info,const char *spath, const char *dpath, const char *filename)
{
  memset(file_info->src_path,0,sizeof(file_info->src_path));
  memset(file_info->dest_path,0,sizeof(file_info->dest_path));
  strcpy(file_info->src_path,spath);
  strcat(file_info->src_path,"/");
  strcat(file_info->src_path,filename);

  strcpy(file_info->dest_path,dpath);
  strcat(file_info->dest_path,"/");
  strcat(file_info->dest_path,filename);
}
