// Author: Rene Garcia Aviles
// Find like C implementation
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <regex.h>
#include <unistd.h>

typedef struct 
{
	char *file_n;
	char *dir_n; 
}Args;

void Help(void); 
Args* GetFlags(int argc, char **argv); 
void PrintError(void);
void FreeStruct(Args *args);
void* FindFile(void *vargs); 

int main(int argc, char *argv[argc + 1])
{
	assert((argc == 2 || argc == 5) && "Wrong usage - Invalid flag or argument, run with <-h> for more information"); 

	Args *args = GetFlags(argc, argv); 

	pthread_t ffile_t;
	int ffile;

	// Main thread
	ffile = pthread_create(&ffile_t, NULL, FindFile, (void*)args);
	pthread_join(ffile_t, NULL); 
	
	FreeStruct(args); 
	
	return EXIT_SUCCESS; 
}

void FreeStruct(Args *args)
{
	free(args->file_n);
	free(args->dir_n);
	free(args);
}

void PrintError(void)
{
	printf("Wrong usage - Invalid flag or argument, run with <-h> for more information\n"); 
}

Args* GetFlags(int argc, char **argv)
{
	Args* args = (Args*)malloc(sizeof(Args)); 
	args->file_n = (char*)calloc(100,sizeof(char));
	args->dir_n = (char*)calloc(100,sizeof(char)); 
	char *flag = (char*)calloc(3,sizeof(char)); 
	
	if (strlen(argv[1]) > 2)
	{
		goto err;
	}
	

	for (size_t i = 1; i < argc; i++)
	{
		strcpy(flag, argv[i]); 
		
		if (flag[0] == '-')
		{
			if (flag[1] ==  'h')
			{
				Help();
				FreeStruct(args);
				free(flag); 
				exit(2); 
			}
			else if (flag[1] == 'f')
			{
				if (argv[i+1] == NULL)
					goto err;

				strcpy(args->file_n, argv[i+1]);
				i++;
			}
			else if (flag[1] == 'd')
			{
				if (argv[i+1] == NULL)
					goto err;

				strcpy(args->dir_n, argv[i+1]);
				i++;
			}
			else
			{
				goto err;
			}
		}
		else
		{
			goto err;
		}
	}

	
	free(flag); 

	if(args)
		return args; 
	
	return NULL; 

err: 
	PrintError();
	FreeStruct(args);
	free(flag); 
	exit(2); 
}

void Help(void)
{
	printf("Usage: \n"); 
	printf("./FILENME <-h> <-f:filename> <-d:dirname>\n"); 
	printf("     -h           Usage menu\n"); 
	printf("     -f filename  String to look for\n"); 
	printf("     -d dirname   Directory in which we will look for\n"); 
}

void* FindFile(void *vargs)
{
	Args* args = (Args*)vargs; 
	
	printf("file: %s\n", args->file_n); 
	printf("dir:  %s\n", args->dir_n); 

	DIR *dir;
	struct dirent *drnt;

	// New thread
	pthread_t ndir_t;
	int ndir; 

	// For current dir
	char cwd[1000]; 
	char *newPath = (char*)calloc(1000,sizeof(char)); 

	chdir(args->dir_n); 

	if (!getcwd(cwd, sizeof(cwd)))
	{
		printf("ERROR: Retrieving working directory\n"); 
		pthread_exit(0); 
	}

	// Open working directory
	// If dir == NULL
	dir = opendir(cwd);
	if (!dir)
	{
		printf("ERROR: Invalid dir: %s\n", cwd); 
		pthread_exit(0); 
	}

	printf("cwd : %s \n", cwd); 

	// While reading, return files
	while ((drnt = readdir(dir)))
	{
		// strcmp returns 0 if equal
		// Skip father and current directory
		if (!strcmp(drnt->d_name, ".") || !strcmp(drnt->d_name, ".."))
		{
			continue;
		}
		else if (drnt->d_type == DT_DIR)
		{
			printf("-> %s - is a directory\n", drnt->d_name); 
		}
		else
		{
			printf("--> %s - is a file\n", drnt->d_name); 
		}
	}

	// Close dir
	closedir(dir); 

	// Free memory 
	free(newPath); 

	pthread_exit(EXIT_SUCCESS); 
}
