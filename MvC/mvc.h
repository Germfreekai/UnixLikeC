typedef struct
{
	char *file_path;
	char *file_name;
	char *full_path;
	int is_dir;
}arg_info;

typedef struct
{
	arg_info *srcinfo; 
	arg_info *dstinfo;
}mv_files;

void help(char *prog, int status);
void *_move_files(mv_files *mvfiles);
void *move_files(int argc, char **argv);
arg_info *get_arg_info(char *arg);
int dir_path_exist(char *dst);
int write_file(char *src, char *dst);
void free_arg_info(arg_info *arginfo);
void free_mvfiles(mv_files *mvfiles);