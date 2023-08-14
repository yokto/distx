// import only once to where it's used
#include <base/types.h>

#define WIN_SEEK_CUR    1
#define WIN_SEEK_SET    0
#define WIN_SEEK_END    2
#define WIN_O_CREAT    256
#define WIN_O_APPEND   8
#define WIN_O_RDONLY   0
#define WIN_O_WRONLY   1
#define WIN_O_RDWR     2

int (*win_wopen)(const uint16_t *filename, int oflag, int mode) __attribute((ms_abi));
int64_t (*win_lseeki64)(int fd, int64_t offset, int origin) __attribute((ms_abi));
int32_t (*win_chsize_s)(int fd, int64_t size) __attribute((ms_abi));
int (*win_read)(int const fd, void * const buffer, unsigned const buffer_size) __attribute((ms_abi));
int (*win_write)(int fd, const void *buffer, unsigned int count) __attribute((ms_abi));
int (*win_close)(int fd) __attribute((ms_abi));
