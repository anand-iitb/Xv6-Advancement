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
        printf("Child : My process ID is:%d\nChild : The parent process ID is:%d\n", pid_ch1, pid_par);
        
        int sl=sleep(5);
    if(sl==0)   
    {
        pid_ch1 = getpid();
        pid_par = getppid();
        printf("\nChild: After sleeping for 5 seconds\nChild : My process ID is:%d\nChild : The parent process ID is:%d", pid_ch1, pid_par);
    }
    }
    else
    {
        pid_par = getpid();
        printf("Parent : My process ID is:%d\nParent : The child process ID is:%d\n", pid_par, p_pid_child); 
    }
    return 0;
}
