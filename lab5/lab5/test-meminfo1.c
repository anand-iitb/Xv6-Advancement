#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
  printf(1, "*Case1: invalid pid*\n");
  getmeminfo(-1);

  printf(1, "----------------------------------------------------\n");
  printf(1, "*Case2: pid = 0*\n");
  getmeminfo(0);  
  exit();
}
