#pragma once

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t __fsword_t;


struct statfs {
	__fsword_t f_type;    /* Type of filesystem (see below) */
	__fsword_t f_bsize;   /* Optimal transfer block size */
	fsblkcnt_t f_blocks;  /* Total data blocks in filesystem */
	fsblkcnt_t f_bfree;   /* Free blocks in filesystem */
	fsblkcnt_t f_bavail;  /* Free blocks available to
				 unprivileged user */
	fsfilcnt_t f_files;   /* Total inodes in filesystem */
	fsfilcnt_t f_ffree;   /* Free inodes in filesystem */
	fsid_t     f_fsid;    /* Filesystem ID */
	__fsword_t f_namelen; /* Maximum length of filenames */
	__fsword_t f_frsize;  /* Fragment size (since Linux 2.6) */
	__fsword_t f_flags;   /* Mount flags of filesystem
				 (since Linux 2.6.36) */
	__fsword_t f_spare[10];
	/* Padding bytes reserved for future use */
};


int statfs(const char *path, struct statfs *buf);
int fstatfs(int fd, struct statfs *buf);

#ifdef __cplusplus
}
#endif
