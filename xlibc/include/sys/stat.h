#pragma once

#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#define st_mtime st_mtim.tv_sec
#define st_atime st_atim.tv_sec
#define st_ctime st_ctim.tv_sec
struct stat {
	mode_t    st_mode;        /* File type and mode */
	int st_dev;
	int st_ino;
	int st_nlink;
	int st_size;
	timespec st_mtim;
	timespec st_atim;
	timespec st_ctim;
	uint32_t     st_uid;         /* User ID of owner */
	uint32_t     st_gid;
};
struct stat64 {
	mode_t    st_mode;        /* File type and mode */
	int st_dev;
	int st_ino;
	int st_nlink;
	int st_size;
	timespec st_mtim;
	timespec st_atim;
	timespec st_ctim;
	uint32_t     st_uid;         /* User ID of owner */
	uint32_t     st_gid;
};

mode_t umask(mode_t mask);

int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);

int stat(const char * pathname, struct stat * statbuf);
int stat64(const char * pathname, struct stat64 * statbuf);
int lstat(const char * pathname, struct stat * statbuf);
int fstat(int fd, struct stat *statbuf);
int fstat64(int fd, struct stat64 *statbuf);

#ifdef __cplusplus
}
#endif
