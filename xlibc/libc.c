#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

#define concat2(X, Y) X ## Y
#define concat(X, Y) concat2(X, Y)
#define DECLARE(ret, name, ...) \
	ret name(__VA_ARGS__); \
	static ret (*concat(name, _ms))(__VA_ARGS__) __attribute((ms_abi)); \
	static ret (*concat(name, _sysv))(__VA_ARGS__); 
/*
#define DECLARE_VAR(typ, name) \
	extern typ concat(external_windows_c_, name); \
	extern typ concat(external_linux_c_, name);
*/

void* __dlopen(char* name) __attribute__ ((weak));
void* __dlopen(char* name __attribute__((unused))) { return 0; }
void* __dlsym(void* handle, char* name) __attribute__ ((weak));
void* __dlsym(void* handle __attribute__((unused)), char* name __attribute__((unused))) { return 0; }
int __printf(char* restrict fmt, ...) __attribute__ ((weak));
int __printf(char* restrict fmt __attribute__((unused)), ...) { return 0; }
void __exit(int ret) __attribute__ ((weak));
void __exit(int ret __attribute__((unused))) { return; }

static bool isWin = false;

DECLARE(int, vfprintf, void* file, const char *restrict fmt, va_list p)
DECLARE(void*, malloc, size_t new_size)
DECLARE(void, free, void* ptr)
DECLARE(char, getc, void* file)

DLL_PUBLIC void * stdin = 0;
DLL_PUBLIC void * stdout = 0;
DLL_PUBLIC void * stderr = 0;

static void* dlsym(void* lib, char* name) {
	void* ret = __dlsym(lib, name);
	if (ret) { return ret; }
	__printf("couldn't find symbol %s\n", name);
	__exit(-1);
	return 0;
}

__attribute__((constructor)) void init() {
	__printf("hello\n");
	void* libc = 0;
	if (!libc) {
		libc = __dlopen("msvcrt.dll");
		isWin = true;
	}
	if (!libc) {
		libc = __dlopen("libc.so.6");
		isWin = false;
	}
	if (!libc) {
		libc = *(void**)0;
	}

	if (isWin) {
		vfprintf_ms = dlsym(libc, "vfprintf");
		malloc_ms = dlsym(libc, "malloc");
		free_ms = dlsym(libc, "free");
		void* (*fdopen)(int, char*) __attribute((ms_abi)) = dlsym(libc, "getc");
		stdin = fdopen( 0, "r" );
		stdout = fdopen( 1, "a" );
		stderr = fdopen( 2, "a" );
	} else {
		vfprintf_sysv = dlsym(libc, "vfprintf");
		malloc_sysv = dlsym(libc, "malloc");
		free_sysv = dlsym(libc, "free");
		stdin = dlsym(libc, "_IO_2_1_stdin_");
		stdout = dlsym(libc, "_IO_2_1_stdout_");
		stderr = dlsym(libc, "_IO_2_1_stderr_");
	}
	//external_linux_c_dprintf(1, "setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
	//printf("setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
}

DLL_PUBLIC
char getc(void* file) {
	if (isWin) {
		return getc_ms(file);
	} else {
		return getc_sysv(file);
	}
}

DLL_PUBLIC
char getchar() {
	return getc(stdin);
}

DLL_PUBLIC
int vfprintf(void* stream, const char *restrict format, va_list argp) {
	if (isWin) {
		return vfprintf_ms(stream, format, argp);
	} else {
		return vfprintf_sysv(stream, format, argp);
	}
}

DLL_PUBLIC
int printf(const char *restrict format, ...) {
	va_list args;
	va_start(args, format);
	int ret = vfprintf(stdout, format, args);
	va_end(args);
	return ret;
}
//
//// found when compiling for arm64 just ignore flags and don't do checks
//DLL_PUBLIC
//int __printf_chk(int flag, const char * format, ...) {
//	va_list args;
//	va_start(args, format);
//	int ret = vfprintf(stdout, format, args);
//	va_end(args);
//	return ret;
//}
//
DLL_PUBLIC
void free(void* ptr) {
	if (isWin) {
		return free_ms(ptr);
	} else {
		return free_sysv(ptr);
	}
}


DLL_PUBLIC
void* malloc(size_t new_size) {
	if (isWin) {
		return malloc_ms(new_size);
	} else {
		return malloc_sysv(new_size);
	}
}
//
//int puts(const char* str) {
//	return fputs(str, stdout);
//}
//
//DLL_PUBLIC
//int fputs(const char* str, void* f) {
//	if (external_elfator_os() == OS_LINUX) {
//		return external_linux_c_fputs(str, f);
//	} else if (external_elfator_os() == OS_WINDOWS) {
//		return external_windows_c_fputs(str, f);
//	}
//}
