#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char *argv[])
{
	char file1[100], file2[100];
	if (argc != 3)
	{
		printf("Usage:./a.out file1 file2");
		exit(1);
	}
	strcpy(file1, argv[1]);
	strcpy(file2, argv[2]);
	int fd1 = open(file1, O_RDONLY);
	int fd2 = open(file2, O_RDWR|O_CREAT, 0666);
	char buf[1024];
	int n;
	while ((n = read(fd1, buf, 1024)) > 0)
	{
		write(fd2, buf, n);
	}
	close(fd1);
	close(fd2);
	return 0;
}
