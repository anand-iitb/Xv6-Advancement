#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	while(1)
	{
		char inp[50];
		printf(">>> ");
		scanf("%s",&inp);
		//printf("%s",inp);
		int ff=fork();
		if(ff==0)
		{
			execl(inp,argv);
			printf("C\n");
		}
		else
		{
		wait(ff);
		}
	}
	return 0;
}
