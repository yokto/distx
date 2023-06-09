#ifndef STDIO_H
#define STDIO_H

#include <systypes.h>
#include <stdarg.h>

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

#ifdef __cplusplus
}
#endif
#endif // STDIO_H
