/* 
 * Author: Rene Garcia Aviles
 * Touch C like implementation
 * Contact: renega1238@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[argc + 1])
{
	assert(argc > 1 && "Missing arguments");

	int i;
	for (i = 1; i < argc; i++)
	{
		FILE *fptr;
		if (! (fptr = fopen(argv[i], "w")))
			goto fopen_error;
		fclose(fptr);
	}

	return EXIT_SUCCESS;

fopen_error:
	fprintf(stderr, "Couldn't create file: %s\n", argv[i]);
	return EXIT_FAILURE;
}
