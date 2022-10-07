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
	exit(1); 
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
	exit(1); 
}

void* CopyFilesToDir (int argc, char **argv)
{
	FILE *file; 
	char ch; 

	return NULL; 
}

char *GetName(char *pfile); 
