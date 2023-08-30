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

    int res;

    // If fils and dir given - fail
    if (src_dir != dst_dir)
        goto invalid_comparison;

    if (! src_dir && ! dst_dir)
        fprintf(stdout, "Compare dirs\n");
    else
        res = diff_files(target_info);

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
    fprintf(stderr, "Can't compare a file and a directory\n");
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
    /* Remove trailing from src */
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
int diff_files(TargetInfo *target_info)
{
    FILE *src_file = _open_file(target_info->src_path);
    int src_line = 0;
    char src_ch;

    FILE *dst_file = _open_file(target_info->dst_path);
    int dst_line = 0;
    char dst_ch;

    if (! src_file || ! dst_file)
    {
        (! src_file) ? fclose(dst_file) : fclose(src_file);
        return 2;
    }

    fprintf(stdout, "src/%s -> dst/%s\n\n", target_info->src_path, target_info->dst_path);

    int res = compare_files(src_file, dst_file);

    fclose(src_file);
    fclose(dst_file);

    return res;
}

/*
 * Open given file and return PTR
 */
void *_open_file(char *fpath)
{
    FILE *fptr;

    if (! (fptr = fopen(fpath, "r")))
        fprintf(stdout, "Failed to read: %s\n", fpath);

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
 */
int compare_files(FILE *src_file, FILE *dst_file)
{
    // Get src and dst file size
    int src_size = _file_size(src_file);
    int dst_size = _file_size(dst_file);

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
            strncat(src_line_buffer, &src_ch, 1);
        }
        if (dst_read)
        {
            dst_ch = fgetc(dst_file);
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
               
                print_diff(src_line_buffer, line, 0);
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

    return files_different;
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
    
    if (status)
        fprintf(stdout, "line: %d [+] %s", line, buffer);
    else
        fprintf(stdout, "line: %d [x] %s", line, buffer);
    
    len += strlen(buffer);
    
    if (buffer[len - 1] != 10)
        fprintf(stdout, "\n");
    fprintf(stdout, "---------\n");
}
