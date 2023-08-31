/*
 * Author: Rene Garcia Aviles
 * Diff like C implementation
 * Contact: renega1238@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "diffh.h"

int main(int argc, char *argv[argc + 1])
{
    if (argc == 1 || ! strcmp(argv[1],"-h"))
        show_help(argv[0]);
    if (argc > 3)
        goto extra_operand;

    TargetInfo *target_info = set_target_info(argv[1], argv[2]);

    int src_dir = is_dir_path(target_info->src_path);
    int dst_dir = is_dir_path(target_info->dst_path);

    int res = 0;

    // If fils and dir given - fail
    if (src_dir != dst_dir)
        goto invalid_comparison;

    if (! src_dir && ! dst_dir)
        res = diff_dirs(target_info);
    else
        res = diff_files(target_info->src_path, target_info->dst_path);

    if (res == 2)
        goto diff_failed;
    
    if (! res)
        goto equal_diff;
    else
        goto no_equal_diff;

extra_operand:
    fprintf(stderr, "Extra operands given: ");
    for (int i = 3; i < argc; i++)
        fprintf(stderr, "%s ", argv[i]);
    exit(EXIT_FAILURE);
invalid_comparison:
    fprintf(stderr, "Can't compare a file and a directory or couldn't open dir\n");
    free_target_info(target_info);
    exit(EXIT_FAILURE);
diff_failed:
    fprintf(stderr, "Something went wrong\n");
    free_target_info(target_info);
    exit(EXIT_FAILURE);
equal_diff:
    free_target_info(target_info);
    exit(EXIT_SUCCESS);
no_equal_diff:
    free_target_info(target_info);
    exit(EXIT_FAILURE);
}

/*
 * Show usage information
 */
void show_help(char *binary_name)
{
    fprintf(stdout, "Usage:\n%s <file | dir> <file | dir>\n", binary_name);
    exit(EXIT_SUCCESS);
}

/*
 * Set src and dst info paths in struct
 * src and dst are NULL terminated strings
 */
TargetInfo *set_target_info(char *src, char *dst)
{
    int src_len = 0;
    int dst_len = 0;

    src_len += strlen(src);
    dst_len += strlen(dst);

    TargetInfo *target_info = (TargetInfo*)malloc(sizeof(TargetInfo));
    
    target_info->src_path = (char*)malloc(src_len + 1);
    target_info->dst_path = (char*)malloc(dst_len + 1);

    target_info->src_path[0] = '\0';
    target_info->dst_path[0] = '\0';

    strcpy(target_info->src_path, src);
    strcpy(target_info->dst_path, dst);

    // Remove trailing slash '/'
    _trail_paths(target_info);
    
    return target_info;
}

/*
 * Remove trailing slash from given paths for
 * src and dst paths
 * Could be a directory or a file, either way
 * get rid of it
 */
void _trail_paths(TargetInfo *target_info)
{
    // Remove trailing from src
    int src_len = 0;
    src_len += strlen(target_info->src_path);
    
    if (target_info->src_path[src_len - 1] == '/')
        target_info->src_path[src_len - 1] = '\0';

    int dst_len = 0;
    dst_len += strlen(target_info->dst_path);
    
    if (target_info->dst_path[dst_len - 1] == '/')
        target_info->dst_path[dst_len - 1] = '\0';   
}

/*
 * Free TargetInfo struct memory
 */
void free_target_info(TargetInfo *target_info)
{
    free(target_info->src_path);
    free(target_info->dst_path);

    free(target_info);
}

/*
 * Verify if given path is a valid directory
 * Returns 
 *  - 0 valid
 *  - 1 invalid
 */
int is_dir_path(char *path)
{
    DIR *dir;
    int res = 1;

    if ( (dir = opendir(path)) != NULL)
    {
        res = 0;
        closedir(dir);
    }

    return res;
}

/*
 * Read src and dst files and compare
 * print results to stdout
 * Returns:
 *  - 0 if files are equal
 *  - 1 if files are not equal
 *  - 2 if failure
 */
int diff_files(char *src, char *dst)
{
    fprintf(stdout, "src/%s -> dst/%s\n\n", src, dst);

    int res = compare_files(src, dst);

    return res;
}

/*
 * Open given file and return PTR
 */
void *_open_file(char *fpath)
{
    FILE *fptr;

    if (! (fptr = fopen(fpath, "r")))
        fprintf(stderr, "Failed to read file: %s\n", fpath);

    return fptr;
}

/*
 * Get file bytes size
 * return size in int
 */
int _file_size(FILE *fptr)
{
    fseek(fptr, 0L, SEEK_END);
    int size = ftell(fptr);

    rewind(fptr);

    return size;
}

/*
 * Read src and dst files and compare line by line
 * Returns:
 *  - 0 if equal
 *  - 1 if not equal
 *  - 2 if failure
 */
int compare_files(char *src, char *dst)
{
    FILE *src_file = _open_file(src);
    FILE *dst_file = _open_file(dst);

    if (! src_file || ! dst_file)
    {
        (! src_file) ? fclose(dst_file) : fclose(src_file);
        return 2;
    }

    // Get src and dst file size
    int src_size = _file_size(src_file);
    int dst_size = _file_size(dst_file);
    if (dst_size == 0 && src_size == 0)
        goto files_empty;

    char *src_line_buffer = (char*)malloc(src_size + 1);
    src_line_buffer[0] = '\0';

    char *dst_line_buffer = (char*)malloc(dst_size + 1);
    dst_line_buffer[0] = '\0';

    int line = 1;

    char src_ch = '\0';
    char dst_ch = '\0';

    int src_read = 1;
    int dst_read = 1;

    int files_different = 0;
    int show_diff = 0;

    while (1)
    {
        if (src_ch == EOF && dst_ch == EOF)
            break;
        
        // Bring chars and add to buffer
        if (src_read)
        {
            src_ch = fgetc(src_file);
            if (src_size != 0)
                strncat(src_line_buffer, &src_ch, 1);
        }
        if (dst_read)
        {
            dst_ch = fgetc(dst_file);
            if (dst_size != 0)
                strncat(dst_line_buffer, &dst_ch, 1);
        }

        if (src_ch != dst_ch)
            show_diff = 1;

        // Keep reading if EOL for src or dst
        if ( (src_ch == 10 && dst_ch != 10) || (src_ch == EOF && dst_ch != EOF) )
            src_read = 0;
        if ( (dst_ch == 10 && src_ch != 10) || (dst_ch == EOF && src_ch != EOF) )
            dst_read = 0;
        if ( (src_ch == 10 && dst_ch == 10)  ||
             (src_ch == EOF && dst_ch == 10) ||
             (dst_ch == EOF && src_ch == 10) )
        {
            // First print buffer
            if (show_diff)
            {
                files_different = 1;

                // Remove ugly char
                if (src_line_buffer[0] == '\0' || src_line_buffer[0] == EOF)
                    src_line_buffer[0] = 32;
                if (dst_line_buffer[0] == '\0' || dst_line_buffer[0] == EOF)
                    dst_line_buffer[0] = 32;
 
                if (src_size != 0)
                    print_diff(src_line_buffer, line, 0);
                if (dst_size != 0)
                    print_diff(dst_line_buffer, line, 1);

                show_diff = 0;
            }
            line++;

            // Empyt buffer
            src_line_buffer[0] = '\0';
            dst_line_buffer[0] = '\0';

            if (src_ch != EOF)
                src_read = 1;
            if (dst_ch != EOF)
                dst_read = 1;
        }
    }

    free(src_line_buffer);
    free(dst_line_buffer);

    fclose(src_file);
    fclose(dst_file);

    return files_different;

files_empty:
    fclose(src_file);
    fclose(dst_file);
    return 0;
}

/*
 * Print diff buffer, wheter introduced
 * or removed regarding dst file
 * Status sets this option
 *  - 1 -> introduced
 *  - 0 -> removed
 */
void print_diff(char *buffer, int line, int status)
{
    int len = 0;
    len += strlen(buffer);
    
    if (status)
        fprintf(stdout, "dst line: %d [+] %s", line, buffer);
    else
        fprintf(stdout, "src line: %d [x] %s", line, buffer);
    
    if (buffer[len - 1] != 10)
        fprintf(stdout, "\n");
    fprintf(stdout, "---------\n");
}

/*
 * Compare directories, look for files only present
 * at one or compare files
 *  - 0 if dirs are equal
 *  - 1 if dirs are not equal
 *  - 2 if failure
 */
int diff_dirs(TargetInfo *target_info)
{
    DirFiles *src_dir_files = set_dir_files(target_info->src_path);
    DirFiles *dst_dir_files = set_dir_files(target_info->dst_path);

    int src_index = 0;
    DIR *src_dir;
    while (src_index < src_dir_files->dir_index)
    {
        src_dir = _open_dir(src_dir_files->dir_paths[src_index]);
        if (! src_dir)
        {
            fprintf(stderr, "Failed to read dir: %s\n", src_dir_files->dir_paths[src_index]);
            goto failed_to_open_dir;
        }
        _get_dir_files_paths(src_dir, src_dir_files, src_dir_files->dir_paths[src_index]);
        src_index++;
        closedir(src_dir);
    }

    int dst_index = 0;
    DIR *dst_dir;
    while (dst_index < dst_dir_files->dir_index)
    {
        dst_dir = _open_dir(dst_dir_files->dir_paths[dst_index]);
        if (! dst_dir)
        {
            fprintf(stderr, "Failed to read dir: %s\n", dst_dir_files->dir_paths[dst_index]);
            goto failed_to_open_dir;
        }
        _get_dir_files_paths(dst_dir, dst_dir_files, dst_dir_files->dir_paths[dst_index]);
        dst_index++;
        closedir(dst_dir);
    }

    // For every file at src_dir_files compare against dst_dir_files
    char *src_file_name;
    char *dst_file_name;

    int res = 0;

    for (int i = 0; i < 120; i++)
    {
        // Compare file name against dst
        if ( strlen(src_dir_files->file_paths[i]) != 0 )
        {
            src_file_name = _get_file_name(src_dir_files->file_paths[i]);

            for (int j = 0; j < 120; j++)
            {
                if ( strlen(dst_dir_files->file_paths[j]) != 0 )
                {
                    dst_file_name = _get_file_name(dst_dir_files->file_paths[j]);

                    if (! strcmp(src_file_name, dst_file_name))
                    {
                        res = diff_files(src_dir_files->file_paths[i], dst_dir_files->file_paths[j]);
                        src_dir_files->file_paths[i][0] = '\0';
                        dst_dir_files->file_paths[j][0] = '\0';
                    }
                    free(dst_file_name);
                }
            }
            free(src_file_name);
        }
    }

    // Print remaining files
    for (int i = 0; i < 120; i++)
        if ( strlen(src_dir_files->file_paths[i]) != 0 )
            fprintf(stdout, "-> Only present at src dir: %s -> %s\n", target_info->src_path, src_dir_files->file_paths[i]);

    for (int i = 0; i < 120; i++)
        if ( strlen(dst_dir_files->file_paths[i]) != 0 )
            fprintf(stdout, "-> Only present at dst dir: %s -> %s\n", target_info->dst_path, dst_dir_files->file_paths[i]);

    free_dir_files(src_dir_files);
    free_dir_files(dst_dir_files);

    return res;

failed_to_open_dir:
    free_dir_files(src_dir_files);
    free_dir_files(dst_dir_files);
    return 2;
}

/* Set dif files struct
 * We assume each dir has < 120 files
 * and file names are <= 500
 */
DirFiles *set_dir_files(char *target_dir)
{
    DirFiles *dir_files = (DirFiles*)malloc(sizeof(DirFiles));
    // Initialize paths buckets
    dir_files->file_paths = (char**)calloc(120, sizeof(char*));
    dir_files->dir_paths = (char**)calloc(120, sizeof(char*));

    // We will have each bucker with
    // /path/to/taget/dir/ ...
    for (int i = 0; i < 120; i++)
    {
        dir_files->file_paths[i] = (char*)calloc(500, sizeof(char));
        dir_files->file_paths[i][0] = '\0';

        dir_files->dir_paths[i] = (char*)calloc(500, sizeof(char));
        dir_files->dir_paths[i][0] = '\0';
        if (i == 0)
            strcat(dir_files->dir_paths[i], target_dir);
    }

    dir_files->file_index = 0;
    dir_files->dir_index = 1;

    return dir_files;
}

/*
 * Open given dir and return PTR
 */
void *_open_dir(char *dpath)
{
    DIR *dptr;

    if (! (dptr = opendir(dpath)))
        fprintf(stderr, "Failed to read dir: %s\n", dpath);

    return dptr;
}

/*
 * Free dir_files struct - each char* per name
 */
void free_dir_files(DirFiles *dir_files)
{
    for (int i = 0; i < 120; i++)
    {
        free(dir_files->file_paths[i]);
        free(dir_files->dir_paths[i]);
    }

    free(dir_files->file_paths);
    free(dir_files->dir_paths);

    free(dir_files);
}

/*
 * Iterate over a directory and store each file name
 * at dir_files struct
 */
void _get_dir_files_paths(DIR *dptr, DirFiles *dir_files, char *current_dir)
{
    struct dirent *drnt;

    // Read/iterate over a dir
    while ( (drnt = readdir(dptr)) != NULL)
    {
        // Skip home and father dirs
        if ( ! strcmp(drnt->d_name, ".") || ! strcmp(drnt->d_name, "..") )
            continue;
        else if (drnt->d_type == DT_DIR)
        {
            // Attach new dir to path
            strcat(dir_files->dir_paths[dir_files->dir_index], current_dir);
            strcat(dir_files->dir_paths[dir_files->dir_index], "/");
            strcat(dir_files->dir_paths[dir_files->dir_index], drnt->d_name);

            dir_files->dir_index++;
        }
        else // For files
        {
            // Attach file name to path
            strcat(dir_files->file_paths[dir_files->file_index], current_dir);
            strcat(dir_files->file_paths[dir_files->file_index], "/");
            strcat(dir_files->file_paths[dir_files->file_index], drnt->d_name);

            dir_files->file_index++;
        }
    }
}

/*
 * Return a file name, without full path
 */
char *_get_file_name(char *full_file_path)
{
    int len = 0;
    len += strlen(full_file_path);

    char *fname = (char*)malloc(len + 1);
    fname[0] = '\0';

    int count = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        if (full_file_path[i] != '/')
            count++;
        else
            break;
    }

    for (int i = len - count; i < len; i++)
        strncat(fname, &full_file_path[i], 1);

    return fname;
}
