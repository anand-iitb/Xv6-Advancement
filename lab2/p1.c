#include <stdio.h>
#include <unistd.h>
int main()
{
    int pid_par = 0, pid_ch1 = 0;
    int p_pid_child = fork();
    if (p_pid_child == 0)
    {
        pid_ch1 = getpid();
        pid_par = getppid();
        printf("Child : My process ID is:%d\nChild : The parent process ID is:%d\n", pid_ch1, pid_par); // printf("CHild\n");
    }
    else
    {
        pid_par = getpid();
        printf("Parent : My process ID is:%d\nParent : The child process ID is:%d\n", pid_par, p_pid_child); // printf("Parent\n");
    }
    return 0;
}