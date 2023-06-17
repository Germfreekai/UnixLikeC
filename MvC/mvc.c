/*
 * Author: Rene Garcia Aviles
 * Mv like C implementation
 * Contact: renega1238@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include "mvc.h"

#define INT_MAX 1000

int main(int argc, char *argv[argc + 1])
{
	if ((argc >= 2) && (! strcmp(argv[1], "-h")))
		goto help_scss;
	else if (argc <= 2)
		goto help_err;

	move_files(argc, argv);

	return EXIT_SUCCESS;

help_err:
	help(argv[0], 1);
help_scss:
	help(argv[0], 0);
}

/*
 * Show help message
 */
void help(char *prog, int status)
{
	fprintf(stdout, "Usage: %s SRC DEST", prog);
	
	// Exit with failure stat if '-h' arg not given
	(! status) ? exit(EXIT_SUCCESS) : exit(EXIT_FAILURE);
}

/*
 * Parse arguments and move files
 */
void *move_files(int argc, char **argv)
{
	// mv_files - src_file - dst_file
	mv_files *mvfiles = (mv_files*)malloc(sizeof(mv_files));

	/* Get dst info - dst must always be last arg */
	mvfiles->dstinfo = get_arg_info(argv[argc - 1]);
	if (mvfiles->dstinfo->is_dir && argc >= 4)
		goto err_dst_no_dir;

	for (int i = 1; i < argc - 1; i++)
	{
		if (i != 1)
			free_arg_info(mvfiles->srcinfo);
		mvfiles->srcinfo = get_arg_info(argv[i]);

		_move_files(mvfiles);
	}
	
	free_mvfiles(mvfiles);

	return NULL;

err_dst_no_dir:
	fprintf(stderr, "Dst directory does not exist: %s\n", mvfiles->dstinfo->full_path);
	free_arg_info(mvfiles->dstinfo);
	free(mvfiles);
	exit(EXIT_FAILURE);
}

/*
 * Get dst info: path - name - full_path
 * strcat returns a null terminated string
 */
arg_info *get_arg_info(char *arg)
{
	arg_info *arginfo = (arg_info*)malloc(sizeof(arg_info));
	arginfo->file_path = (char*)calloc(INT_MAX, sizeof(char));
	arginfo->file_name = (char*)calloc(INT_MAX, sizeof(char));
	arginfo->full_path = (char*)calloc(INT_MAX, sizeof(char));

	// Verify if it is a valid dir
	arginfo->is_dir = dir_path_exist(arg);

	int arg_len;
	arg_len = strlen(arg);

	int count;
	count = 0;

	int i;
	// Remove null terminated char from count
	for (i = arg_len - 1; i >= 0; i--)
		(arg[i] != '/') ? (count++) : (i = -1);

	// Argument ends with '/'
	if (count == 0)
		goto trailing_slash;

	// Add full path - could be either a file or a dir
	strcat(arginfo->full_path, arg);

	// Add just path - if single word, copy it
	(arg_len == count) ? (strncat(arginfo->file_path, arg, arg_len))
	                   : (strncat(arginfo->file_path, arg, arg_len - count));

	// Append just chars after last '/' to get name 
	for (i = 0; i <= arg_len; i++)
		if (i >= (arg_len - count))
			strncat(arginfo->file_name, &arg[i], 1); /* Append one char */

	return arginfo;

trailing_slash:
	strncat(arginfo->file_path, arg, arg_len);
	strcat(arginfo->file_name, "NULL");
	strcat(arginfo->full_path, arg);
	return arginfo;
}

/*
 * Verify if given path (dst)
 * exists - get path str
 * and name str
 */
int dir_path_exist(char *path)
{
	/* Returns 0 if exists and 1 if it does not */
	DIR *dir;
	if ((dir = opendir(path)) != NULL)
	{
		closedir(dir);
		return 0;
	}
	else
		return 1;
}

/*
 * Move files auxiliar function.
 * Copies file from src to dst
 */
void *_move_files(mv_files *mvfiles)
{
	char *resulting_file = (char*)calloc(INT_MAX, sizeof(char));
	int res;
	
	// Recursive function move not supported atm
	if ((! mvfiles->srcinfo->is_dir) || (! strcmp(mvfiles->srcinfo->file_name, "NULL")))
		goto err_src_dir;
	// Dst dir does not exist
	if ((mvfiles->dstinfo->is_dir) && (! strcmp(mvfiles->dstinfo->file_name, "NULL")))
		goto err_invalid_dst_dir;

	// If dst dir exists - move file to this directory
	// We know that full path is a directory
	if (! mvfiles->dstinfo->is_dir)
	{
		strcat(resulting_file, mvfiles->dstinfo->full_path);
		// If dstinfo->file_name != NULL we do not have trailing slash
		if (strcmp(mvfiles->dstinfo->file_name, "NULL"))
			strcat(resulting_file, "/");
		strcat(resulting_file, mvfiles->srcinfo->file_name);

		if ((res = write_file(mvfiles->srcinfo->full_path, resulting_file)) == 1)
			goto err_write_file_src;
		else if (res == 2)
			goto err_write_file_dst;
		else if (res == 3)
			goto err_remove_src;
	}
	else
	{
		// File_path is not a dir
		if (dir_path_exist(mvfiles->dstinfo->file_path))
			goto err_invalid_dst_dir;
		
		// Same name used
		if (! strcmp(mvfiles->srcinfo->file_name, mvfiles->dstinfo->file_name))
		{
			strcat(resulting_file, mvfiles->dstinfo->file_path);
			strcat(resulting_file, mvfiles->srcinfo->file_name);

			if ((res = write_file(mvfiles->srcinfo->full_path, resulting_file)) == 1)
				goto err_write_file_src;
			else if (res == 2)
				goto err_write_file_dst;
			else if (res == 3)
				goto err_remove_src;
		}
		// Different name used
		else
		{
			if ((res = write_file(mvfiles->srcinfo->full_path, mvfiles->dstinfo->full_path)) == 1)
				goto err_write_file_src;
			else if (res == 2)
				goto err_write_file_dst;
			else if (res == 3)
				goto err_remove_src;
		}
	}

	free(resulting_file);

	return NULL;
err_src_dir:
	fprintf(stderr, "Recursive dir move not supported at current version\n");
	free(resulting_file);
	free_mvfiles(mvfiles);
	exit(EXIT_FAILURE);
err_invalid_dst_dir:
	fprintf(stderr, "Dst dir does not exits\n");
	free(resulting_file);
	free_mvfiles(mvfiles);
	exit(EXIT_FAILURE);
err_write_file_src:
	fprintf(stderr, "Failed to read src file: %s\n", mvfiles->srcinfo->full_path);
	free(resulting_file);
	free_mvfiles(mvfiles);
	exit(EXIT_FAILURE);
err_write_file_dst:
	fprintf(stderr, "Failed to write dst file: %s\n", resulting_file);
	free(resulting_file);
	free_mvfiles(mvfiles);
	exit(EXIT_FAILURE);
err_remove_src:
	fprintf(stderr, "Could not remove src file: %s\n", mvfiles->srcinfo->full_path);
	free(resulting_file);
	free_mvfiles(mvfiles);
	exit(EXIT_FAILURE);
}

/*
 * Write file from src to dst
 * Delete src file after write
*/
int write_file(char *src, char *dst)
{
	FILE *src_file;
	FILE *dst_file;
	char ch;

	// Open src file
	if (! (src_file = fopen(src, "r")))
		return 1;
	
	// Open dst file 
	if (! (dst_file = fopen(dst, "w")))
		return 2;

	ch = 32;
	while ((ch = fgetc(src_file)) != EOF)
		fprintf(dst_file, "%c", ch);

	// Close files
	fclose(src_file);
	fclose(dst_file);

	// Remove src file
	if (remove(src))
		return 3;

	return 0;

}
/*
 * Free dynamic memory
 * for struct mv_files
 */
void free_arg_info(arg_info *arginfo)
{
	free(arginfo->file_path);
	free(arginfo->file_name);
	free(arginfo->full_path);
	free(arginfo);
}
void free_mvfiles(mv_files *mvfiles)
{
	free_arg_info(mvfiles->srcinfo);
	free_arg_info(mvfiles->dstinfo);

	free(mvfiles);
}