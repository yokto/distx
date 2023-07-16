#include <systypes.h>
// int internal_printf(int (*putc)(void* arg, int chr), void* arg, const char* format, va_list args);
int __debug_printf(char* restrict fmt, ...);
