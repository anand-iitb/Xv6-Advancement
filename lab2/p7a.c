#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void child(int n)
{
	if(n==0) return;
	int ff=fork();
	if(ff==0)
	{
		printf("Child %d is created\n",getpid());
		child(n-1);
	}
	else
	{
		wait(ff);
		printf("Child %d of parent %d exited\n",ff,getpid());
	}
}
int main(int argc,char *argv[])
{
	int n=atoi(argv[1]);
	printf("Parent is: %d\n",getpid());
	printf("Number of children: %d\n",n);		
	int ff=fork();
	if(ff==0)
	{
		printf("Child %d is created\n",getpid());
		child(n-1);
	}
	else
	{
		wait(ff);
		printf("Child %d of parent %d exited\n",ff,getpid());
		printf("Parent exited\n");
	}
	return 0;
}
