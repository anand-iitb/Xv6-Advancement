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

void processtask(long number);

void processtask(long number)
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: sum <infile>\n");
        exit(EXIT_FAILURE);
    }
    char *fn = argv[1];
    // Read from file
    FILE *fin = fopen(fn, "r");
    long t;
    fscanf(fin, "%ld\n", &t);
    printf("The number of tasks are : %ld \n", t);
    
    char type;
    long num;
    while (fscanf(fin, "%c %ld\n", &type, &num) == 2)
    {
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
    }
    fclose(fin);
    // Print global variables
    printf("%ld %ld %ld %ld %ld\n", sum, odd, even, min, max);

    return (EXIT_SUCCESS);
}
