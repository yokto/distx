//#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

#define OS_LINUX 1
#define OS_WINDOWS 0

typedef uint64_t size_t;

#define concat2(X, Y) X ## Y
#define concat(X, Y) concat2(X, Y)
#define DECLARE(ret, name, ...) \
	ret concat(external_windows_c_, name)(__VA_ARGS__) __attribute((ms_abi)); \
	ret concat(external_linux_c_, name)(__VA_ARGS__) __attribute((ms_abi)); 

DECLARE(char, getchar)
DECLARE(int, printf, const char * fmt, ...)
DECLARE(void, free, void* ptr) 
DECLARE(void*, malloc, size_t new_size) 

uint32_t external_elfator_os();

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

DLL_PUBLIC
void free(void* ptr) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_free(ptr);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_free(ptr);
	}
}


DLL_PUBLIC
void* malloc(size_t new_size) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_malloc(new_size);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_malloc(new_size);
	}
}
