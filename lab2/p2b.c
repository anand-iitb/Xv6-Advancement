#include <stdio.h>
#include <unistd.h>
int main()
{
    int n;
    scanf("%d", &n);
    int ff = fork();
    if (ff == 0)
    {
        int pid = getpid();
        for (int i = 1; i < n + 1; i++)
        {
            printf("C %d %d\n", pid, i);
        }
    }
    else
    {
        wait(ff);
        int pid = getpid();
        for (int i = n + 1; i < 2 * n + 1; i++)
        {
            printf("P %d %d\n", pid, i);
        }
    }
    return 0;
}