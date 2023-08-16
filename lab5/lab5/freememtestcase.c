#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  if (argc < 2){
		printf(2, "Usage: freememtestcase size(in bytes)\n");
		exit();
	}

	int size = atoi(argv[1]);
	if (size >= 0){
		sbrk(size);
	} else {
		printf(2, "Invalid size %s\n", argv[1]);
	}
  int avail_mem = freememstat();
  printf(1, "Available memory: %d\n", avail_mem);
  exit();
}
