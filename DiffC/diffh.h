typedef struct
{
    char *src_path;
    char *dst_path;
}TargetInfo;

void show_help(char *binary_name);

TargetInfo *set_target_info(char *src, char *dst);
void _trail_paths(TargetInfo *target_info);
void free_target_info(TargetInfo *target_info);

int is_dir_path(char *path);

int diff_files(TargetInfo *target_info);
void *_open_file(char *fpath);
int _file_size(FILE *fptr);
int compare_files(FILE *src_file, FILE *dst_file);

void print_diff(char *buffer, int line, int status);
