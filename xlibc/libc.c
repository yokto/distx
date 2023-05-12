#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <threads.h>
#include <stdio.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

#define concat2(X, Y) X ## Y
#define concat(X, Y) concat2(X, Y)
#define DECLARE(ret, name, ...) \
	ret name(__VA_ARGS__); \
	static ret (*concat(name, _ms))(__VA_ARGS__) __attribute((ms_abi)); \
	static ret (*concat(name, _sysv))(__VA_ARGS__); 

#define IMPLEMENT(name, ...) \
{ \
	if (concat(name, _ms)) { \
		return concat(name, _ms)(__VA_ARGS__); \
	} else { \
		return concat(name, _sysv)(__VA_ARGS__); \
	} \
}


void* __dlopen(char* name) __attribute__ ((weak));
void* __dlopen(char* name __attribute__((unused))) { return 0; }
void* __dlsym(void* handle, char* name) __attribute__ ((weak));
void* __dlsym(void* handle __attribute__((unused)), char* name __attribute__((unused))) { return 0; }
int __printf(char* restrict fmt, ...) __attribute__ ((weak));
int __printf(char* restrict fmt __attribute__((unused)), ...) { return 0; }
void __exit(int ret) __attribute__ ((weak));
void __exit(int ret __attribute__((unused))) { return; }

static bool isWin = false;

DECLARE(int, vfprintf, FILE* file, const char *restrict fmt, va_list p)
DECLARE(int, snprintf, char * s, size_t n, const char * format, ...)
DECLARE(void*, malloc, size_t new_size)
DECLARE(void*, calloc, size_t nmemb, size_t size)
DECLARE(void*, aligned_alloc, size_t alignment, size_t size); // strictly speaking ms version has exchanged arguments
DECLARE(void*, realloc, void *ptr, size_t size)
DECLARE(void, free, void* ptr)
DECLARE(void, aligned_free, void *ptr);
DECLARE(void *, memset, void *ptr, int value, size_t num);
DECLARE(void *, memcpy, void *restrict dest, const void *restrict src, size_t n);
DECLARE(char, getc, FILE* file)
DECLARE(int, fflush, FILE* file)
DECLARE(char *, getenv, const char *name)
DECLARE(void, thrd_yield, void)
DECLARE(int, mtx_init, mtx_t* mutex, int type)
DECLARE(int, mtx_unlock, mtx_t *mutex)
DECLARE(int, mtx_lock, mtx_t* mutex)
DECLARE(size_t, fwrite, const void * ptr, size_t size, size_t nmemb, FILE * stream)
DECLARE(int, remove, const char *pathname)

DLL_PUBLIC FILE * stdin = 0;
DLL_PUBLIC FILE * stdout = 0;
DLL_PUBLIC FILE * stderr = 0;

static void* dlsym(void* lib, char* name) {
	void* ret = __dlsym(lib, name);
	if (ret) { return ret; }
	__printf("couldn't find symbol %s\n", name);
	__exit(-1);
	return 0;
}

__attribute__((constructor)) void init() {
	__printf("hello\n"); void* libc = 0;
	if (!libc) {
		libc = __dlopen("msvcrt.dll");
		isWin = true;
	}
	if (!libc) {
		libc = __dlopen("libc.so.6");
		isWin = false;
	}
	if (!libc) {
		__builtin_trap();
	}

	if (isWin) {
		vfprintf_ms = dlsym(libc, "vfprintf");
		snprintf_ms = dlsym(libc, "snprintf");
		malloc_ms = dlsym(libc, "malloc");
		calloc_ms = dlsym(libc, "calloc");
		aligned_alloc_ms = dlsym(libc, "_aligne_malloc");
		aligned_free_ms = dlsym(libc, "_aligne_free");
		realloc_ms = dlsym(libc, "realloc");
		free_ms = dlsym(libc, "free");
		fflush_ms = dlsym(libc, "fflush");
		memset_ms = dlsym(libc, "memset");
		memcpy_ms = dlsym(libc, "memcpy");
		getenv_ms = dlsym(libc, "getenv");
		thrd_yield_ms = dlsym(libc, "thrd_yield");
		mtx_init_ms = dlsym(libc, "mtx_init");
		mtx_lock_ms = dlsym(libc, "mtx_lock");
		mtx_unlock_ms = dlsym(libc, "mtx_unlock");
		fwrite_ms = dlsym(libc, "fwrite");
		remove_ms = dlsym(libc, "remove");
		void* (*fdopen)(int, char*) __attribute((ms_abi)) = dlsym(libc, "getc");
		stdin = fdopen( 0, "r" );
		stdout = fdopen( 1, "a" );
		stderr = fdopen( 2, "a" );
	} else {
		vfprintf_sysv = dlsym(libc, "vfprintf");
		snprintf_sysv = dlsym(libc, "snprintf");
		malloc_sysv = dlsym(libc, "malloc");
		calloc_sysv = dlsym(libc, "calloc");
		aligned_alloc_sysv = dlsym(libc, "aligned_alloc");
		aligned_free_sysv = dlsym(libc, "free");
		realloc_sysv = dlsym(libc, "realloc");
		free_sysv = dlsym(libc, "free");
		fflush_sysv = dlsym(libc, "fflush");
		memcpy_sysv = dlsym(libc, "memcpy");
		getenv_sysv = dlsym(libc, "getenv");
		thrd_yield_sysv = dlsym(libc, "thrd_yield");
		mtx_init_sysv = dlsym(libc, "mtx_init");
		mtx_lock_sysv = dlsym(libc, "mtx_lock");
		mtx_unlock_sysv = dlsym(libc, "mtx_unlock");
		fwrite_sysv = dlsym(libc, "fwrite");
		remove_sysv = dlsym(libc, "remove");
		stdin = dlsym(libc, "_IO_2_1_stdin_");
		stdout = dlsym(libc, "_IO_2_1_stdout_");
		stderr = dlsym(libc, "_IO_2_1_stderr_");
	}
	//external_linux_c_dprintf(1, "setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
	//printf("setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
}

DLL_PUBLIC
char getc(FILE* file) {
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
int vfprintf(FILE* stream, const char *restrict format, va_list argp) {
	if (isWin) {
		return vfprintf_ms(stream, format, argp);
	} else {
		return vfprintf_sysv(stream, format, argp);
	}
}

DLL_PUBLIC
int snprintf(char * s, size_t n, const char *restrict format, ...) {
    	va_list argp;
    	va_start(argp, format);
	if (isWin) {
		return snprintf_ms(s, n, format, argp);
	} else {
		return snprintf_sysv(s, n, format, argp);
	}
	va_end(argp);
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

DLL_PUBLIC void *calloc(size_t nmemb, size_t size) IMPLEMENT(calloc, nmemb, size)

DLL_PUBLIC
void* aligned_alloc(size_t alignment, size_t new_size) {
	if (isWin) {
		return aligned_alloc_ms(new_size, alignment);
	} else {
		return aligned_alloc_sysv(alignment, new_size);
	}
}

DLL_PUBLIC
void aligned_free(void* ptr) {
	if (isWin) {
		return aligned_free_ms(ptr);
	} else {
		return free_sysv(ptr);
	}
}

DLL_PUBLIC
void *realloc(void *ptr, size_t size) {
	if (isWin) {
		return realloc_ms(ptr, size);
	} else {
		return realloc_sysv(ptr, size);
	}
}

DLL_PUBLIC
int fprintf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    const int ret = vfprintf(stream, format, args);
    va_end(args);
    return ret;
}

DLL_PUBLIC
int fflush(FILE* file) {
	if (isWin) {
		return fflush_ms(file);
	} else {
		return fflush_sysv(file);
	}
}

DLL_PUBLIC int remove(const char *pathname) IMPLEMENT(remove, pathname)

DLL_PUBLIC
size_t fwrite(const void * ptr, size_t size, size_t nmemb, FILE *restrict stream) {
	if (isWin) {
		return fwrite_ms(ptr, size, nmemb, stream);
	} else {
		return fwrite_sysv(ptr, size, nmemb, stream);
	}
}

DLL_PUBLIC
void *memset(void *ptr, int value, size_t num) {
	if (isWin) {
		return memset_ms(ptr, value, num);
	} else {
		return memset_sysv(ptr, value, num);
	}
}

DLL_PUBLIC
void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
	if (isWin) {
		return memcpy_ms(dest, src, n);
	} else {
		return memcpy_sysv(dest, src, n);
	}
}

DLL_PUBLIC
char *getenv(const char *name) {
	if (isWin) {
		return getenv_ms(name);
	} else {
		return getenv_sysv(name);
	}
}

DLL_PUBLIC
void thrd_yield(void) {
	if (isWin) {
		return thrd_yield_ms();
	} else {
		return thrd_yield_sysv();
	}
}

DLL_PUBLIC
int mtx_init(mtx_t* mutex, int type) {
	if (isWin) {
		return mtx_init_ms(mutex, type);
	} else {
		return mtx_init_sysv(mutex, type);
	}
}

DLL_PUBLIC
int mtx_lock(mtx_t* mutex) {
	if (isWin) {
		return mtx_lock_ms(mutex);
	} else {
		return mtx_lock_sysv(mutex);
	}
}

DLL_PUBLIC
int mtx_unlock(mtx_t* mutex) {
	if (isWin) {
		return mtx_unlock_ms(mutex);
	} else {
		return mtx_unlock_sysv(mutex);
	}
}

DLL_PUBLIC
size_t strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s) {}
    return (s - str);
}

DLL_PUBLIC
int strncmp(const char* str1, const char* str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] == '\0' || str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
    }
    return 0;
}

DLL_PUBLIC
wchar_t* wcschr(const wchar_t* str, wchar_t wc) {
    while (*str) {
        if (*str == wc) {
            return (wchar_t*)str;
        }
        str++;
    }
    if (*str == wc) {
        return (wchar_t*)str;
    }
    return NULL;
}

DLL_PUBLIC
wchar_t* wcspbrk(const wchar_t* str, const wchar_t* chars) {
    while (*str) {
        const wchar_t* cp = chars;
        while (*cp) {
            if (*cp == *str) {
                return (wchar_t*)str;
            }
            cp++;
        }
        str++;
    }
    return NULL;
}

DLL_PUBLIC
wchar_t* wcsstr(const wchar_t* str, const wchar_t* substr) {
    if (*substr == '\0') {
        return (wchar_t*) str;
    }

    while (*str) {
        const wchar_t* s1 = str;
        const wchar_t* s2 = substr;

        while (*s1 && *s2 && (*s1 == *s2)) {
            s1++;
            s2++;
        }

        if (*s2 == '\0') {
            return (wchar_t*) str;
        }

        str++;
    }

    return NULL;
}

DLL_PUBLIC
wchar_t* wcsrchr(const wchar_t* str, wchar_t wc) {
    const wchar_t* last = NULL;
    while (*str) {
        if (*str == wc) {
            last = str;
        }
        str++;
    }
    if (*str == wc) {
        last = str;
    }
    return (wchar_t*) last;
}

DLL_PUBLIC
wchar_t* wmemchr(const wchar_t* ptr, wchar_t wc, size_t num) {
    while (num-- > 0) {
        if (*ptr == wc) {
            return (wchar_t*)ptr;
        }
        ptr++;
    }
    return NULL;
}

DLL_PUBLIC
void *memmove(void *dest, const void *src, size_t n) {
    char *dst_ptr = (char*) dest;
    const char *src_ptr = (const char*) src;

    // If source and destination regions overlap, use a temporary buffer
    if (src_ptr < dst_ptr && dst_ptr < src_ptr + n) {
        char *tmp = (char*) malloc(n);
        if (tmp == NULL) {
            return NULL; // Out of memory
        }

        // Copy source to tmp buffer
        for (size_t i = 0; i < n; i++) {
            *(tmp + i) = *(src_ptr + i);
        }

        // Copy tmp buffer to destination
        for (size_t i = 0; i < n; i++) {
            *(dst_ptr + i) = *(tmp + i);
        }

        free(tmp);
    } else {
        // No overlap, copy directly from source to destination
        for (size_t i = 0; i < n; i++) {
            *(dst_ptr + i) = *(src_ptr + i);
        }
    }

    return dest;
}

DLL_PUBLIC
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

DLL_PUBLIC
int dl_iterate_phdr() {
	return 0;
}

DLL_PUBLIC
int isdigit(int c) {
    if (c >= '0' && c <= '9') {
        return 1;
    } else {
        return 0;
    }
}

DLL_PUBLIC
int isxdigit(int c) {
    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
        (c >= 'A' && c <= 'F')) {
        return 1;
    } else {
        return 0;
    }
}

int memcmp(const void* ptr1, const void* ptr2, size_t size) {
    const unsigned char* p1 = (const unsigned char*) ptr1;
    const unsigned char* p2 = (const unsigned char*) ptr2;
    for (size_t i = 0; i < size; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }
    return 0;
}

void abort() {
    __builtin_trap();
}
