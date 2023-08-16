#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    int x = 2;
    int pid = fork();
    if (pid == -1) {
        return 1;
    }
    
    if (pid == 0) {
        x+=5;
    }
    sleep(2);
    
    if(pid){
        printf("PID of Process: %d\n", getpid());
    }
    else{
        printf("PID of Process: %d\n", getpid());
    }

    printf("Value of x: %d\n", x);
    if (pid != 0) {
        wait(NULL);
    }
    return 0;
}