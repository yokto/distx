//#include <stdio.h>
#include <stdarg.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

#define OS_LINUX 1
#define OS_WINDOWS 0

char external_windows_c_getchar() __attribute((ms_abi));
char external_windows_c_printf() __attribute((ms_abi));

DLL_PUBLIC
char getchar(void) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_getchar();
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_getchar();
	}
}

DLL_PUBLIC
int printf(const char *restrict format, va_list argp) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_printf(format, argp);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_printf(format, argp);
	}
}
