#include <stdio.h>
#include <signal.h>

int main()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	printf("Process Id is: %d\n", getpid());
	while (1)
	{
		printf("Waiting...\n");
		sleep(3);
	}
}
