#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

// TODO: YOUR CODE HERE
// INITIALIZE DATA STRUCTURE TO STORE PIDS OF PROCESSES TO KILL LATER

int pid[64];
int init = 0;
/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line)
{

	// tokenizer variables
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	// loop on length of line
	for (i = 0; i < strlen(line); i++)
	{
		char readChar = line[i];
		// tokenize on any kind of space
		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else
		{
			token[tokenIndex++] = readChar;
		}
	}

	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}

void exec(char **tokens, int seq)
{
	if (tokens == NULL)
	{
		return;
	}
	int child = 1000;
	if (strcmp(tokens[0], "cd") == 0)
	{
		if (chdir(tokens[1]) != 0)
		{
			printf("Error: Invalid directory\n");
		}
	}
	else if ((child = fork()) == 0)
	{
		if (execvp(tokens[0], tokens) == -1)
		{
			printf("Error: Invalid command\n");
		}
		exit(0);
	}
	else
	{
		pid[init++] = child;
		if (seq)
			wait(NULL);
	}
}

// TODO
// MAKE FUNCTIONS AS REQUIRED TO AVOID REPETITIVE WORK

int main(int argc, char *argv[])
{

	char line[MAX_INPUT_SIZE];
	char **tokens;
	int *pids = (int *)malloc(MAX_NUM_TOKENS * sizeof(int));
	// TODO: YOUR CODE HERE
	// INITIALIZE GLOBAL DATA STRUCTURE OF PIDS TO SOME DEFAULT VALUE
	// INITIALIZE OTHER VARIABLES AS NEEDED
	for (int i = 0; i < 64; i++)
		pid[i] = 10000;

	while (1)
	{

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; // terminate with new line
		tokens = tokenize(line);

		// You can maintain boolean variables to check which type of command is given by user and then
		// conditionally execute depending on them

		int child = 0;
		int seq = 1;
		// for (int i = 0; tokens[i] != NULL; i++)
		// {
		// 	if (strcmp(tokens[i], "&&") == 0)
		// 		seq = 1;
		// }
		for (int i = 0; tokens[i] != NULL; i++)
		{
			if (strcmp(tokens[i], "&&&") == 0)
				seq = 0;
		}
		if (strcmp(tokens[0], "exit") == 0)
		{
			exit(0);
		}
		else if (seq == 1)
		{
			char **token = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
			int pnt = 0;
			for (int i = 0; tokens[i] != NULL; i++)
			{
				if (strcmp(tokens[i], "&&") == 0)
				{
					token[pnt] = NULL;
					pnt = 0;
					exec(token, 1);
					free(token);
					token = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
				}
				else
					token[pnt++] = tokens[i];
			}
			token[pnt] = NULL;
			exec(token, 1);
			free(token);
		}
		else if (seq == 0)
		{
			char **token = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
			int pnt = 0;
			for (int i = 0; tokens[i] != NULL; i++)
			{
				if (strcmp(tokens[i], "&&&") == 0)
				{
					token[pnt] = NULL;
					pnt = 0;
					exec(token, 0);
					free(token);
					token = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
				}
				else
					token[pnt++] = tokens[i];
			}
			token[pnt] = NULL;
			exec(token, 0);
			free(token);
		}

		// TODO: YOUR CODE HERE
		//
		// TODO: YOUR CODE HERE

		// freeing the memory
		for (int i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}

		free(tokens);
	}
	return 0;
}
