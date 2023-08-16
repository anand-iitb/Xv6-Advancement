#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  printf(1, "*Case3: specified valid pid(pid > 0)[getpid() > 0]*\n");
  printf(1, "----------------------------------------------------\n");
  printf(1, "Memory information before sbrk system call\n");
  int pid = getpid();
  getmeminfo(pid);
  sbrk(4096 * 2048);
  printf(1, "----------------------------------------------------\n");
  printf(1, "Memory information after sbrk system call\n");
  getmeminfo(pid);  
  exit();
}
