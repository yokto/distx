#ifndef BASE_FS_H
#define BASE_FS_H

#include <base/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BASE_FS_EOF (-1)

/**
 * Translates a utf8, slash separated path to a native system path and writes it into native_path
 * returns 0 on success.
 * returns a positive error number otherwise.
 */
int32_t base_fs_tonativepath(const char *pathname, void* nativepath);
/**
 * Same as base_fs_tonativepath. Except it only calculates the length.
 */
int32_t base_fs_tonativepathlen(const char *pathname, uintptr_t* length);

int32_t base_fs_fromnativepath(const void* nativepath, const char* pathname);
/**
 * Same as base_fs_tonativepath. Except it only calculates the length.
 */
int32_t base_fs_fromnativepathlen(const void* nativepath, uintptr_t* length);

// standard file handles
extern intptr_t base_fs_stdout;
extern intptr_t base_fs_stderr;
extern intptr_t base_fs_stdin;

#define BASE_FS_OPEN_READ        1
#define BASE_FS_OPEN_WRITE       2
#define BASE_FS_OPEN_APPEND      4
/**
 * returns 0 on success. In this case fd is set to the newly opend file descriptor.
 * returns a positive error number otherwise.
 */
int32_t base_fs_open(const void *nativepath, uintptr_t* fd, uint32_t flags);

/**
 * returns 0 on success.
 * returns -1 == BASE_FS_EOF on if eof was reached.
 * returns a positive error number otherwise.
 * In any case read is set to the number of bytes read.
 */
int32_t base_fs_read(uintptr_t* fd, void *buf, uintptr_t count, uintptr_t* read);

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



#ifdef __cplusplus
}
#endif
#endif // BASE_FS_H
