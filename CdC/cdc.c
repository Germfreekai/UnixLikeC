// Author: Rene Garcia Aviles
// Cd like C implementation 
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <unistd.h> 
#include <string.h>

void Help(char *fname);

int main(int argc, char *argv[argc + 1])
{
	assert(argc == 2 && "Wrong usage - ./cdc <dir>"); 

	if (!strcmp(argv[1],"-h"))
	{
		Help(argv[0]);
		exit(0);
	}
	char cwd[100]; 

	getcwd(cwd, sizeof(cwd)); 
	printf("Current dir -> %s\n", cwd); 

	if (chdir(argv[1]))
		goto err; 

	getcwd(cwd, sizeof(cwd)); 
	printf("Current dir -> %s\n", cwd); 
	
	return EXIT_SUCCESS; 

err: 
	printf("Directory < %s > does not exist\n", argv[1]); 
	return EXIT_FAILURE; 
}

void Help(char *fname)
{
	printf("Usage:\n%s <dir>\n     -h    Show help message\n", fname);
}
