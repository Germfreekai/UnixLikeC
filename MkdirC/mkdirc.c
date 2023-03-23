/*
 * Author: Rene Garcia Aviles
 * Mkdir like C implementation
 * Contact: renega1238@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

void Help(char *sname);
void Mkdir(char *dname, int perms);
int OctalN(int perms);

int main(int argc, char *argv[argc + 1])
{
	if (argc < 2){ fprintf(stderr, "Missing arguments, run ./%s <-h> for more information\n", argv[0]); exit(EXIT_FAILURE); }
	if (!strcmp(argv[1], "-h")) Help(argv[0]);

	//int perms;
	int perms;
	if (argv[2] && strlen(argv[2]) < 3)
	{
		fprintf(stdout, "Missing permissions for 'others'\nRun ./%s <-h> for more information\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (!argv[2]) 
		perms = 0775;
	else
	{
		perms = atoi(argv[2]);
		perms = OctalN(perms);
	}

	Mkdir(argv[1], perms);

	return EXIT_SUCCESS;
}

void Help(char *sname)
{
	fprintf(stdout, "Usage: ./%s <-h> <dir_name> <perms>\n", sname);
	fprintf(stdout, "       -h        Usage information\n");
	fprintf(stdout, "      <dir_name> <perms>\nNote: give perms on numeric format e.g 775\n");
	exit(EXIT_SUCCESS);
}

int OctalN(int perms)
{
	int octnum;
	octnum = 0;

	int count;
	count = 1;

	int n = perms;

	int rem;

	while (n != 0)
	{
		rem = n % 8;
		
		octnum += rem * count;
		
		count *= 10;

		n /= 8;
	}

	return octnum;
}

void Mkdir(char *dname, int perms)
{
	// if (mkdir(dname, S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH))
	if (mkdir(dname, perms))
	{
		fprintf(stderr, "Failed to create dir: %s\n", dname);
		exit(EXIT_FAILURE);
	}
}
