#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <grp.h>

void Help (char *fname);

int main (int argc, char *argv[argc + 1])
{
	if (argc < 2){ fprintf(stderr, "Missing args, run with <-h>\n"); exit(1); }
	if (!strcmp("-h", argv[1])) Help(argv[0]);

	char cwd[1000];
	getcwd(cwd, sizeof(cwd));

	char *fname;
	fname = cwd;
	strcat(fname, "/");
	strcat(fname, argv[2]);

	printf("User information:\n");

	struct passwd *pw = getpwnam(argv[1]);
	if (!pw)
	{
		printf("Coud not get user info\n");
		exit(EXIT_FAILURE);
	}
	printf("user login name: %s\n", pw->pw_name);
	printf("user full name: %s\n", pw->pw_gecos);
	printf("user login directory: %s\n", pw->pw_dir);
	printf("user login shell: %s\n", pw->pw_shell);
	printf("user uid: %d\n", pw->pw_uid);
	printf("user gid: %d\n", pw->pw_gid);

	if (chown (fname, pw->pw_uid, pw->pw_gid) == -1)
	{
		fprintf(stderr, "Could not change owner\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

void Help(char *fname)
{
	printf("Usage:\n%s <-h> <user>:<group> <file>\n    -h    Show help message\n", fname);
	exit(0);
}
