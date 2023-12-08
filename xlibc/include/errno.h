#ifndef ERRNO_H
#define ERRNO_H
#ifdef __cplusplus
extern "C" {
#endif

extern __thread int errno;
#define errno errno

#define ERANGE 1
#define ENOENT  2
#define EINTR  4
#define ENOEXEC  8
#define ENOMEM 12
#define EACCES 13
#define	EBUSY 16
#define	EILSEQ 17
#define EINVAL 22
#define ENOTDIR 20
#define ETIMEDOUT 110
#define ESHUTDOWN 108


#ifdef __cplusplus
}
#endif
#endif
