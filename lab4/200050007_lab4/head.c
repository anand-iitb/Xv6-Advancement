#include "types.h"
#include "stat.h"
#include "user.h"
char buf;

void cat(int fd, int lines)
{
    int n;
    if (lines == 0)
        return;
    while ((n = read(fd, &buf, sizeof(buf))) > 0)
    {
        if (lines == 0)
            break;
        if (write(1, &buf, n) != n)
        {
            printf(1, "cat: write error\n");
            exit();
        }
        if (buf == '\n')
            lines--;
    }
    if (n < 0)
    {
        printf(1, "cat: read error\n");
        exit();
    }
}

int main(int argc, char *argv[])
{
    int fd, i;

    if (argc < 2)
    {
        printf(1, "usage: cat file ...\n");
        exit();
    }
    int n = atoi(argv[1]);
    if (argc == 2)
    {
        cat(0, n);
    }
    for (i = 2; i < argc; i++)
    {
        if ((fd = open(argv[i], 0)) < 0)
        {
            printf(1, "cat: cannot open %s\n", argv[i]);
            exit();
        }
        printf(1, "------------%s------------\n", argv[i]);
        cat(fd, n);
        close(fd);
    }
    exit();
}
