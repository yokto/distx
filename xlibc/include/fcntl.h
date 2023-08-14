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
#ifdef __cplusplus
}
#endif
