#ifndef STDIO_H
#define STDIO_H

#include <base/types.h>
#include <stdarg.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef __SIZE_TYPE__ size_t;
typedef signed long ssize_t;

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
char *fgets(char * s, int size, FILE * stream);
int vfprintf(FILE* stream, const char * format, va_list argp);
int printf(const char * format, ...);
int fprintf(FILE* stream, const char * format, ...);
int snprintf(char * s, size_t n, const char * format, ...);
int sprintf(char * s, const char * format, ...);
int vsnprintf(char * s, size_t n, const char * format, va_list arg);
int vasprintf(char ** strp, const char * format, va_list arg );
int feof(FILE *stream);
//int __printf_chk(int flag, const char * format, ...);
int puts(const char* str);
int putchar(int c);
int fputs(const char* str, FILE* f);
int fputc(int c, FILE *stream);
int vsscanf(const char *s, const char *format, va_list arg);
int sscanf(const char *s, const char *format, ...);
int scanf(const char *format, ...);
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

//int chmod(const char *pathname, mode_t mode);


typedef uint64_t mode_t;
#define S_ISLNK(mode) (false)
#define S_ISREG(mode) (mode & 0x8000)
#define S_ISDIR(mode) (mode & 0x4000)
#define S_ISBLK(mode) (false)
#define S_ISCHR(mode) (false)
#define S_ISFIFO(mode) (false)
#define S_ISSOCK(mode) (false)

//int open(const char *pathname, int flags, mode_t mode);
//int close(int fd);

typedef int64_t off_t;

int mkdir(const char *pathname, mode_t mode);
char *realpath(const char * path,
               char * resolved_path);
char *getcwd(char *buf, size_t size);
int chdir(const char *path);
ssize_t readlink(const char * pathname, char * buf, size_t bufsiz);
int rename(const char *oldpath, const char *newpath);
int truncate(const char *path, off_t length);
typedef uint64_t fsblkcnt_t;
typedef uint64_t fsfilcnt_t;
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

int ferror(FILE *stream);


#define O_CLOEXEC 524288
//#define O_RDONLY 0
#define O_DIRECTORY 65536
#define O_NOFOLLOW 131072

#define PATH_MAX 4096

void perror(const char *s);


#ifdef __cplusplus
}
#endif
#endif // STDIO_H
