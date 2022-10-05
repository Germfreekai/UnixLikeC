#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *FileNameExt(char *pfile); 
char *FileName(char *pfile); 
int CmpNames(char *file, char *file_name); 

int main(int argc, char *argv[argc + 1])
{

	char *pfile = (char*)calloc(20, sizeof(char)); 
	char *file = (char*)calloc(20, sizeof(char)); 

	strcpy(pfile, argv[1]); 
	strcpy(file, argv[2]); 

	printf("Full path file: %s\n", pfile); 
	printf("File name: %s\n", file); 

	if (file == pfile)
		printf("Hello\n"); 

	char *fname_ext = FileNameExt(pfile); 
	printf("File name with extension: %s\n", fname_ext); 

	char *fname = FileName(fname_ext); 
	printf("File name without extension: %s\n", fname); 

	printf("Match with %s: %s\n", fname, CmpNames(file, fname) == 0 ? "True" : "False"); 
	printf("Match with %s: %s\n", fname_ext, CmpNames(file, fname_ext) == 0 ? "True" : "False"); 

	free(pfile); 
	free(file); 
	free(fname_ext); 
	free(fname); 

	return EXIT_SUCCESS; 

}

char *FileNameExt(char *pfile)
{
	char *file_name = (char*)calloc(20, sizeof(char)); 

	int f_len = strlen(pfile); 

	int count; 
	count = 0; 
	for (int i = f_len - 1; i >= 0; i--)
	{
		if (pfile[i] != '/')
		{
			count++; 
		}
		else
		{
			i = -1;
		}
	}

	for (int i = f_len - count; i < f_len; i++)
	{
		if ( i == f_len - count)
		{
			snprintf(file_name, 2, "%c", pfile[i]); 
		}
		else
		{
			// Append just one char
			strncat(file_name, &pfile[i], 1);
		}
	}

	return file_name;
}

char *FileName(char *pfile)
{

	char *file_name = (char*)calloc(20,sizeof(char)); 

	int f_len = strlen(pfile); 

	// Extension len
	int e_len; 
	e_len = 0; 

	for (int i = f_len - 1; i >= 0; i--)
	{
		if (pfile[i] != '.')
		{
			e_len++;
		}
		else
		{
			e_len++;
			i = -1; 
		}
	}

	for (int i = 0; i < f_len - e_len; i++)
	{
		if (!i)
		{
			snprintf(file_name, 2, "%c", pfile[i]);
		}
		else
		{
			strncat(file_name, &pfile[i], 1); 
		}
	}

	return file_name;

}

int CmpNames(char *file, char *file_name)
{
	if (strlen(file) != strlen(file_name))
		return 1; 

	while (*file)
	{
		if (file[0] != file_name[0])
			return 1; 

		file++; 
		file_name++;
	}

	return 0;
}

