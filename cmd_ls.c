#include "cmd_ls.h"
#include <dirent.h>
int cmd_ls_execute(cmd_t *pcmd) {
  int ret;
  if (pcmd == NULL) return -1;
  if (pcmd->cmd_arg_count != 2) {
    fprintf(stderr, "Command argument Error. \n");
    return -1;
  }
  if (pcmd->cmd_arg_list[1] != NULL) return cmd_list_directory(pcmd->cmd_arg_list[1]);
  return 0;
}

int cmd_list_directory(const char *dirpath) {
  DIR *pdir = NULL;
  struct dirent *pdirent = NULL;
  char path[128] = {0};

  pdir = opendir(dirpath);
  if (pdir == NULL) {
    perror("open(): ");
    return -1;
  }
  file_attr_t attr;
  while ((pdirent = readdir(pdir)) != NULL) {
    if (strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
      continue;
#ifdef DEBUG
    printf("[DEBUG] filenmae < %s > \n", pdirent->d_name);
#endif
    memset(&attr, 0, sizeof(attr)); // 内存清零
    make_path_ls(path,dirpath,pdirent->d_name);
    if(pdirent->d_type == DT_LNK) { // 软链接文件
      get_file_attr(&attr, path, pdirent->d_name, true);
    } else {
      get_file_attr(&attr, path, pdirent->d_name, false);
    }
    show_file_attributes(&attr);
  }

  closedir(pdir);
  return 0;
}
/*
 * pattr:自定义文件属性结构体指针
 * path: 目录路径
 * filename: 文件名
 * islink:是否为软链接
*/

int get_file_attr(struct file_attribute *pattr, const char *path, const char *filename, bool islink) {
  int ret;
  if (islink) {
    ret = lstat(path, &pattr->f_attr_stat_info);
  } else {
    ret = stat(path, &pattr->f_attr_stat_info);
  }
  if (ret == -1) {
    perror("[ERROR] stat(): ");
    return -1;
  }
  get_file_type_ls(pattr);
  get_file_permission(pattr);
  get_file_uname(pattr);
  get_file_gname(pattr);
  get_file_last_modify_time(pattr);
  strcpy(pattr->f_attr_name,filename);
  if (pattr->f_attr_type =='l') {
     // 读取软链接文件的内容并存储到结构体中
    ret = readlink(path, pattr->f_attr_link_content, sizeof(pattr->f_attr_link_content));
    if (ret == -1) {
      perror("readlink():");
      return -1;
    }
  }
  return 0;
}

void show_file_attributes(struct file_attribute *pattr) {
  printf(" %c", pattr->f_attr_type); // 文件类型
  printf("%s", pattr->f_attr_permission); // 权限
  printf("%hu", pattr->f_attr_stat_info.st_nlink); // 硬链接数
  printf("%s",pattr->f_attr_uname); // 用户名
  printf("%s",pattr->f_attr_gname); // 用户组名
  printf("%lld",pattr->f_attr_stat_info.st_size); // 文件尺寸
  printf("%s",pattr->f_attr_mtime);
  if (pattr->f_attr_type =='l') {
    printf("%s -> %s\n",pattr->f_attr_name,pattr->f_attr_link_content);
  } else {
    printf("%s \n", pattr->f_attr_name);
  }

  putchar('\n');
}
// ls -l test
void make_path_ls(char *path, const char *dirpath, const char *filename) {
  strcpy(path, dirpath);
  strcat(path, "/");
  strcat(path, filename);
}

int get_file_type_ls(struct file_attribute *pattr)
{

  mode_t mode = pattr->f_attr_stat_info.st_mode;

  if(pattr == NULL)
    return -1;

  switch(mode & S_IFMT){ // 计算具体文件类型的bit
    case S_IFBLK :
      pattr->f_attr_type = 'b';
      break;

    case S_IFCHR :
      pattr->f_attr_type = 'c';
      break;

    case S_IFDIR :
      pattr->f_attr_type = 'd';
      break;

    case S_IFIFO :
      pattr->f_attr_type = 'p';
      break;

    case S_IFLNK :
      pattr->f_attr_type = 'l';
      break;

    case S_IFREG :
      pattr->f_attr_type = '-';
      break;

    case S_IFSOCK:
      pattr->f_attr_type = 's';
      break;

    default:
      break;
  }

  return 0;
}

int get_file_permission(struct file_attribute *pattr)
{
  int i,index = 0;
  char perm[] = {'r','w','x'};

  mode_t mode = pattr->f_attr_stat_info.st_mode;
#ifdef DEBUG
  printf("[ST_MODE]: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(mode));
#endif
  for(i = 8;i >= 0;i--)
  {
    if((mode >> i) & 0x1) // 循环移位操作
      pattr->f_attr_permission[index] = perm[index % 3];
    else
      pattr->f_attr_permission[index] = '-';

    index++;
  }

  pattr->f_attr_permission[index] = '\0';
  return 0;
}

void get_file_uname(struct file_attribute *pattr)
{
  struct passwd *pwd = getpwuid(pattr->f_attr_stat_info.st_uid);
  strcpy(pattr->f_attr_name, pwd->pw_name);
}
void get_file_gname(struct  file_attribute *pattr)
{
  struct group *grp = getgrgid(pattr->f_attr_stat_info.st_gid);
  strcpy(pattr->f_attr_name,grp->gr_name);
}
void get_file_last_modify_time(struct file_attribute *pattr)
{
  char *timestr = ctime(&pattr->f_attr_stat_info.st_mtime); // 将最后修改时间转换字符串
  strcpy(pattr->f_attr_mtime,timestr);
  pattr->f_attr_mtime[strlen(timestr) -1] = '\0'; // ctime函数包含'\n'

}
