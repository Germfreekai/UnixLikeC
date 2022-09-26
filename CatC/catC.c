// Author: Rene Garcia Aviles
// Cat like C implementation 
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void Flag(char *opt); 
void Help(); 
void PrintFiles(char **argv, int argc);

int main(int argc, char *argv[argc + 1])
{
	assert(argc > 1 && "Missing arguments, run with -h for more info"); 

	if(strlen(argv[1]) < 3)
	{
		char *opt = (char*)calloc(3,sizeof(char));
		strcpy(opt, argv[1]); 
		
		Flag(opt); 
		
		free(opt);
		
		exit(0);
	}

	PrintFiles(argv, argc); 

	return EXIT_SUCCESS;
}

void Flag(char *opt)
{
	if(opt[0] == '-')
	{
		if(opt[1] == 'h')
		{
			Help();
		}
		else
		{
			printf("Wrong flag, run with -h\n"); 
			free(opt); 
			exit(1); 
		}
	}
}

void Help()
{
	printf("Usage:\n./executable < args >\n"); 
	printf("    -h    Show help\n"); 
}

void PrintFiles(char **argv, int argc)
{
	FILE *ptr;
	char ch;

	for(size_t i = 1; i < argc; i++)
	{
		char *file = (char*)calloc(100,sizeof(char));

		strcpy(file, argv[i]); 

		ptr = fopen(file, "r");

		if(ptr == NULL)
		{
			printf("[%zu] File < %s > could not be opened\n", i, file);
			
			free(file); 

			exit(2); 
		}

		printf("[%zu] File > %s \n", i, file); 
		
		ch = 32; 
		while(ch != EOF)
		{
			ch = fgetc(ptr);
			putchar(ch); 
		}

		ch = 10; 
		putchar(ch); 
		putchar(ch); 

		fclose(ptr);
		free(file); 
	}
}
