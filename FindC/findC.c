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
Args *GetFlags(int argc, char **argv); 
void PrintError(void);
void FreeStruct(Args *args);
void* FindFile(void *vargs); 
char *FileName(char *fname); 
int CmpNames(char *gfile, char *cfile);
char *FileExt(char *fname);

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

Args *GetFlags(int argc, char **argv)
{
	Args* args = (Args*)malloc(sizeof(Args)); 
	args->file_n = (char*)calloc(1000,sizeof(char));
	args->dir_n = (char*)calloc(1000,sizeof(char)); 
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
	
	DIR *dir;
	struct dirent *drnt;

	// New thread
	pthread_t ndir_t;
	int ndir; 

	// For current dir
	char cwd[1000]; 

	// Go to given directory 
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

	// While reading, return files
	while ((drnt = readdir(dir)))
	{
		// strcmp returns 0 if equal
		// Skip father and current directory
		if (!strcmp(drnt->d_name, ".") || !strcmp(drnt->d_name, ".."))
		{
			continue;
		}
		// If a directroy, create new thread
		else if (drnt->d_type == DT_DIR)
		{
			char *newPath = (char*)calloc(1000,sizeof(char)); 
			
			strcpy(newPath, cwd); 
			strcat(newPath, "/");

			// Path to new dir
			strcat(newPath, drnt->d_name); 

			// Pass new directory 
			strcpy(args->dir_n, newPath); 

			// Create new thread
			ndir = pthread_create (&ndir_t, NULL, FindFile, (void*)args); 
			pthread_join (ndir_t, NULL); 

			// When returned, we need to go up a level 
			chdir (".."); 
			getcwd(cwd, sizeof(cwd)); 

			// Return to father directory 
			strcpy(args->dir_n, cwd); 

			free(newPath); 
		}
		else
		{
			char *name_wext = FileName(drnt->d_name); 

			char *ext = FileExt(drnt->d_name); 

			int o; 
			o = 0; 
			int a; 
			a = 0;

			if (ext)
			{
				if(!CmpNames(args->file_n, ext))
					a = 1; 
			}
			
			if (name_wext)
			{
				if (!CmpNames(args->file_n, name_wext) || !CmpNames(args->file_n, drnt->d_name))
					o = 1;
			}
			else
			{
				if (!CmpNames(args->file_n, drnt->d_name))
					o = 1; 
			}

			if (o || a)
			{
				char *fullPathName = (char*)calloc(1000,sizeof(char)); 
				
				strcpy(fullPathName, cwd);
				strcat(fullPathName, "/"); 
				strcat(fullPathName, drnt->d_name);
				
				printf("> %s\n", fullPathName); 

				free(fullPathName); 
			}
			
			free(ext); 
			free(name_wext); 
		}
	}

	// Close dir
	closedir(dir); 

	pthread_exit(EXIT_SUCCESS); 
}

char *FileExt(char *fname)
{
	char *ext = (char*)calloc(20,sizeof(char)); 

	int flen = strlen(fname); 

	int elen; 
	elen = 0; 

	for (int i = flen - 1; i >= 0; i--)
	{
		if (fname[i] != '.')
		{
			elen++;
		}
		else
		{
			elen++;
			i = -1; 
		}
	}

	// If no extension or starts with . 
	if (elen == flen)
	{
		free(ext); 
		return NULL;
	}
	
	for (int i = flen - elen; i < flen; i++)
	{
		if ( i == flen - elen)
		{
			snprintf(ext, 2, "%c", fname[i]); 
		}
		else
		{
			strncat(ext, &fname[i], 1); 
		}
	}

	return ext; 

}

char *FileName(char *fname)
{
	char *name = (char*)calloc(1000,sizeof(char)); 

	int flen = strlen(fname); 

	// Extension len 
	int elen;
	elen = 0; 

	// Get extension size
	for (int i = flen - 1; i >= 0; i--)
	{
		if (fname[i] != '.')
		{
			elen++; 
		}
		else
		{
			elen++;
			i = -1;
		}
	}

	// If no extension or starts with . 
	if (elen == flen)
	{
		free(name); 
		return NULL;
	}

	// Remove extension
	for (int i = 0; i < flen - elen; i++)
	{
		if (!i)
		{
			snprintf(name, 2, "%c", fname[i]);
		}
		else
		{
			strncat(name, &fname[i], 1); 
		}
	}

	return name; 
}

// gfile = given file name
// cfile = calculated file name
int CmpNames(char *gfile, char *cfile)
{
	if (strlen(gfile) != strlen(cfile))
		return 1; 

	while (*gfile)
	{
		if (gfile[0] != cfile[0])
			return 1;

		gfile++;
		cfile++; 
	}

	return 0;
}
