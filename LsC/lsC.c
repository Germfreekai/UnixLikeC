// Author: Rene Garcia Aviles
// Ls like C implementations
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <string.h>

typedef struct 
{
	char **argv;
	int argc;

}Args;

void GetDir(Args *args); 
void *PrintDir(void *vargs); 
void Help(char *fname);

int main(int argc, char *argv[argc + 1])
{
	if (!strcmp(argv[1], "-h"))
		Help(argv[0]);
	
	Args *args = (Args*)malloc(sizeof(Args)); 
	args->argv = argv;
	args->argc = argc;

	GetDir(args); 

	free(args); 

	return EXIT_SUCCESS;
}

void Help(char *fname)
{
	printf("Usage: \n%s <-h> <dir>\n    -h    Show help messagen\n", fname);
	exit(0);
}

void GetDir(Args *args)
{

	pthread_t nDir_t;
	int nDir; 

	// ls with no arguments, prints current dir
	if(args->argc == 1)
	{
		char **cwd = (char**)calloc(1,sizeof(char*));
		cwd[0] = (char*)calloc(2,sizeof(char*));
		strcpy(cwd[0],".");

		args->argv = cwd;

		nDir = pthread_create(&nDir_t, NULL, PrintDir, (void*)args->argv[0]);
		pthread_join(nDir_t, NULL); 

		putchar(10); 
		
		free(cwd[0]); 
		free(cwd); 

	}
	else
	{
		for (size_t i = 1; i < args->argc; i++)
		{
			nDir = pthread_create(&nDir_t, NULL, PrintDir, (void*)args->argv[i]); 
			pthread_join(nDir_t, NULL); 
			if (i != args->argc - 1)
				putchar(10); 
		}

		putchar(10); 
	}

}

void *PrintDir(void *vargs)
{
	char *dir_name = (char*)vargs; 

	DIR *dir; 
	struct dirent *drnt;

	dir = opendir(dir_name); 
	
	if (!dir)
	{
		printf("ERROR Invalid dir: %s\n", dir_name); 
		pthread_exit(0); 

	}
	
	printf("--> %s \n", dir_name); 
	while ((drnt = readdir(dir)))
	{
		printf("%s\t", drnt->d_name); 
	}

	closedir(dir);

	pthread_exit(EXIT_SUCCESS); 
}
