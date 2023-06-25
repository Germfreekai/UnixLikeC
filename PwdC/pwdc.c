/*
 * Author: Rene Garcia Aviles
 * Pwd C like implementation
 * Contact: renega1238@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_pwd();

int main(void)
{
	char cwd[150];
	getcwd(cwd, sizeof(cwd));

	fprintf(stdout, "Current dir: %s\n", cwd);

	return EXIT_SUCCESS;
}
