#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int account_balance = 0;
pthread_mutex_t lock;

void *increment(void *value)
{
    // int valu = *((int *)value);

    for (int j = 0; j < (long)value; j++)
    {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < 1000000; i++)
        {
            account_balance++;
        }
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char *argv[])
{
    clock_t start = clock();
    if (argc < 2)
    {
        printf("Usage: %s <number of threads>", argv[0]);
        return 0;
    }
    int threadNum = atoi(argv[1]);
    pthread_t th[threadNum];
    pthread_mutex_init(&lock, NULL);
    int i;
    long value_per_thread = (2048 / threadNum);
    for (i = 0; i < threadNum; i++)
    {
        if (pthread_create(th + i, NULL, &increment, (void *)(value_per_thread)) != 0)
        {
            perror("Failed to create thread");
            return 1;
        }
        printf("Transaction %d has started\n", i);
    }
    for (i = 0; i < threadNum; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            return 2;
        }
        printf("Transaction %d has finished\n", i);
    }
    printf("Account Balance is : %d\n", account_balance);
    printf("Time spent: %f ms\n", 1000 * ((double)(clock() - start) / CLOCKS_PER_SEC));
    return 0;
}
