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
	int32_t st_dev;
	int32_t st_ino;
	int32_t st_nlink;
	int64_t st_size;
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
int lstat(const char * pathname, struct stat * statbuf);
int fstat(int fd, struct stat *statbuf);

#define S_IFMT 0xf000
#define S_IFREG 0x8000
#define S_IFDIR 0x8000
#define S_IFBLK 0

#ifdef __cplusplus
}
#endif
