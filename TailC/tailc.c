#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>

void help(char **argv);
void Tail(int argc, char **argv); 
int get_file_lines_number();
void tail_print_file(int F, int lines, int file_n_lines); 
void keep_reading(); 
void signalHandler(int sig); 

FILE *fptr; 
FILE *tfile;
char *file; 

int main(int argc, char *argv[argc + 1])
{
	assert(argc > 1 && "Missing arguments, run with <-h> for more information"); 

	// Control CTRL C 
	signal(SIGINT, signalHandler); 

	// Call help 
	if (argc == 2 && !strcmp(argv[1], "-h"))
	{
		help(argv); 
	}

	Tail(argc, argv); 

	return EXIT_SUCCESS; 

}

void help(char **argv)
{
	printf("Usage: %s <-h> <options> file\n", argv[0]); 
	printf("       -h               Show help\n"); 
	printf("       -n <number>      Set number of lines to be printed\n"); 
	printf("       -F               Read into a file continuously\n"); 
}

void Tail(int argc, char **argv)
{
	int lines; 
	lines = 10;

	file = (char*)calloc(1000,sizeof(char)); 

	int F; 
	F = 0; 

	// If == 2 -> tail <file> 
	if (argc > 2)
	{
		// We need to loop over the options to find flags
		for (size_t i = 1; i < argc; i++)
		{
			// -n 
			if (!strcmp(argv[i], "-n"))
			{
				i++;
				if (!atoi(argv[i]))
				{
					printf("Wrong argument for flag '-n', run with <-h> for more information\n"); 
					free(file); 
					exit(1); 
				}
				lines = atoi(argv[i]); 
				continue; 
			}
			else if (!strcmp(argv[i], "-F"))
			{
				F = 1; 
			}
			else
			{
				strcpy(file,argv[i]); 
			}
		}
	}
	else
	{
		strcpy(file,argv[1]); 
	}

	// Open file
	fptr = fopen(file, "r"); 
	if (!fptr)
	{
		printf("File <%s> could not be opened or does not exist...\n", file); 
		free(file); 
		exit(1); 
	}
	
	int file_n_lines;
	file_n_lines = get_file_lines_number();

	if (!file_n_lines)
	{
		fclose(fptr); 
		exit(0); 
	}
	
	tail_print_file(F, lines, file_n_lines); 
	
}

// Count number of lines - so we can position our pointer
int get_file_lines_number()
{
	char ch; 

	int count; 
	count = 0; 

	while ((ch = fgetc(fptr)) != EOF)
	{
		if (ch == 10)
			count++;
	}

	return count; 
}

void tail_print_file(int F, int lines, int file_n_lines)
{
	// Return file pointer to start of the file 
	rewind(fptr); 

	int print; 
	print = 0; 

	if (lines > file_n_lines+1)
	{
		if (file_n_lines <= 9)
		{
			lines = 0;
		}
		else
		{
			lines = file_n_lines - 9; 
		}
	}
	else
	{
		lines = file_n_lines - (lines - 1); 
	}

	char ch; 

	int count; 
	count = 0; 
	while ((ch = fgetc(fptr)) != EOF)
	{
		if (count == lines)
			print = 1;
		if (print)
			putchar(ch); 
		if (ch == 10)
			count++; 
	}

	fflush(stdout); 

	if (F)
		keep_reading();

	free(file); 
	fclose(fptr); 
}

void keep_reading()
{
	int len;
	len = ftell(fptr); 
	
	fclose(fptr); 
	
	char ch; 
	int slen; 
	
	while (1)
	{
		// Wait for new lines to add
		sleep(3);

		tfile = fopen(file, "r"); 
		fseek(tfile, 0L, SEEK_END); 
		slen = ftell(tfile); 
		
		// If file is bigger, print new things
		if (slen > len)
		{
			// Where to start writting again
			fseek(tfile, len, SEEK_SET);

			while ((ch = fgetc(tfile)) != EOF)
				putchar(ch);
			
			// new reference point
			len = slen;
		}

		fclose(tfile);
		
		fflush(stdout); 
	}

}

void signalHandler(int sig)
{
	if (!tfile)
		fclose(tfile); 

	free(file); 
	
	exit(0); 
}
