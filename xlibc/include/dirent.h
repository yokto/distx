#ifndef DIRENT_H
#define DIRENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* DIR;
struct dirent {
	uint64_t       d_ino;       /* Inode number */
	uint64_t       d_off;       /* Not an offset; see below */
	unsigned short d_reclen;    /* Length of this record */
	unsigned char  d_type2;      /* Type of file; not supported // needs to be not d_type2 for building gnumake
				       by all filesystem types */
	char           d_name[256];
};
DIR* opendir(const char * path);
struct dirent* readdir(DIR * dir);
int closedir(DIR* dir);

#ifdef __cplusplus
}
#endif
#endif // DIRENT_H
