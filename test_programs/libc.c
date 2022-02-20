#include <stdio.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

#define OS_LINUX 1
#define OS_WINDOWS 0


DLL_PUBLIC
char c_getchar(void) {
//	if (external_elvator_os() == OS_LINUX) {
		return external_c_getchar();
	//} else if (external_elvator_os() == OS_WINDOWS) {
		return external_c_getchar();
	//}
}

DLL_PUBLIC
int c_printf(const char *restrict format, va_list argp) {
	return external_c_printf(format, argp);
}
