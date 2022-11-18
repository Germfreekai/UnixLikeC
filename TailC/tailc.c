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
	// FILE *fptr; 
	
	int lines; 
	lines = 10;

	char *file = (char*)calloc(1000,sizeof(char)); 

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
	free(file); 

	int file_n_lines;
	file_n_lines = get_file_lines_number();

	if (!file_n_lines)
	{
		fclose(fptr); 
		exit(0); 
	}
	
	//tail_print_file(fptr, F, lines, file_n_lines); 
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
	// return file pointer to start of the file 
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

	fclose(fptr); 
}

void keep_reading()
{
	char ch; 
	int len; 

	len = ftell(fptr); 
	printf("len : %d\n", len); 

	while (1)
	{
		sleep(0.2); 
		ch = fgetc(fptr); 
		len = ftell(fptr); 
		printf("len : %d\n", len); 
		if (ch != EOF)
			putchar(ch); 
		fflush(stdout); 
	}
}

void signalHandler(int sig)
{
	if (!fptr)
		fclose(fptr); 

	exit(0); 
}
