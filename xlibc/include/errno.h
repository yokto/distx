#ifndef ERRNO_H
#define ERRNO_H
#ifdef __cplusplus
extern "C" {
#endif

extern int errno;
#define ERANGE 1
#define EINVAL 22
#define ENOMEM 12
#define	EBUSY 16
#define	EILSEQ 17

#ifdef __cplusplus
}
#endif
#endif
