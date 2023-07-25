#ifndef STDIO_H
#define STDIO_H

#include <systypes.h>
#include <stdarg.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef void* FILE;
typedef uint64_t fpos_t;

extern FILE* stderr;
extern FILE* stdout;
extern FILE* stdin;

#define EOF -1
#define FOPEN_MAX 0x1000
#define FILENAME_MAX 0x1000
#define BUFSIZ 0x1000

#define _IOFBF 1
#define _IOLBF 2
#define _IONBF 3

#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

#define TMP_MAX 0x10000
#define L_tmpnam 0x40

char getc(FILE* file);
char getchar();
int vfprintf(FILE* stream, const char * format, va_list argp);
int printf(const char * format, ...);
int fprintf(FILE* stream, const char * format, ...);
int snprintf(char * s, size_t n, const char * format, ...);
int vsnprintf(char * s, size_t n, const char * format, va_list arg);
int vasprintf(char ** strp, const char * format, va_list arg );
//int __printf_chk(int flag, const char * format, ...);
int puts(const char* str);
int fputs(const char* str, FILE* f);
int fputc(int c, FILE *stream);
int vsscanf(const char *s, const char *format, va_list arg);
int sscanf(const char *s, const char *format, ...);
FILE *fopen(const char *filename, const char *mode);
size_t fread(void * ptr, size_t size, size_t nmemb, FILE * stream);
size_t fwrite(const void * ptr, size_t size, size_t nmemb, FILE * stream);
long ftell(FILE* stream);
int fseek(FILE *stream, long int offset, int whence);
FILE *fdopen(int fd, const char *mode);
int fclose(FILE* stream);
int ungetc(int c, FILE *stream);



int fflush(FILE*);
int remove(const char *pathname);

int mtime(const char *pathname, struct timespec * time);
int setmtime(const char *pathname, struct timespec * time);

typedef void* DIR;
struct dirent {
	uint64_t       d_ino;       /* Inode number */
	uint64_t       d_off;       /* Not an offset; see below */
	unsigned short d_reclen;    /* Length of this record */
	unsigned char  d_type;      /* Type of file; not supported
				       by all filesystem types */
	char           d_name[256];
};
DIR* opendir(const char * path);
struct dirent* readdir(DIR * dir);
int closedir(DIR* dir);
//int chmod(const char *pathname, mode_t mode);


typedef uint64_t mode_t;
#define S_ISLNK(mode) (mode == 1)
#define S_ISREG(mode) (mode == 2)
#define S_ISDIR(mode) (mode == 3)
#define S_ISBLK(mode) (mode == 4)
#define S_ISCHR(mode) (mode == 5)
#define S_ISFIFO(mode) (mode == 6)
#define S_ISSOCK(mode) (mode == 7)

int open(const char *pathname, int flags, mode_t mode);
int close(int fd);

struct stat {
	mode_t    st_mode;        /* File type and mode */
	int st_dev;
	int st_ino;
	int st_nlink;
	int st_size;
	timespec st_mtim;
};
typedef int64_t off_t;

int stat(const char * pathname, struct stat * statbuf);
int lstat(const char * pathname, struct stat * statbuf);
int fstat(int fd, struct stat *statbuf);
int mkdir(const char *pathname, mode_t mode);
char *realpath(const char * path,
               char * resolved_path);
char *getcwd(char *buf, size_t size);
int chdir(const char *path);
ssize_t readlink(const char * pathname, char * buf, size_t bufsiz);
int rename(const char *oldpath, const char *newpath);
int truncate(const char *path, off_t length);
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;
struct statvfs {
               unsigned long  f_bsize;    /* Filesystem block size */
               unsigned long  f_frsize;   /* Fragment size */
               fsblkcnt_t     f_blocks;   /* Size of fs in f_frsize units */
               fsblkcnt_t     f_bfree;    /* Number of free blocks */
               fsblkcnt_t     f_bavail;   /* Number of free blocks for
                                             unprivileged users */
               fsfilcnt_t     f_files;    /* Number of inodes */
               fsfilcnt_t     f_ffree;    /* Number of free inodes */
               fsfilcnt_t     f_favail;   /* Number of free inodes for
                                             unprivileged users */
               unsigned long  f_fsid;     /* Filesystem ID */
               unsigned long  f_flag;     /* Mount flags */
               unsigned long  f_namemax;  /* Maximum filename length */
           };
int statvfs(const char * path, struct statvfs * buf);


#define O_CLOEXEC 524288
#define O_RDONLY 0
#define O_DIRECTORY 65536
#define O_NOFOLLOW 131072

#define PATH_MAX 4096





#ifdef __cplusplus
}
#endif
#endif // STDIO_H
