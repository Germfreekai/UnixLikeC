#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	FILE *fptr; 
	char ch; 

	fptr = fopen("README.md", "r"); 
	
	if (!fptr)
	{
		printf("Failed\n"); 
		exit(1); 
	}

	while ((ch = fgetc(fptr)) != EOF)
	{
		printf(" > Char %c == %d",ch,ch); 
	}

	return EXIT_SUCCESS; 
}
