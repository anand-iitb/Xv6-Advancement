#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    // create three process and read x from 1st and pass using pipe and read y from 2nd and pass to pipe then add in 3rd
    int x, y;
    int pipefds[2];
    if (pipe(pipefds) == -1)
    {
        printf("pipe failed");
        exit(1);
    }
    int pid1 = fork();
    if (pid1 == 0)
    {
        printf("Process A: Input value of x : ");
        scanf("%d", &x);
        write(pipefds[1], &x, sizeof(x));
    }
    else
    {
        wait(pid1);
        int pid2 = fork();
        if (pid2 == 0)
        {
            int xx = 0;
            read(pipefds[0], &xx, sizeof(xx));
            printf("Process B: Input value of y : ");
            scanf("%d", &y);
            y += xx;
            write(pipefds[1], &y, sizeof(y));
        }
        else
        {
            wait(pid2);
            int pid3 = fork();
            if (pid3 == 0)
            {
                int sum = 0;
                read(pipefds[0], &sum, sizeof(sum));
                printf("Process C: Result after addition : %d\n", sum);
                exit(0);
            }
        }
    }
}
