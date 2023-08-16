#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	// 	Parent is : 32409
	// Number of children: 10
	// Child 32410 is created
	// Child 32411 is created
	// Child 32412 is created
	// Child 32413 is created
	// Child 32414 is created
	// Child 32415 is created
	// Child 32416 is created
	// Child 32418 is created
	// Child 32419 is created
	// Child 32417 is created
	// Child 32410 of parent 32409 exited
	// Child 32411 of parent 32409 exited
	// Child 32412 of parent 32409 exited
	// Child 32413 of parent 32409 exited
	// Child 32414 of parent 32409 exited
	// Child 32415 of parent 32409 exited
	// Child 32416 of parent 32409 exited
	// Child 32418 of parent 32409 exited
	// Child 32419 of parent 32409 exited
	// Child 32417 of parent 32409 exited
	// Parent exited
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
// printf("Parent is: %d\n", getpid());
// printf("Number of children: %d\n", n);
// int ff = fork();
// if (ff == 0)
// {
// 	printf("Child %d is created\n", getpid());
// 	int sl = sleep(1);
// 	if (sl == 0)
// 		exit(0);
// }
// else
// {
// 	wait(ff);
// 	printf("Child %d of parent %d exited\n", ff, getpid());
// 	printf("Parent exited\n");
// }