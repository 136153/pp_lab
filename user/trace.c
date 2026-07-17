#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int  mask;

  if(argc < 3){
    fprintf(2, "usage: trace mask command...\n");
    exit(1);
  }
  mask = atoi(argv[1]);    //atoi : char ->  int
  
  // 调用你写的 trace 系统调用，设置掩码
  trace(mask);

  // 执行后面的命令，比如 grep hello README
  exec(argv[2], &argv[2]);
  
  // 如果 exec 失败才会执行到这里
  fprintf(2, "exec %s failed\n", argv[2]);
  exit(1);
}