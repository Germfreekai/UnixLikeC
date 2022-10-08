// Author: Rene Garcia Aviles
// Cp like C implementation 
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void* Help (int argc, char **argv); 
void* CopyFile (char **argv); 
void* CopyFilesToDir (int argc, char **argv); 
char *GetName(char *pfile); 

int main (int argc, char *argv[argc + 1])
{
	assert(argc > 1 && "Wrong usage, run with -h for more information"); 

	Help (argc, argv); 

	if (argc == 3)
	{
		CopyFile (argv); 
	}
	else if (argc > 3)
	{
		CopyFilesToDir (argc, argv); 
	}

	return EXIT_SUCCESS; 
}

void* Help (int argc, char **argv)
{
	char *flag = (char*)calloc(3, sizeof(char)); 
	
	if (argc == 2)
	{
		strcpy(flag, argv[1]); 

		if (flag[0] == '-')
		{
			if (flag[1] == 'h')
				printf("Usage - ./cpc <-h> <src+> <dst>\n     -h    Show usage\n    src    Files to copy +\n    dst    Destination file or directory\n"); 
			else
				goto err; 
		}
		else
		{
			goto err; 
		}
	}

	free (flag); 

	return NULL; 
err: 
	free(flag); 
	printf("Wrong usage, run with -h for more information\n"); 
	return NULL; 
}

void* CopyFile (char **argv)
{
	FILE *file;
	FILE *wfile; 
	char ch; 

	// Open file
	file = fopen (argv[1], "r"); 

	// Open file to write
	wfile = fopen (argv[2], "w"); 
	
	if (!file || !wfile)
		goto ferror; 

	ch = 32; 
	while ((ch = fgetc (file)) != EOF)
	{
		fprintf (wfile, "%c", ch); 
	}
	
	// Close files
	fclose (file); 
	fclose (wfile); 

	return NULL; 

ferror: 
	printf("Could not read file %s\n", argv[1]); 
	fclose (file); 
	fclose (wfile); 
	exit(1); 
}

void* CopyFilesToDir (int argc, char **argv)
{
	FILE *file; 
	char ch; 

	char *dir = (char*)calloc(1000, sizeof(char)); 
	strcpy(dir, argv[argc - 1]); 

	for (size_t i = 1; i < argc - 1; i++)
	{
		char *fname = GetName(argv[i]); 

		// argv simulation 
		char **false_argv = (char**)calloc(3, sizeof(char*)); 
		for (size_t a = 0; a <= 2; a++)
			false_argv[a] = (char*)calloc(1000,sizeof(char)); 

		char *fcfile = (char*)calloc(1000,sizeof(char)); 

		strcpy(fcfile, dir); 
		if (dir[strlen(dir)-1] != '/')
			strcat(fcfile, "/"); 
		strcat(fcfile, fname); 
		
		// Copy files
		strcpy(false_argv[1],argv[i]);
		strcpy(false_argv[2], fcfile); 

		CopyFile(false_argv); 

		for (size_t b = 0; b <= 2; b++)
			free(false_argv[b]);
		
		free(false_argv); 
		free(fname); 
		free(fcfile); 
	}

	free(dir); 
	return NULL; 
}

char *GetName(char *pfile)
{
	char *fname = (char*)calloc(1000,sizeof(char)); 

	int f_len = strlen(pfile); 

	int count; 
	count = 0; 

	for (int i = f_len - 1; i >= 0; i--)
	{
		if (pfile[i] != '/')
		{
			count++;
		}
		else
		{
			i = -1;
		}
	}

	if (f_len == count)
	{
		strcpy(fname, pfile);
		return fname; 
	}


	for (int i = f_len - count; i < f_len; i++)
	{
		if (i == f_len - count)
		{
			snprintf(fname, 2, "%c", pfile[i]); 
		}
		else
		{
			strncat(fname, &pfile[i], 1); 
		}
	}
	
	return fname; 

}
