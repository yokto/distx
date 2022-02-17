#include <stdio.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))


DLL_PUBLIC
char c_getchar(void) {
	return external_c_getchar();
}

DLL_PUBLIC
int c_printf(const char *restrict format, va_list argp) {
	return external_c_printf(format, argp);
}
