#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef uint64_t FILE;
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
int vfprintf(FILE* stream, const char *restrict format, va_list argp);
int printf(const char *restrict format, ...);
//int __printf_chk(int flag, const char * format, ...);
int puts(const char* str);
int fputs(const char* str, FILE* f);

int fseek(FILE* stream, fpos_t offset, int origin );
