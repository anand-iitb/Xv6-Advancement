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
        	printf("\nChild : Check Child process state\n");
    		printf("\nChild : Press any key to continue\n");
    		getchar();
    		printf("Child : Check Child process state again\n");
        
    }
    else
    {
        pid_par = getpid();
        printf("Parent : My process ID is:%d\nParent : The child process ID is:%d\n", pid_par, p_pid_child); 
        int sl=sleep(60);
        if(sl==0) wait(p_pid_child);
    }
    return 0;
}
