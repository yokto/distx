#include <stdarg.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <threads.h>
#include <error.h>
#include <stdio.h>
#include <limits.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>

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
DECLARE(int, vsnprintf, char * s, size_t n, const char * format, va_list arg)
DECLARE(int, vswprintf, wchar_t * ws, size_t len, const wchar_t * format, va_list arg);
DECLARE(int, vasprintf, char ** strp, const char * format, va_list arg )
DECLARE(int, vsscanf, const char *s, const char *format, va_list arg)
DECLARE(void*, malloc, size_t new_size)
DECLARE(void*, calloc, size_t nmemb, size_t size)
DECLARE(void*, aligned_alloc, size_t alignment, size_t size); // strictly speaking ms version has exchanged arguments
DECLARE(void*, realloc, void *ptr, size_t size)
DECLARE(void, free, void* ptr)
DECLARE(void, aligned_free, void *ptr);
DECLARE(void *, memset, void *ptr, int value, size_t num);
DECLARE(void *, memcpy, void *restrict dest, const void *restrict src, size_t n);
DECLARE(char, getc, FILE* file)
DECLARE(int, ungetc, int c, FILE *stream)
DECLARE(int, fflush, FILE* file)
DECLARE(char *, getenv, const char *name)
DECLARE(void, thrd_yield, void)
DECLARE(int, cnd_broadcast, cnd_t *cond )
DECLARE(int, cnd_wait, cnd_t* cond, mtx_t* mutex )
DECLARE(void, call_once, once_flag* flag, void (*func)(void) );
DECLARE(int, tss_create, tss_t* tss_key, tss_dtor_t destructor)
DECLARE(void *, tss_get, tss_t tss_key );
DECLARE(int, tss_set, tss_t tss_id, void *val );
DECLARE(int, mtx_init, mtx_t* mutex, int type)
DECLARE(int, mtx_unlock, mtx_t *mutex)
DECLARE(int, mtx_lock, mtx_t* mutex)
DECLARE(size_t, fwrite, const void * ptr, size_t size, size_t nmemb, FILE * stream)
DECLARE(FILE*, fopen, const char *filename, const char *mode)
DECLARE(FILE*, fdopen, int fd, const char *mode)
DECLARE(int, fclose, FILE* stream)
DECLARE(long, ftell, FILE* stream)
DECLARE(int, fseek, FILE *stream, long int offset, int whence)
DECLARE(size_t, fread, void * ptr, size_t size, size_t nmemb, FILE * stream)
DECLARE(int, fputc, int c, FILE *stream)
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
		vsnprintf_ms = dlsym(libc, "vsnprintf");
		vswprintf_ms = dlsym(libc, "vswprintf");
		vasprintf_ms = dlsym(libc, "vasprintf");
		vsscanf_ms = dlsym(libc, "vsscanf");
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
		cnd_broadcast_ms = dlsym(libc, "cnd_broadcast");
		cnd_wait_ms = dlsym(libc, "cnd_wait");
		call_once_ms = dlsym(libc, "call_once");
		tss_create_ms = dlsym(libc, "tss_create");
		tss_get_ms = dlsym(libc, "tss_get");
		tss_set_ms = dlsym(libc, "tss_set");
		mtx_init_ms = dlsym(libc, "mtx_init");
		mtx_lock_ms = dlsym(libc, "mtx_lock");
		mtx_unlock_ms = dlsym(libc, "mtx_unlock");
		fwrite_ms = dlsym(libc, "fwrite");
		fopen_ms = dlsym(libc, "fopen");
		fdopen_ms = dlsym(libc, "_fdopen");
		fclose_ms = dlsym(libc, "fclose");
		ftell_ms = dlsym(libc, "ftell");
		fseek_ms = dlsym(libc, "fseek");
		fread_ms = dlsym(libc, "fread");
		fputc_ms = dlsym(libc, "fputc");
		remove_ms = dlsym(libc, "remove");
		getc_ms = dlsym(libc, "getc");
		ungetc_ms = dlsym(libc, "ungetc");
		stdin = fdopen( 0, "r" );
		stdout = fdopen( 1, "a" );
		stderr = fdopen( 2, "a" );
	} else {
		vfprintf_sysv = dlsym(libc, "vfprintf");
		snprintf_sysv = dlsym(libc, "snprintf");
		vsnprintf_sysv = dlsym(libc, "vsnprintf");
		vswprintf_sysv = dlsym(libc, "vswprintf");
		vasprintf_sysv = dlsym(libc, "vasprintf");
		vsscanf_sysv = dlsym(libc, "vsscanf");
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
		cnd_broadcast_sysv = dlsym(libc, "cnd_broadcast");
		cnd_wait_sysv = dlsym(libc, "cnd_wait");
		call_once_sysv = dlsym(libc, "call_once");
		tss_create_sysv = dlsym(libc, "tss_create");
		tss_get_sysv = dlsym(libc, "tss_get");
		tss_set_sysv = dlsym(libc, "tss_set");
		mtx_init_sysv = dlsym(libc, "mtx_init");
		mtx_lock_sysv = dlsym(libc, "mtx_lock");
		mtx_unlock_sysv = dlsym(libc, "mtx_unlock");
		fwrite_sysv = dlsym(libc, "fwrite");
		fopen_sysv = dlsym(libc, "fopen");
		fdopen_ms = dlsym(libc, "fdopen");
		fclose_sysv = dlsym(libc, "fclose");
		ftell_sysv = dlsym(libc, "ftell");
		fseek_sysv = dlsym(libc, "fseek");
		fread_sysv = dlsym(libc, "fread");
		fputc_sysv = dlsym(libc, "fputc");
		remove_sysv = dlsym(libc, "remove");
		getc_sysv = dlsym(libc, "getc");
		ungetc_sysv = dlsym(libc, "ungetc");
		stdin = dlsym(libc, "_IO_2_1_stdin_");
		stdout = dlsym(libc, "_IO_2_1_stdout_");
		stderr = dlsym(libc, "_IO_2_1_stderr_");
	}
	//external_linux_c_dprintf(1, "setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
	//printf("setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
}

DLL_PUBLIC
char getc(FILE* file) IMPLEMENT(getc, file)

DLL_PUBLIC
int ungetc(int c, FILE *stream) IMPLEMENT(ungetc, c, stream)

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
int vsnprintf(char * s, size_t n, const char *restrict format, va_list argp) IMPLEMENT(vsnprintf, s, n, format, argp)

DLL_PUBLIC
int vswprintf(wchar_t * ws, size_t len, const wchar_t * format, va_list arg) IMPLEMENT(vswprintf, ws, len, format, arg)

DLL_PUBLIC
int swprintf(wchar_t* ws, size_t len, const wchar_t* format, ...) {
    va_list args;
    va_start(args, format);

    int result = vswprintf(ws, len, format, args);

    va_end(args);
    return result;
}

DLL_PUBLIC
int vasprintf(char ** strp, const char * format, va_list arg) IMPLEMENT(vasprintf, strp, format, arg)

DLL_PUBLIC
int vsscanf(const char *s, const char *format, va_list arg) IMPLEMENT(vsscanf, s, format, arg)

int sscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int result = vsscanf(str, format, args);

    va_end(args);
    return result;
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
FILE *fopen(const char *filename, const char *mode) IMPLEMENT(fopen, filename, mode)

DLL_PUBLIC
FILE *fdopen(int fd, const char *mode) IMPLEMENT(fdopen, fd, mode)

DLL_PUBLIC
int fclose(FILE *stream) IMPLEMENT(fclose, stream)

DLL_PUBLIC
long ftell(FILE* stream) IMPLEMENT(ftell, stream)

DLL_PUBLIC
int fseek(FILE *stream, long int offset, int whence) IMPLEMENT(fseek, stream, offset, whence)

DLL_PUBLIC
size_t fread(void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream) IMPLEMENT(fread, ptr, size, nmemb, stream)

DLL_PUBLIC
size_t fwrite(const void * ptr, size_t size, size_t nmemb, FILE *restrict stream) IMPLEMENT(fwrite, ptr, size, nmemb, stream)

DLL_PUBLIC
int fputc(int c, FILE *stream) IMPLEMENT(fputc, c, stream)

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

DLL_PUBLIC int cnd_broadcast( cnd_t *cond ) IMPLEMENT(cnd_broadcast, cond)
DLL_PUBLIC int cnd_wait( cnd_t* cond, mtx_t* mutex ) IMPLEMENT(cnd_wait, cond, mutex)
DLL_PUBLIC void call_once( once_flag* flag, void (*func)(void) ) IMPLEMENT(call_once, flag, func)
DLL_PUBLIC int tss_create(tss_t* tss_key, tss_dtor_t destructor) IMPLEMENT(tss_create, tss_key, destructor)
DLL_PUBLIC void *tss_get(tss_t tss_key) IMPLEMENT(tss_get, tss_key)
DLL_PUBLIC int tss_set(tss_t tss_id, void *val) IMPLEMENT(tss_set, tss_id, val)

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

DLL_PUBLIC
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

DLL_PUBLIC
void abort() {
    __builtin_trap();
}

DLL_PUBLIC
void __tls_get_addr() {
    fprintf(stderr, "__tls_get_addr not implemented");
    fflush(stderr);
    abort();
}

DLL_PUBLIC
void __cxa_atexit() {
    fprintf(stderr, "__tls_get_addr not implemented");
    fflush(stderr);
    abort();
}

DLL_PUBLIC
int fputs(const char *s, FILE *stream) {
    int len = strlen(s);
    size_t nwritten = fwrite(s, 1, len, stream);
    if ((int)nwritten != len) {
        return EOF;
    }
    return 0;
}

DLL_PUBLIC
void *memchr(const void *s, int c, size_t n) {
    const unsigned char *p = s;
    for (size_t i = 0; i < n; i++) {
        if (p[i] == (unsigned char)c) {
            return (void *)(p + i);
        }
    }
    return NULL;
}

#define UNUSED(x) (void)(x)
DLL_PUBLIC
char* strerror_r(int errnum, char* buf, size_t buflen) {
    UNUSED(errnum);
    UNUSED(buf);
    UNUSED(buflen);
    return "unknown error";
}

DLL_PUBLIC
locale_t uselocale(locale_t locale) {
	return locale;
}

DLL_PUBLIC
long int strtol(const char* str, char** endptr, int base) {
    // Check for valid base
    if (base < 0 || (base != 0 && (base < 2 || base > 36))) {
        errno = EINVAL;  // Invalid argument
        return 0;
    }

    // Handle optional leading whitespace
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) {
        str++;
    }

    // Handle optional sign
    int sign = 1;
    if (*str == '-' || *str == '+') {
        sign = (*str++ == '-') ? -1 : 1;
    }

    // Handle optional base prefix (0x or 0X for hexadecimal)
    if (base == 0) {
        if (*str == '0') {
            if (*(str + 1) == 'x' || *(str + 1) == 'X') {
                base = 16;
                str += 2;
            } else {
                base = 8;
                str++;
            }
        } else {
            base = 10;
        }
    }

    // Parse digits and calculate the result
    long int result = 0;
    int overflow = 0;
    while (*str != '\0') {
        int digit;
        if (*str >= '0' && *str <= '9') {
            digit = *str - '0';
        } else if (*str >= 'a' && *str <= 'z') {
            digit = *str - 'a' + 10;
        } else if (*str >= 'A' && *str <= 'Z') {
            digit = *str - 'A' + 10;
        } else {
            break;  // Invalid character
        }

        if (digit >= base) {
            break;  // Invalid digit for base
        }

        if (!overflow) {
            long int prev = result;
            result = result * base + digit;
            if (result / base != prev) {
                overflow = 1;
            }
        }

        str++;
    }

    // Set the end pointer if requested
    if (endptr != NULL) {
        *endptr = (char*)str;
    }

    // Handle overflow and underflow
    if (overflow) {
        errno = ERANGE;  // Result out of range
        return (sign == 1) ? LONG_MAX : LONG_MIN;
    }

    return result * sign;
}

DLL_PUBLIC
unsigned long int strtoul(const char* str, char** endptr, int base) {
    return (unsigned long int)strtol(str, endptr, base);
}

DLL_PUBLIC
long long int strtoll(const char* str, char** endptr, int base) {
    return (long long int)strtol(str, endptr, base);
}

DLL_PUBLIC
unsigned long long int strtoull(const char* str, char** endptr, int base) {
    return (unsigned long long int)strtol(str, endptr, base);
}

DLL_PUBLIC
int errno = 0;

DLL_PUBLIC
int isascii(int c) {
    return (c >= 0 && c <= 127);
}

DLL_PUBLIC
int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

DLL_PUBLIC
int isprint(int c) {
    return (c >= 32 && c < 127);
}

DLL_PUBLIC
int iscntrl(int c) {
    return (c >= 0 && c < 32) || c == 127;
}

DLL_PUBLIC
int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

DLL_PUBLIC
int islower(int c) {
    return c >= 'a' && c <= 'z';
}

DLL_PUBLIC
int isalpha(int c) {
    return isupper(c) || islower(c);
}

DLL_PUBLIC
int ispunct(int c) {
    return (c >= 33 && c <= 47) || (c >= 58 && c <= 64) ||
           (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

DLL_PUBLIC
int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

DLL_PUBLIC
int isgraph(int c) {
    return c >= 33 && c <= 126;
}

DLL_PUBLIC
int toupper(int c) {
    if (islower(c))
        return c - 32;
    else
        return c;
}

DLL_PUBLIC
int tolower(int c) {
    if (isupper(c))
        return c + 32;
    else
        return c;
}

DLL_PUBLIC
wint_t btowc(int c) {
    if (c == EOF)
        return WEOF;

    // Check if the character is a valid single-byte character
    if (c >= 0 && c <= UCHAR_MAX)
        return (wint_t)c;

    // Invalid character
    return WEOF;
}

DLL_PUBLIC
int wctob(wint_t wc) {
    if (wc == WEOF)
        return EOF;

    // Check if the wide character can be represented as a single-byte character
    if (wc <= UCHAR_MAX)
        return (int)wc;

    // Invalid wide character
    return EOF;
}

DLL_PUBLIC
long wcstol(const wchar_t *nptr, wchar_t **endptr, int base) {
    return wcstoll(nptr, endptr, base);
}

DLL_PUBLIC
unsigned long wcstoul(const wchar_t *nptr, wchar_t **endptr, int base) {
    return wcstoull(nptr, endptr, base);
}

DLL_PUBLIC
long long wcstoll(const wchar_t *nptr, wchar_t **endptr, int base) {
    const wchar_t *p = nptr;
    bool negative = false;
    long long result = 0;

    // Skip leading whitespace
    while (iswspace(*p))
        ++p;

    // Handle optional sign
    if (*p == L'-') {
        negative = true;
        ++p;
    } else if (*p == L'+') {
        ++p;
    }

    // Handle base prefix
    if ((base == 0 || base == 16) && p[0] == L'0' && p[1] == L'x') {
        p += 2;
        base = 16;
    } else if ((base == 0 || base == 8) && p[0] == L'0') {
        ++p;
        base = 8;
    } else if (base == 0) {
        base = 10;
    }

    // Convert digits
    for (; *p != L'\0'; ++p) {
        int digit;
        if (*p >= L'0' && *p <= L'9') {
            digit = *p - L'0';
        } else if (*p >= L'A' && *p <= L'Z') {
            digit = *p - L'A' + 10;
        } else if (*p >= L'a' && *p <= L'z') {
            digit = *p - L'a' + 10;
        } else {
            break; // Invalid character, stop parsing
        }

        if (digit >= base)
            break; // Invalid digit for the given base

        result = result * base + digit;
    }

    if (endptr != NULL)
        *endptr = (wchar_t *)p;

    return negative ? -result : result;
}

DLL_PUBLIC
unsigned long long wcstoull(const wchar_t *nptr, wchar_t **endptr, int base) {
    const wchar_t *p = nptr;
    bool overflow = false;
    unsigned long long result = 0;

    // Skip leading whitespace
    while (iswspace(*p))
        ++p;

    // Handle optional sign
    if (*p == L'+') {
        ++p;
    }

    // Handle base prefix
    if ((base == 0 || base == 16) && p[0] == L'0' && p[1] == L'x') {
        p += 2;
        base = 16;
    } else if ((base == 0 || base == 8) && p[0] == L'0') {
        ++p;
        base = 8;
    } else if (base == 0) {
        base = 10;
    }

    // Convert digits
    for (; *p != L'\0'; ++p) {
        int digit;
        if (*p >= L'0' && *p <= L'9') {
            digit = *p - L'0';
        } else if (*p >= L'A' && *p <= L'Z') {
            digit = *p - L'A' + 10;
        } else if (*p >= L'a' && *p <= L'z') {
            digit = *p - L'a' + 10;
        } else {
            break; // Invalid character, stop parsing
        }

        if (digit >= base)
            break; // Invalid digit for the given base

        // Check for overflow
        if (result > (ULLONG_MAX - digit) / base) {
            overflow = true;
            break;
        }

        result = result * base + digit;
    }

    if (endptr != NULL)
        *endptr = (wchar_t *)p;

    return overflow ? ULLONG_MAX : result;
}


DLL_PUBLIC
size_t wcsnrtombs(char *dest, const wchar_t **src, size_t nwc, size_t len, mbstate_t *ps) {
    size_t count = 0;
    const wchar_t *srcPtr = *src;

    if (dest == NULL || src == NULL || len == 0) {
        return (size_t)-1; // Invalid arguments
    }

    while (*srcPtr != L'\0' && nwc > 0 && len > 1) {
        size_t bytesWritten = wcrtomb(dest, *srcPtr, ps);
        if (bytesWritten == (size_t)-1) {
            return (size_t)-1; // Encoding error
        }

        count += bytesWritten;
        len -= bytesWritten;
        dest += bytesWritten;
        srcPtr++;
        nwc--;
    }

    *src = srcPtr; // Update the source pointer
    *dest = '\0'; // Null-terminate the destination string

    return count;
}

DLL_PUBLIC
size_t wcstombs(char *dest, const wchar_t *src, size_t len) {
    const wchar_t *srcPtr = src;
    size_t count = 0;

    if (dest == NULL || src == NULL || len == 0) {
        return (size_t)-1; // Invalid arguments
    }

    while (*srcPtr != L'\0' && len > 1) {
        size_t bytesWritten = wcrtomb(dest, *srcPtr, NULL);
        if (bytesWritten == (size_t)-1) {
            return (size_t)-1; // Encoding error
        }

        count += bytesWritten;
        len -= bytesWritten;
        dest += bytesWritten;
        srcPtr++;
    }

    *dest = '\0'; // Null-terminate the destination string

    if (len == 0) {
        return (size_t)-1; // Insufficient buffer size
    }

    return count;
}

DLL_PUBLIC
int wcrtomb(char *s, wchar_t wc, mbstate_t *ps) {
    fprintf(stderr, "wcrtomb not implemented %s %d %p", s, wc, ps);
    fflush(stderr);
    abort();
    return 0;
}

DLL_PUBLIC
int wctomb(char *s, wchar_t wc) {
    return wcrtomb(s, wc, NULL);
}

DLL_PUBLIC
int mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps) {
    UNUSED(pwc);
    UNUSED(s);
    UNUSED(n);
    UNUSED(ps);
    fprintf(stderr, "mbrtowc not implemented");
    fflush(stderr);
    abort();
    return 0;
}

DLL_PUBLIC
size_t mbsnrtowcs(wchar_t *dst, const char **src, size_t nms, size_t len, mbstate_t *ps) {
    UNUSED(dst);
    UNUSED(src);
    UNUSED(nms);
    UNUSED(len);
    UNUSED(ps);
    fprintf(stderr, "mbsnrtowcs not implemented");
    fflush(stderr);
    abort();
    return 0;
}

DLL_PUBLIC
int mbtowc(wchar_t *pwc, const char *s, size_t n) {
    static mbstate_t state = {0}; // Static conversion state for the current thread
    mbstate_t local_state = state; // Local conversion state

    if (s == NULL) {
        // Reset the conversion state
        memset(&state, 0, sizeof(mbstate_t));
        return 0;
    }

    if (n == 0) {
        // Incomplete multibyte character
        return -1;
    }

    int result = mbrtowc(pwc, s, n, &local_state);

    if (result >= 0) {
        // Character successfully converted
        if (pwc != NULL) {
            state = local_state; // Update the global conversion state
        }
    } else {
        // Invalid multibyte character or incomplete sequence
        memset(&state, 0, sizeof(mbstate_t)); // Reset the global conversion state
    }

    return result;
}

DLL_PUBLIC
size_t mbrlen(const char *s, size_t n, mbstate_t *ps) {
    static mbstate_t state = {0}; // Static conversion state for the current thread
    mbstate_t local_state = ps != NULL ? *ps : state; // Local conversion state

    if (s == NULL) {
        // Reset the conversion state
        memset(&state, 0, sizeof(mbstate_t));
        return 0;
    }

    if (n == 0) {
        // Incomplete multibyte character
        return (size_t)-1;
    }

    size_t result = 0;
    const char *p = s;

    while (n > 0) {
        wchar_t wc;
        int len = mbrtowc(&wc, p, n, &local_state);

        if (len > 0) {
            // Valid multibyte character
            p += len;
            n -= len;
            result += len;
        } else if (len == 0) {
            // Reached the end of the string
            break;
        } else {
            // Invalid multibyte character or incomplete sequence
            result = (size_t)-1;
            memset(&state, 0, sizeof(mbstate_t)); // Reset the global conversion state
            break;
        }
    }

    if (ps != NULL) {
        *ps = local_state; // Update the conversion state
    } else {
        state = local_state; // Update the global conversion state
    }

    return result;
}

DLL_PUBLIC
int iswspace(wint_t wc) {
    return (wc == L' ' || wc == L'\t' || wc == L'\n' || wc == L'\r' ||
            wc == L'\f' || wc == L'\v');
}

double pow(double base, double exponent) {
    double result = 1.0;

    if (exponent == 0.0) {
        return 1.0;
    } else if (exponent > 0.0) {
        for (int i = 0; i < exponent; ++i) {
            result *= base;
        }
    } else {
        for (int i = 0; i > exponent; --i) {
            result /= base;
        }
    }

    return result;
}

float powf(float base, float exponent) {
    float result = 1.0f;

    if (exponent == 0.0f) {
        return 1.0f;
    } else if (exponent > 0.0f) {
        for (int i = 0; i < exponent; ++i) {
            result *= base;
        }
    } else {
        for (int i = 0; i > exponent; --i) {
            result /= base;
        }
    }

    return result;
}

DLL_PUBLIC
float strtof(const char *nptr, char **endptr) {
    // Skip leading whitespace
    while (isspace(*nptr))
        ++nptr;

    // Handle optional sign
    bool negative = false;
    if (*nptr == '-') {
        negative = true;
        ++nptr;
    } else if (*nptr == '+') {
        ++nptr;
    }

    // Parse integer part
    float result = 0.0f;
    while (isdigit(*nptr)) {
        result = result * 10.0f + (*nptr - '0');
        ++nptr;
    }

    // Parse fraction part
    if (*nptr == '.') {
        float divisor = 10.0f;
        ++nptr;
        while (isdigit(*nptr)) {
            result += (*nptr - '0') / divisor;
            divisor *= 10.0f;
            ++nptr;
        }
    }

    // Handle exponent part
    if (toupper(*nptr) == 'E') {
        int exponent = 0;
        int exponentSign = 1;
        ++nptr;
        if (*nptr == '-') {
            exponentSign = -1;
            ++nptr;
        } else if (*nptr == '+') {
            ++nptr;
        }
        while (isdigit(*nptr)) {
            exponent = exponent * 10 + (*nptr - '0');
            ++nptr;
        }
        result *= powf(10.0f, exponent * exponentSign);
    }

    // Set end pointer if requested
    if (endptr != NULL)
        *endptr = (char *)nptr;

    // Apply sign
    if (negative)
        result = -result;

    return result;
}

DLL_PUBLIC
double strtod(const char *nptr, char **endptr) {
    return (double)strtof(nptr, endptr);
}

DLL_PUBLIC
long double strtold (const char* nptr, char** endptr) {
    return (double)strtof(nptr, endptr);
}

DLL_PUBLIC
double wcstod(const wchar_t *nptr, wchar_t **endptr) {
    // Skip leading whitespace
    while (iswspace(*nptr))
        ++nptr;

    // Handle optional sign
    bool negative = false;
    if (*nptr == L'-') {
        negative = true;
        ++nptr;
    } else if (*nptr == L'+') {
        ++nptr;
    }

    // Parse integer part
    double result = 0.0;
    while (iswdigit(*nptr)) {
        result = result * 10.0 + (*nptr - L'0');
        ++nptr;
    }

    // Parse fraction part
    if (*nptr == L'.') {
        double divisor = 10.0;
        ++nptr;
        while (iswdigit(*nptr)) {
            result += (*nptr - L'0') / divisor;
            divisor *= 10.0;
            ++nptr;
        }
    }

    // Handle exponent part
    if (towupper(*nptr) == L'E') {
        int exponent = 0;
        int exponentSign = 1;
        ++nptr;
        if (*nptr == L'-') {
            exponentSign = -1;
            ++nptr;
        } else if (*nptr == L'+') {
            ++nptr;
        }
        while (iswdigit(*nptr)) {
            exponent = exponent * 10 + (*nptr - L'0');
            ++nptr;
        }
        result *= pow(10.0, exponent * exponentSign);
    }

    // Set end pointer if requested
    if (endptr != NULL)
        *endptr = (wchar_t *)nptr;

    // Apply sign
    if (negative)
        result = -result;

    return result;
}

DLL_PUBLIC
float wcstof(const wchar_t *nptr, wchar_t **endptr) {
    return (float)wcstod(nptr, endptr);
}

DLL_PUBLIC
long double wcstold(const wchar_t *nptr, wchar_t **endptr) {
    return wcstod(nptr, endptr);
}


DLL_PUBLIC
int iswdigit(wint_t wc) {
    return (wc >= L'0' && wc <= L'9');
}

DLL_PUBLIC
wint_t towupper(wint_t wc) {
    if (wc >= L'a' && wc <= L'z')
        return wc - (L'a' - L'A');
    return wc;
}	

DLL_PUBLIC
size_t mbsrtowcs(wchar_t *dest, const char **src, size_t len, mbstate_t *ps) {
    if (dest == NULL || src == NULL || *src == NULL)
        return (size_t)-1; // Invalid arguments

    mbstate_t state;
    if (ps == NULL)
        ps = &state;
    memset(ps, 0, sizeof(mbstate_t)); // Initialize the conversion state

    size_t count = 0; // Number of wide characters converted
    size_t dest_len = 0; // Length of the wide character string
    const char *src_ptr = *src;

    // Iterate over the multibyte characters and convert them to wide characters
    while (len != 0) {
        wchar_t wc;
        size_t result = mbrtowc(&wc, src_ptr, len, ps);

        if (result == (size_t)-1) {
            // Invalid multibyte character
            errno = EILSEQ;
            return (size_t)-1;
        } else if (result == (size_t)-2) {
            // Incomplete multibyte character
            errno = EINVAL;
            return (size_t)-1;
        } else if (result == 0) {
            // Reached the end of the multibyte string
            break;
        } else {
            // Valid multibyte character
            ++count;

            if (dest != NULL) {
                if (dest_len >= (size_t)INT_MAX || dest_len + 1 >= len)
                    return (size_t)-1; // Insufficient buffer size

                *dest++ = wc;
            }

            src_ptr += result;
            len -= result;
            dest_len++;
        }
    }

    *src = src_ptr; // Update the source pointer

    if (dest != NULL)
        *dest = L'\0'; // Null-terminate the wide character string

    return count;
}

DLL_PUBLIC
int strcoll(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) {
            // Convert characters to unsigned char to handle locale-specific behavior
            unsigned char c1 = (unsigned char)*s1;
            unsigned char c2 = (unsigned char)*s2;

            if (tolower(c1) != tolower(c2)) {
                // Characters are not equal, return their comparison result
                return tolower(c1) - tolower(c2);
            }
        }
        ++s1;
        ++s2;
    }

    // One or both strings have ended, compare their lengths
    return strlen(s1) - strlen(s2);
}