// Author: Rene Garcia Aviles
// Cd like C implementation 
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <unistd.h> 

int main(int argc, char *argv[argc + 1])
{
	assert(argc == 2 && "Wrong usage - ./cdc <dir>"); 

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


