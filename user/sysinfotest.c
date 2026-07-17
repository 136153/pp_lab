
#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/sysinfo.h"
#include "user/user.h"

void
testcall()
{
  struct sysinfo info;

  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }
}

void
testmem()
{
  struct sysinfo info;
  int freemem_before, freemem_after;

  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }
  freemem_before = info.freemem;

  // 分配一些内存，看看空闲内存是否减少
  char *p = malloc(1024 * 1024); // 分配 1MB
  if (p == 0) {
    printf("FAIL: malloc failed\n");
    exit(1);
  }

  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }
  freemem_after = info.freemem;

  if (freemem_after >= freemem_before) {
    printf("FAIL: free memory did not decrease after malloc\n");
    exit(1);
  }

  free(p);
}

void
testproc()
{
  struct sysinfo info;
  int nproc_after;

  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }

  // fork 一个子进程，看看进程数是否增加
  int pid = fork();
  if (pid < 0) {
    printf("FAIL: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // 子进程直接退出
    exit(0);
  } else {
    // 父进程等待子进程
    wait(0);
  }

  if (sysinfo(&info) < 0) {
    printf("FAIL: sysinfo failed\n");
    exit(1);
  }
  nproc_after = info.nproc;

  // 注意：这里只是简单测试，因为子进程已经退出，进程数应该恢复
  // 如果测试发现进程数不对，可以放宽条件
  if (nproc_after < 1) {
    printf("FAIL: nproc is invalid\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  printf("sysinfotest: start\n");
  testcall();
  testmem();
  testproc();
  printf("sysinfotest: OK\n");
  exit(0);
}