# Chown C like implementation 

Change file owner using C 

---

I have not been a fan from this implementation - some things I have used and investigated are: 
I also saw that there is a chown function for C: https://man7.org/linux/man-pages/man2/chown.2.html

## struct passwd
---
Reference: https://linux.die.net/man/3/getpwuid

From #include <pwd.h>

struct passwd {
	char *pw_name;
	char *pw_passwd;
	uid_t pw_uid; --> does not work 
	gid_t pw_gid; --> does not work
	time_t pw_change;
	char *pw_class;
	char *pw_gecos;
	char *pw_dir;
	char *pw_shell;
	time_t pw_expire;
};  

Useful to get information from a given user - examples:

struct passwd *pw = getpwnam("germfreekai");
if (!pw)
{
    printf("Could not get user or group UID\n");
    exit(1);
}

printf("user login name: %s\n", pw->pw_name);
printf("user full name: %s\n", pw->pw_gecos);
printf("user login directory: %s\n", pw->pw_dir);
printf("user login shell: %s\n", pw->pw_shell);

--- 

## struct stat
---
Reference: https://man7.org/linux/man-pages/man2/lstat.2.html

From #include <sys/stat.h>

struct stat {
	dev_t     st_dev;         /* ID of device containing file */
	ino_t     st_ino;         /* Inode number */
	mode_t    st_mode;        /* File type and mode */
	nlink_t   st_nlink;       /* Number of hard links */
	uid_t     st_uid;         /* User ID of owner */
	gid_t     st_gid;         /* Group ID of owner */
	dev_t     st_rdev;        /* Device ID (if special file) */
	off_t     st_size;        /* Total size, in bytes */
	blksize_t st_blksize;     /* Block size for filesystem I/O */
	blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
	
	/* Since Linux 2.6, the kernel supports nanosecond
	   precision for the following timestamp fields.
	   For the details before Linux 2.6, see NOTES. */
	
	struct timespec st_atim;  /* Time of last access */
	struct timespec st_mtim;  /* Time of last modification */
	struct timespec st_ctim;  /* Time of last status change */

	#define st_atime st_atim.tv_sec      /* Backward compatibility */
	#define st_mtime st_mtim.tv_sec
	#define st_ctime st_ctim.tv_sec
};

Useful to get information from a file - for example to get file's owner - examples 

struct stat info;
stat("filename", &info);

struct passwd *fpw = getpwuid(info.st_uid);

if (!fpw)
{
    printf("Could not get user or group UID\n");
    exit(1);
}

printf("user login name: %s\n", fpw->pw_name);
printf("user full name: %s\n", fpw->pw_gecos);
printf("user login directory: %s\n", fpw->pw_dir);
printf("user login shell: %s\n", fpw->pw_shell);

