#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  uint ret;
  int pid = getpid();
  
  printf(1, "Initial memory information\n");
  getmeminfo(pid);
  printf(1, "--------------------Partial Testcase (implemented only step1)--------------------------------\n");

  ret = mmap (-1234);
  if(ret == 0)
    printf(1, "mmap failed for wrong inputs(i.e. %d)\n", -1234);
  else
    exit();
  
  ret = mmap (1234);
  if(ret == 0)
    printf(1, "mmap failed for wrong inputs(i.e. %d)\n", 1234);
  else
    exit();

  printf(1, "----------------------------------------------------\n");
  ret = mmap(4096);
  
  if(ret == 0 )
    printf(1, "mmap failed\n");
  else {
    printf(1, "After mmap one page\n");
    getmeminfo(pid);

    char *addr = (char *) ret;

    addr[0] = 'a';

    printf(1, "After access of one page\n");
    getmeminfo(pid);
  }
  exit();
}
