#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global variables
long sum = 0;
long odd = 0;
long even = 0;
long min = INT_MAX;
long max = INT_MIN;
bool done = false;

pthread_mutex_t lock;

struct ds
{
    char type;
    long num;
};

void processtask(int number)
{
    // simulate burst time
    sleep(number);

    // update global variables
    sum += number;
    if (number % 2 == 1)
    {
        odd++;
    }
    else
    {
        even++;
    }
    if (number < min)
    {
        min = number;
    }
    if (number > max)
    {
        max = number;
    }
}
void *temp(void *typ)
{
    pthread_mutex_lock(&lock);
    // typ has both type and number
    struct ds *temp = (struct ds *)typ;
    char type = temp->type;
    long num = temp->num;
    if (type == 'p')
    { // processing task
        processtask(num);
        printf("Task completed\n");
    }
    else if (type == 'w')
    { // waiting period
        sleep(num);
        printf("Wait Over\n");
    }
    else
    {
        printf("ERROR: Type Unrecognizable: '%c'\n", type);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_unlock(&lock);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: exe <infile> <number of threads>");
        exit(EXIT_FAILURE);
    }
    // init mutex
    pthread_mutex_init(&lock, NULL);
    char *fn = argv[1];
    int threadNum = atoi(argv[2]);
    int thread_busy[threadNum];
    for (int i = 0; i < threadNum; i++)
    {
        thread_busy[i] = 0;
    }
    pthread_t threads[threadNum];
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    char type;
    long num;
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
        struct ds *typ = malloc(sizeof(struct ds));
        typ->type = type;
        typ->num = num;
        int fnd = 0;
        while (fnd == 0)
        {
            for (int i = 0; i < threadNum; i++)
            {
                if (thread_busy[i] == 0)
                {
                    thread_busy[i] = 1;
                    pthread_create(&threads[i], NULL, &temp, (void *)typ);
                    fnd = 1;
                    break;
                }
            }
        }
        
    }

    fclose(fin);
    // Print global variables
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}
