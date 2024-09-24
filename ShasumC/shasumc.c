/*
 * Unix Like C
 *
 * shasumc.c
 *
 * Generate sha (256) digest for a file
 * and validate it
 */

/*
 * For the sake of overdoing, I will use linked list
 * and as much pointers as possible
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <assert.h>

#define FILE_BUFFER 32768

void compute_sha256(FILE *file, unsigned char *hash);
void print_hash(unsigned char *hash);

int main(int argv, char **argc)
{

    assert(argv > 1 && "Usage: shasumc filename");

    FILE 
    return EXIT_SUCCESS;
}

