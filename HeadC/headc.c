// Author: Rene Garcia Aviles 
// Head like C implementation 
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <string.h>
#include <ctype.h>

void Help(char **argv); 
void Head(int argc, char **argv); 

int main(int argc, char *argv[argc + 1])
{
	assert(argc > 1 && "Missing arguments, run with <-h> for more information"); 

	if (argc == 2 && !strcmp(argv[1],"-h"))
	{
		Help(argv); 
	}

	Head(argc, argv); 

	return EXIT_SUCCESS; 
}

void Help(char **argv)
{
	if (strlen(argv[1]) > 2 || argv[1][1] != 'h')
	{
		printf("Wrong flag \'%s\' used, run with <-h> for more information\n", argv[1]); 
		exit(EXIT_FAILURE); 
	}

	printf("Usage - %s <args> <file> \n", argv[0]); 
	printf("      -h               Show usage\n"); 
	printf("      -n <n lines>     Number of lines to show\n"); 

	exit(EXIT_SUCCESS); 
}

void Head(int argc, char **argv)
{
	FILE *pfile; 
	char ch; 
	
	int lines; 
	lines = 10; 

	int count; 
	count = 0; 
	
	char *file = (char*)calloc(1000,sizeof(char)); 

	if (argc > 2)
	{
		if (!strcmp(argv[1], "-n"))
		{
			// atoi returns 0 if no numbers are found
			if (!atoi(argv[2]))
			{
				printf("Wrong usage, after <-n> an number of lines should be given\n"); 
				free(file); 
				exit(1); 
			}

			lines = atoi(argv[2]); 
			strcpy(file, argv[3]); 
		}
		else
		{
			printf("Wrong flag \'%s\', run with <-h> for more info\n", argv[1]); 
			exit(1); 
		}
	}

	if (argc == 2)
		strcpy(file, argv[1]); 

	pfile = fopen(file, "r"); 
	if (!pfile)
	{
		printf("Could not open file: %s\n", file); 
		free(file); 
		exit(1); 
	}

	while (count < lines && (ch = fgetc(pfile)) != EOF)
	{
		if (ch == 10)
			count++; 
		putchar(ch); 
	}

	fclose(pfile); 
	free(file); 
}
