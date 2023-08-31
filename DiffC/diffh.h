typedef struct
{
    char *src_path;
    char *dst_path;
}TargetInfo;

typedef struct
{
    char **file_paths;
    int file_index;
    char **dir_paths;
    int dir_index;
}DirFiles;

void show_help(char *binary_name);

TargetInfo *set_target_info(char *src, char *dst);
void _trail_paths(TargetInfo *target_info);
void free_target_info(TargetInfo *target_info);

int is_dir_path(char *path);

int diff_files(char *src, char *dst);
void *_open_file(char *fpath);
int _file_size(FILE *fptr);
int compare_files(char *src, char *dst);

void print_diff(char *buffer, int line, int status);

int diff_dirs(TargetInfo *target_info);
void *_open_dir(char *dpath);
DirFiles *set_dir_files(char *target_dir);
void free_dir_files(DirFiles *dir_files);
void _get_dir_files_paths(DIR *dptr, DirFiles *dir_files, char *current_dir);
char *_get_file_name(char *full_file_path);
