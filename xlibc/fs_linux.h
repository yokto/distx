#ifndef XLIBC_LINUX_H
#define XLIBC_LINUX_H

#include <base/types.h>

#define LINUX_SEEK_CUR        1
#define LINUX_SEEK_SET        0
#define LINUX_SEEK_END        2
#define LINUX_O_CREAT         64
#define LINUX_O_APPEND        1024
#define LINUX_O_RDONLY        0
#define LINUX_O_WRONLY        1
#define LINUX_O_RDWR          2

int (*linux_open)(const char *pathname, int flags, int mode);
int64_t (*linux_lseek)(int fd, int64_t offset, int whence);
int (*linux_ftruncate)(int fd, int64_t length);
uintptr_t (*linux_read)(int fd, void *buf, uintptr_t count);
uintptr_t (*linux_write)(int fd, const void *buf, uintptr_t count);
int (*linux_close)(int fd);

#endif // XLIBC_LINUX_H
