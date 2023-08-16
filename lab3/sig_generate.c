#include<stdio.h>
#include<stdlib.h>
#include<signal.h>tw
int main(int argc,char *argv[])
{
	int PID=atoi(argv[1]),pid=PID;
	kill(PID,SIGINT);
	printf("SIGINT signal sent to PID: %d\n",pid);
	kill(PID,SIGTERM);
	printf("SIGTERM signal sent to PID: %d\n",pid);
	kill(PID,SIGKILL);
	printf("SIGKILL signal sent to PID: %d\n",pid);
}
