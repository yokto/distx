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
#define DECLARE_VAR(typ, name) \
	extern typ concat(external_windows_c_, name); \
	extern typ concat(external_linux_c_, name); 

DECLARE(char, getc)
DECLARE(int, printf, const char * fmt, ...)
DECLARE(void, free, void* ptr) 
DECLARE(void*, malloc, size_t new_size) 
DECLARE(int, puts, const char* str) 
void * external_windows_c__fdopen(int fd, const char* mode) __attribute((ms_abi)); 

DECLARE_VAR(void*, stdin)
DECLARE_VAR(void*, stdout)
DECLARE_VAR(void*, stderr)

uint32_t external_elfator_os();

DLL_PUBLIC void * stdin = 0;
DLL_PUBLIC void * stdout = 0;
DLL_PUBLIC void * stderr = 0;

__attribute__((constructor)) void init() {
	if (external_elfator_os() == OS_LINUX) {
		//external_linux_c_dprintf(1, "set stdin at %p to %p\n", stdin_ptr, &external_linux_c_stdin);
		stdin = external_linux_c_stdin;
		stdout = external_linux_c_stdout;
		stderr = external_linux_c_stderr;
	} else if (external_elfator_os() == OS_WINDOWS) {
		stdin = external_windows_c__fdopen( 0, "r" );
		stdout = external_windows_c__fdopen( 1, "r" );
		stderr = external_windows_c__fdopen( 2, "r" );
	}
}

DLL_PUBLIC
char getc(void* file) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_getc(file);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_getc(file);
	}
}

DLL_PUBLIC
char getchar(const char* str) {
	return getc(stdin);
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

int puts(const char* str) {
	return fputs(str, stdout);
}

DLL_PUBLIC
int fputs(const char* str, void* f) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_fputs(str, f);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_fputs(str, f);
	}
}
