#ifndef UNISTD_H
#define UNISTD_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <base/types.h>
#include <base/fs.h>
#include <stdio.h>
#include <sys/types.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define O_RDONLY BASE_FS_OPEN_READ
#define O_WRONLY BASE_FS_OPEN_WRITE
#define O_RDWR (BASE_FS_OPEN_WRITE | BASE_FS_OPEN_READ)
#define O_CREAT BASE_FS_OPEN_CREATE
#define O_LARGEFILE 0
#define O_EXCL 0
#define O_TRUNC 0
#define O_APPEND 0

extern char ** environ;

uint64_t getsid(pid_t pid);

inline static ssize_t write(int fd, const void *buf, size_t count) {
	uintptr_t written;
	base_fs_write(fd, buf, count, &written);
	return written;
}

inline static ssize_t read(int fd, void *buf, size_t count) {
	uintptr_t read;
	base_fs_read(fd, buf, count, &read);
	return read;
}

inline static int close(int fd) {
	return base_fs_close(fd);
}


inline static int64_t lseek(int fd, int64_t offset, int whence) {
	int64_t ret;
	base_fs_seek(fd, offset, whence, &ret);
	return ret;
}
#define lseek64 lseek

inline static int fsync(uintptr_t fd) { return 0; }

inline static int unlink(const char* path) {
	return remove(path);
}

pid_t getpid(void);

#define _SC_ARG_MAX 0
#define _POSIX_ARG_MAX 1
#define _SC_PAGE_SIZE 2
#define _SC_GETPW_R_SIZE_MAX 3
inline static long sysconf(int name) {
	if (name == _SC_PAGE_SIZE) { return 4096; }
	if (name == _SC_GETPW_R_SIZE_MAX) { return 4096; }
	return -1;
}

unsigned int alarm(unsigned int seconds);

int dup2(int oldfd, int newfd);

pid_t fork(void);

int execve(const char *pathname, char *const argv[],
		char *const envp[]);
int execv(const char *pathname, char *const argv[]);

void _exit(int status);

#define R_OK 1
#define W_OK 2
#define X_OK 4
#define F_OK 8

int access(const char *pathname, int mode);
int eaccess(const char *pathname, int mode);

int symlink(const char *target, const char *linkpath);

int link(const char *oldpath, const char *newpath);


int chown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);


uid_t getuid(void);


int usleep(useconds_t usec);

int pipe(int pipefd[2]);

#ifdef __cplusplus
}
#endif
#endif // UNISTD_H
