// Author: Rene Garcia Aviles 
// Quick regex example for regular expressions
// Contact: renega1238@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int main(int argc, char *argv[argc + 1])
{
	// Create regex variable
	regex_t reegex; 

	int val; 

	char *word = argv[1]; 
	printf("%s\n", word); 

	if (word == argv[1])
	{
		printf("EQUAL\n"); 
	}
	// Create regEx
	// Against what we are comparing
	val = regcomp(&reegex, "/mnt/data/germfreekai", REG_EXTENDED); 
	if(val)
	{
		printf("Failed to compile regex\n"); 
		exit(1); 
	}

	// Compare 
	val = regexec(&reegex, "/mnt/data/germfreekai", 0, NULL, 0); 

	if (val == 0)
	{
		printf("Pattern found\n"); 
	}
	else if (val == REG_NOMATCH)
	{
		printf("Pattern NOT found\n"); 
	}
	else
	{
		printf("ERROR: Smth happened\n"); 
	}

	return EXIT_SUCCESS; 
}
