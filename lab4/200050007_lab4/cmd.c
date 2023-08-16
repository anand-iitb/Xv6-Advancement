// #include "syscall.h"
#include "types.h"
#include "stat.h"
#include "user.h"
int main(int argc, char *argv[])
{
    if(argc<=1)
    {
        printf(1,"usage: cmd ...\n");
        exit();
    }
    int pid;
    if((pid=fork())==0)
    {
        exec(argv[1],argv+1);
        printf(1,"exec error\n");
        exit();
    }
    else
    {
        wait();
        // waitpid(pid);
    }
    exit();
}