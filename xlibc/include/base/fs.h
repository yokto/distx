// DONT USE
// THIS WILL BE WORKED OVER
// AN ASYNCHRONOUS API MIGHT MAKE MORE SENSE
// EVERYTHING ASYNCIO
// MIGHT MAKE MORE SENSE
// ESPECIALLY FOR NETWORK FILESYSTEMS
#ifndef BASE_FS_H
#define BASE_FS_H

#include <base/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BASE_FS_READ_EOF (-1)


// standard file handles
extern uintptr_t base_fs_stdout;
extern uintptr_t base_fs_stderr;
extern uintptr_t base_fs_stdin;

#define BASE_FS_OPEN_READ        1
#define BASE_FS_OPEN_WRITE       2
#define BASE_FS_OPEN_APPEND      4
#define BASE_FS_OPEN_CREATE      8
/**
 * returns 0 on success. In this case fd is set to the newly opend file descriptor.
 * returns a positive error number otherwise.
 */
int32_t base_fs_open(const char *path, uintptr_t* fd, uint32_t flags);

/**
 * returns 0 on success.
 * returns -1 == BASE_FS_EOF on if eof was reached.
 * returns a positive error number otherwise.
 * In any case read is set to the number of bytes read.
 */
int32_t base_fs_read(uintptr_t fd, void *buf, uintptr_t count, uintptr_t* read);

/** 
 * returns 0 on success.
 * returns a positive error number otherwise.
 * In either case written is set to the number of bytes written.
 */
int32_t base_fs_write(uintptr_t fd, const void *buf, uintptr_t count, uintptr_t* written);

/** 
 * returns 0 on success.
 * returns a positive error number otherwise.
 */
int32_t base_fs_close(uintptr_t fd);

#define BASE_FS_SEEK_SET   1
#define BASE_FS_SEEK_CUR   2
#define BASE_FS_SEEK_END   3
int32_t base_fs_seek(uintptr_t fd, int64_t offset, uint32_t whence, int64_t* new_offset);

int32_t base_fs_truncate(uintptr_t fd, int64_t length);

int32_t base_fs_tell(uintptr_t fd, int64_t * pos);

#ifdef __cplusplus
}
#endif
#endif // BASE_FS_H
