#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_freememstat(void)
{
  return freememstat();
}
int sys_helloYou(void)
{
  char *name;
  argstr(0, &name);
  helloYou(name);
  return 0;
}
int sys_getNumProc(void)
{
  return getNumProc();
}

int sys_getMaxPid(void)
{
  return getMaxPid();
}
int sys_getmeminfo(void)
{
  int pid;
  if (argint(0, &pid) < 0)
    return -1;
  return getmeminfo(pid);
}
int sys_mmap(void)
{
  int addr;
  int n;
  if (argint(0, &n) < 0)
    return 0;
  if (grow_mmap(n) < 0)
    return 0;
  addr = myproc()->sz - n;
  return addr;
}
int sys_wait2(void)
{
  int *wtime;
  int *rtime;
  argptr(0, (char **)&wtime, sizeof(int));
  argptr(1, (char **)&rtime, sizeof(int));
  return wait2(wtime, rtime);
}

int sys_clone(void)
{
  void (*fn)(int *);
  int *arg;
  void *stack;
  argptr(0, (char **)&fn, sizeof(void *));
  argptr(1, (char **)&arg, sizeof(int *));
  argptr(2, (char **)&stack, sizeof(void *));
  return clone(fn, arg, stack);
}
int sys_join(void)
{
  return join();
}