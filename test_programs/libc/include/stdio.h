#include <stdint.h>

char getc(void* file);
char getchar(const char* str);
int vfprintf(void* stream, const char *restrict format, va_list argp);
int printf(const char *restrict format, ...)
int __printf_chk(int flag, const char * format, ...);
int puts(const char* str);
int fputs(const char* str, void* f);
