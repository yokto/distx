#pragma once

#include <sys/types.h>
#include <base/fs.h>

#ifdef __cplusplus
extern "C" {
#endif

struct flock {
	short l_type;    /* Type of lock: F_RDLCK,
			    F_WRLCK, F_UNLCK */
	short l_whence;  /* How to interpret l_start:
			    SEEK_SET, SEEK_CUR, SEEK_END */
	off_t l_start;   /* Starting offset for lock */
	off_t l_len;     /* Number of bytes to lock */
	pid_t l_pid;     /* PID of process blocking our lock
			    (set by F_GETLK and F_OFD_GETLK) */
};

#define F_SETLK 0
#define F_UNLCK 0
#define F_WRLCK 0
#define F_SETLKW 0

int fcntl(int fd, int cmd, ... /* arg */ );

int open(const char *pathname, int flags, ...);
int open64(const char *pathname, int flags, ...);

#define S_IRWXU 0700
#define S_IRUSR 0400
#define S_IWUSR 0200
#define S_IXUSR 0100
#define S_IRWXG 0070
#define S_IRGRP 0040
#define S_IWGRP 0020
#define S_IXGRP 0010
#define S_IRWXO 0007
#define S_IROTH 0004
#define S_IWOTH 0002
#define S_IXOTH 0001


#ifdef __cplusplus
}
#endif
