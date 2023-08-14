#ifndef ERRNO_H
#define ERRNO_H
#ifdef __cplusplus
extern "C" {
#endif

extern __thread int errno;
#define ERANGE 1
#define ENOENT  2
#define ENOMEM 12
#define	EBUSY 16
#define	EILSEQ 17
#define EINVAL 22
#define ENOTDIR 20


#ifdef __cplusplus
}
#endif
#endif
