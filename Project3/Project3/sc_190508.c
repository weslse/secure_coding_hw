#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAXLINE 2000

int main()
{
	char* txtLine = (char*)malloc(sizeof(char) * NUM_MAXLINE);
	FILE* fd = fopen("./endgame.txt", "rt");
	int wordCnt = 0;

	if (fd == NULL)
	{
		printf("file open error");
		return -1;
	}
	
	while (fgets(txtLine, NUM_MAXLINE, fd) != NULL)
	{
		char* token = strtok(txtLine, " ,:;?");
		while (token != NULL)
		{
			printf("%s\n", token);
			token = strtok(NULL, " ,:;?");
			wordCnt++;
			
		}
	}

	printf("Total word count : %d\n", wordCnt);
	fclose(fd);

	return 0;
}
