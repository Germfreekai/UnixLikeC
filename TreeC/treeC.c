// Author: Rene Garcia Aviles
// Tree like C implementation
// Contact: renega1238@gmail.com
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{

	int spaces; 
	char* dir_n;

}Args; 

// Thread for navigate and count new Dir
void* foundNewDir(void* vargs); 
void Help(char *fname);

int main(int argc, char* argv[argc+1])
{

	assert(argc == 2 && "ERR: Wrong usage || Correct form: ./FILENAME <directory>");

	if (!strcmp(argv[1],"-h"))
		Help(argv[0]);

	// Initialize struct (args)
	Args* dirInf = (Args*)malloc(sizeof(Args));
	dirInf->spaces = 0;
	dirInf->dir_n = argv[1];

	// Thread vars
	pthread_t fnd_t; 
	int fnd; 

	// Create main thread
	fnd = pthread_create(&fnd_t, NULL, foundNewDir, (void*)dirInf);
	pthread_join(fnd_t, NULL); 

	// Free memory
	free(dirInf); 

	return EXIT_SUCCESS; 

}

void Help(char *fname)
{
	printf("Usage: %s <-h> <dir>\n    -h    Show help message\n", fname);
	exit(0);
}

// Compartimos memoria en el Struct
void* foundNewDir(void* vargs)
{

	Args* dirInf = (Args*)vargs; 

	// Struct for dirs
	DIR* dir; 
	struct dirent* drnt; 

	// New thread
	pthread_t ndir_t;
	int ndir;

	// For current dir
	char cwd[1000]; 
	char* newPath;

	printf("%*c", dirInf->spaces, 32);
	printf(">> %s \n", dirInf->dir_n); 

	dirInf->spaces = dirInf->spaces + 5;

	// Change to user's dir
	chdir(dirInf->dir_n); 

	// Get directory
	getcwd(cwd, sizeof(cwd)); 

	// Open directory
	dir = opendir(dirInf->dir_n); 
	if(dir == NULL)
	{

		printf("ERR: Invalid dir: %s\n", dirInf->dir_n);
		pthread_exit(0); 

	}


	// While over returned files / directories
	while((drnt = readdir(dir)) != NULL)
	{
		// strcmp returns 0 if equal
		// Skip father and current dierctory
		if(!strcmp(drnt->d_name, ".") || !strcmp(drnt->d_name, ".."))
		{

			continue;

		}
		// If directory
		// Create new thread
		else if(drnt->d_type == DT_DIR)
		{

			// New path
			newPath = cwd;
			strcat(newPath,"/");
			
			// Path to new path
			strcat(newPath, drnt->d_name);

			// Pass new directory
			dirInf->dir_n = newPath;

			// Create new thread for new directory
			ndir = pthread_create(&ndir_t, NULL, foundNewDir, (void*)dirInf);
			pthread_join(ndir_t, NULL);

			// Go up a level
			chdir(".."); 
			getcwd(cwd, sizeof(cwd));

			// Retrun to father directory
			dirInf->dir_n = cwd; 
			dirInf->spaces = dirInf->spaces - 5; 
			

		}
		// Print files
		else
		{

			printf("%*c", dirInf->spaces,32);
			printf("|\n");
			printf("%*c", dirInf->spaces, 32);
			printf("+ %s \n", drnt->d_name); 

		}

	}

	// Close
	closedir(dir); 

	// Exit thread
	pthread_exit(EXIT_SUCCESS); 

}

