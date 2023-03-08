#include "cmd_ls.h"
#include <dirent.h>
int cmd_ls_execute(cmd_t *pcmd)
{
  int ret;
  if(pcmd ==NULL) return -1;
  if(pcmd->cmd_arg_count !=2) {
    fprintf(stderr,"Command argument Error. \n");
    return -1;
  }
 if(pcmd->cmd_arg_list[1] != NULL)  return cmd_list_directory(pcmd->cmd_arg_list[1]);
  return 0;
}

int cmd_list_directory(const char *dirpath)
{
  DIR *pdir = NULL;
  struct dirent *pdirent = NULL;


  char path[128] = {0};

  pdir = opendir(dirpath);
  if(pdir == NULL){
    perror("open(): ");
    return -1;
  }

  while((pdirent = readdir(pdir)) != NULL){
    if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0)
      continue;
    // ..........
  }

  closedir(pdir);

}
