#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int n = atoi(argv[1]);
	int pid = getpid();
	printf("Parent is : %d\n", pid);
	printf("Number of children: %d\n", n);
	int pid_[n], gpid_[n];
	for (int i = 0; i < n; i++)
	{
		int pid = fork();
		if (pid == 0)
		{
			printf("Child %d is created\n", getpid());
			int sl = sleep(1);
			if (sl == 0)
			{
				exit(0);
			}
		}
		else
		{
			wait(pid);
			// printf("Child %d of parent %d exited\n", pid, getpid());
			pid_[i] = pid;
			gpid_[i] = getpid();
		}
	}
	for (int i = 0; i < n; i++)
	{
		printf("Child %d of parent %d exited\n", pid_[i], gpid_[i]);
	}
	printf("Parent exited\n");

	return 0;
}