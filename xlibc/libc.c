#include <stdarg.h>
#include <zwolf.h>
#include <base/fs.h>
#include <proc.h>
#include <dirent.h>
#include <locale.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <dlfcn.h>
#include <sys/wait.h>
#include <unicode.h>
#include <stddef.h>
#include <string.h>
#include <threads.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include <common.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <fs.h>
#include <thread_impl.h>
#include <unistd.h>

//#define FENCE 1

#define SUCCESS 0

DLL_PUBLIC
__thread int errno = 0;

DLL_PUBLIC
char ** environ = 0;

#define DECLARE(ret, name, ...) \
	ret name(__VA_ARGS__); \
	static ret (*concat(name, _ms))(__VA_ARGS__) __attribute((ms_abi)); \
	static ret (*concat(name, _sysv))(__VA_ARGS__); 

#define IMPLEMENT(name, ...) \
{ \
	debug_printf("execute os %s\n", #name); \
	if (isWin) { \
		return concat(name, _ms)(__VA_ARGS__); \
	} else { \
		return concat(name, _sysv)(__VA_ARGS__); \
	} \
}

DLL_PUBLIC
void abort() {
    __builtin_trap();
}

typedef uint64_t linux_dev_t;
typedef uint64_t linux_ino_t;
typedef uint32_t linux_mode_t;
typedef uint64_t linux_nlink_t;
typedef uint32_t linux_uid_t;
typedef uint32_t linux_gid_t;
typedef uint64_t linux_dev_t;
typedef uint64_t linux_off_t;
typedef uint64_t linux_blksize_t;
typedef uint64_t linux_blkcnt_t;
struct linux_stat {
               linux_dev_t     st_dev;         /* ID of device containing file */
               linux_ino_t     st_ino;         /* Inode number */
               linux_nlink_t   st_nlink;       /* Number of hard links */
               linux_mode_t    st_mode;        /* File type and mode */
               linux_uid_t     st_uid;         /* User ID of owner */
               linux_gid_t     st_gid;         /* Group ID of owner */
	       int pad0;

               linux_dev_t     st_rdev;        /* Device ID (if special file) */
               linux_off_t     st_size;        /* Total size, in bytes */
               linux_blksize_t st_blksize;     /* Block size for filesystem I/O */
               linux_blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

               /* Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES. */

               struct timespec st_atim;  /* Time of last access */
               struct timespec st_mtim;  /* Time of last modification */
               struct timespec st_ctim;  /* Time of last status change */
	       char padding[144];
};
typedef uint32_t windows_dev_t;
typedef uint16_t windows_ino_t;
typedef uint32_t windows_dev_t;
typedef uint32_t windows_off_t;
typedef uint64_t windowstime_t;
struct windows_stat {
    windows_dev_t st_dev;         // Device ID of the file
    windows_ino_t st_ino;         // Inode number
    unsigned short st_mode; // File mode (permissions and file type)
    short st_nlink;        // Number of hard links
    short st_uid;          // User ID of the file's owner
    short st_gid;          // Group ID of the file's owner
    windows_dev_t st_rdev;        // Device ID (if file is a special file)
    windows_off_t st_size;        // File size in bytes
    windowstime_t wst_atime;       // Last access time
    windowstime_t wst_mtime;       // Last modification time
    windowstime_t wst_ctime;       // Last status change time
    char padding[48];
};



//void print_backtrace();

static bool isWin = false;

DECLARE(int, vswprintf, wchar_t * ws, size_t len, const wchar_t * format, va_list arg);
DECLARE(void*, malloc, size_t new_size)
DECLARE(void*, calloc, size_t nmemb, size_t size)
DECLARE(void*, aligned_alloc, size_t alignment, size_t size); // strictly speaking ms version has exchanged arguments
DECLARE(void*, realloc, void *ptr, size_t size)
DECLARE(void, free, void* ptr)
DECLARE(void, aligned_free, void *ptr);
DECLARE(int, fflush, FILE* file)
DECLARE(size_t, fwrite, const void * ptr, size_t size, size_t nmemb, FILE * stream)
static FILE* (*_wfopen_ms)(const uint16_t *filename, const uint16_t *mode) __attribute((ms_abi)); 
static FILE* (*fopen_sysv)(const char *filename, const char *mode);
void *(*mmap_sysv)(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int (*munmap_sysv)(void *addr, size_t length);
DECLARE(FILE*, fdopen, int fd, const char *mode)
DECLARE(int, fclose, FILE* stream)
DECLARE(long, ftell, FILE* stream)
DECLARE(int, fseek, FILE *stream, long int offset, int whence)
DECLARE(size_t, fread, void * ptr, size_t size, size_t nmemb, FILE * stream)
DECLARE(int, remove, const char *pathname)
int (*_rmdir_ms)(const char *pathname) __attribute((ms_abi));
//DECLARE(int, cnd_init, cnd_t* cond)
//DECLARE(int, cnd_broadcast, cnd_t *cond )
//DECLARE(int, cnd_wait, cnd_t* cond, mtx_t* mutex )
//DECLARE(int, cnd_signal, cnd_t *cond );
//DECLARE(void, cnd_destroy, cnd_t* cond );
//DECLARE(int, cnd_timedwait, cnd_t*  cond, mtx_t*  mutex, const struct timespec*  time_point );
//bool (*SleepConditionVariableCS)(cnd_t* cond, mtx_t*, uint32_t) __attribute((ms_abi));
//DECLARE(int, mtx_init, mtx_t* mutex, int type)
//DECLARE(int, mtx_unlock, mtx_t *mutex)
//DECLARE(int, mtx_lock, mtx_t* mutex)
//DECLARE(int, mtx_trylock, mtx_t *mutex );
//DECLARE(void, mtx_destroy, mtx_t *mutex );
DECLARE(int, clock_gettime, clockid_t clockid, struct timespec *tp);
DECLARE(int, getentropy, void *buffer, size_t length);
DECLARE(char*, realpath, const char * path, char * resolved_path);
uint16_t * (*_wfullpath_ms)(uint16_t *absPath, const uint16_t *relPath, size_t maxLength) __attribute((ms_abi));
DECLARE(int, mkdir, const char *pathname, mode_t mode);
int (*_mkdir_ms)(const char *pathname) __attribute((ms_abi));
DECLARE(int, stat, const char * pathname, struct stat * statbuf);
int (*_wstat_ms)(uint16_t * pathname, struct stat * statbuf) __attribute((ms_abi));
DECLARE(int, fstat, int fd, struct stat * statbuf);
char* (*getcwd_sysv)(char *buf, size_t size);
uint16_t* (*wgetcwd_ms)(uint16_t * path, size_t size) __attribute((ms_abi));
DECLARE(struct dirent*, readdir, DIR * dir);
DECLARE(int, closedir, DIR* dir);
DECLARE(int, chdir, const char *path);
DECLARE(DIR*, opendir, const char * path);
#define MAX_WIN_PATH 260
typedef struct WIN32_FIND_DATAW {
	char somedata[44];
	uint16_t name[MAX_WIN_PATH];
	char restdata[16];
} WIN32_FIND_DATAW;
int32_t (*FindFirstFileW_ms)(uint16_t * path, WIN32_FIND_DATAW* lpFindFileData) __attribute((ms_abi));
bool (*FindNextFileW_ms)(int32_t hFindFile, WIN32_FIND_DATAW* lpFindFileData) __attribute((ms_abi));
DECLARE(ssize_t, readlink, const char * pathname, char * buf, size_t bufsiz);
DECLARE(int, rename, const char *oldpath, const char *newpath);
DECLARE(int, truncate, const char *path, off_t length);
DECLARE(int, statvfs, const char * path, struct statvfs * buf);
DECLARE(int, lstat, const char * pathname, struct stat * statbuf);

DECLARE(void, tss_delete, tss_t tss_id )
DECLARE(void *, tss_get, tss_t tss_key )
DECLARE(int, tss_set, tss_t tss_id, void *val )
int tss_create(tss_t* tss_key, tss_dtor_t destructor);
static int (*tss_create_sysv)(tss_t* tss_key, tss_dtor_t destructor);
static uint32_t (*tss_create_ms)() __attribute((ms_abi));

// THRD
DECLARE(void, thrd_yield, void)
DECLARE(thrd_t, thrd_current, void);
DECLARE(int, thrd_detach, thrd_t thr );
DECLARE(int, thrd_equal, thrd_t lhs, thrd_t rhs );
static uint32_t (*GetThreadId_ms)(thrd_t thr) __attribute((ms_abi));

DECLARE(int, thrd_join, thrd_t thr, int *res );
static uint32_t (*thrd_join_wait_ms)(int handle, uint32_t ms) __attribute((ms_abi));

int thrd_sleep(const struct timespec* duration, struct timespec* remaining);
static int (*thrd_sleep_sysv)(const struct timespec* duration, struct timespec* remaining);
static uint32_t (*thrd_sleep_ms)(uint32_t dwMilliseconds, bool bAlertable) __attribute((ms_abi));

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
static int (*thrd_create_sysv)(thrd_t *thr, thrd_start_t func, void *arg);
static uint64_t (*thrd_create_ms)(
		void* lpThreadAttributes,
		size_t dwStackSize,
		int (*lpStartAddress)(void *) __attribute__((ms_abi)), 
		void* lpParameter,
		uint32_t dwCreationFlags,
		uint32_t* lpThreadId) __attribute((ms_abi));
/// TRD END



DLL_PUBLIC FILE * stdin = 0;
DLL_PUBLIC FILE * stdout = 0;
DLL_PUBLIC FILE * stderr = 0;

static void* dlsym(void* lib, char* name) {
	void* ret = zwolf_sym(lib, name);
	if (ret) { return ret; }
	debug_printf("couldn't find symbol %s\n", name);
	__builtin_trap();
	return 0;
}

__attribute__((constructor)) void init() {
	debug_printf("init inside %s\n", "xlibc");
	void* libc = 0;
	void* kernel32 = 0;
	if (!libc) {
		libc = zwolf_open("msvcrt.dll");
		kernel32 = zwolf_open("KERNEL32.DLL");
		isWin = true;
	}
	if (!libc) {
		libc = zwolf_open("libc.so.6");
		isWin = false;
	}
	if (!libc) {
		__builtin_trap();
	}
	if (isWin && kernel32 == 0) { 
		debug_printf("couldn't open kernel32\n");
		__builtin_trap();
	}

	init_fs(isWin, libc);
	init_threads(isWin, libc);
	init_proc(isWin, libc, kernel32);
	if (isWin) {
		vswprintf_ms = dlsym(libc, "vswprintf");
		malloc_ms = dlsym(libc, "malloc");
		calloc_ms = dlsym(libc, "calloc");
		aligned_alloc_ms = dlsym(libc, "_aligned_malloc");
		aligned_free_ms = dlsym(libc, "_aligned_free");
		realloc_ms = dlsym(libc, "realloc");
		free_ms = dlsym(libc, "free");
		fflush_ms = dlsym(libc, "fflush");
		thrd_yield_ms = dlsym(kernel32, "SwitchToThread");
		thrd_current_ms = dlsym(kernel32, "GetCurrentThread");
		thrd_sleep_ms = dlsym(kernel32, "SleepEx");
		tss_create_ms = dlsym(kernel32, "TlsAlloc");
		tss_delete_ms = dlsym(kernel32, "TlsFree");
		tss_get_ms = dlsym(kernel32, "TlsGetValue");
		tss_set_ms = dlsym(kernel32, "TlsSetValue");
		fwrite_ms = dlsym(libc, "fwrite");
		_wfopen_ms = dlsym(libc, "_wfopen");
		fdopen_ms = dlsym(libc, "_fdopen");
		fclose_ms = dlsym(libc, "fclose");
		ftell_ms = dlsym(libc, "ftell");
		fseek_ms = dlsym(libc, "fseek");
		fread_ms = dlsym(libc, "fread");
		remove_ms = dlsym(libc, "remove");
		_rmdir_ms = dlsym(libc, "_rmdir");
		//cnd_init_ms = dlsym(kernel32, "InitializeConditionVariable");
		//cnd_broadcast_ms = dlsym(kernel32, "WakeAllConditionVariable");
		//SleepConditionVariableCS = dlsym(kernel32, "SleepConditionVariableCS");
		//cnd_wait_ms = 0;
		//cnd_signal_ms = dlsym(kernel32, "WakeConditionVariable");
		//cnd_destroy_ms = 0; // not needed
		//cnd_timedwait_ms = 0;
//		mtx_init_ms = dlsym(kernel32, "InitializeCriticalSection");
//		mtx_lock_ms = dlsym(kernel32, "EnterCriticalSection");
//		mtx_unlock_ms = dlsym(kernel32, "LeaveCriticalSection");
//		mtx_trylock_ms = dlsym(kernel32, "TryEnterCriticalSection");
//		mtx_destroy_ms = dlsym(kernel32, "DeleteCriticalSection");
		thrd_equal_ms = 0;
		GetThreadId_ms = dlsym(kernel32, "GetThreadId");
		clock_gettime_ms = 0; //dlsym(libc, "clock_gettime");
		getentropy_ms = 0; //dlsym(libc, "getentropy");
		realpath_ms = 0; //dlsym(libc, "realpath");
		_wfullpath_ms = dlsym(libc, "_wfullpath");
		mkdir_ms = 0;
		_mkdir_ms = dlsym(libc, "_mkdir");
		stat_ms = dlsym(libc, "_stat");
		_wstat_ms = dlsym(libc, "_wstat");
		fstat_ms = dlsym(libc, "_fstat");
		wgetcwd_ms = dlsym(libc, "_wgetcwd");
		readdir_ms = 0; //dlsym(libc, "readdir");
		closedir_ms = 0; //dlsym(libc, "closedir");
		chdir_ms = 0; //dlsym(libc, "chdir");
		opendir_ms = 0; //dlsym(libc, "opendir");
		FindFirstFileW_ms = dlsym(kernel32, "FindFirstFileW");
		FindNextFileW_ms = dlsym(kernel32, "FindNextFileW");
		readlink_ms = 0; //dlsym(libc, "readlink");
		rename_ms = dlsym(libc, "rename");
		truncate_ms = 0; //dlsym(libc, "truncate");
		statvfs_ms = 0; //dlsym(libc, "statvfs");
		lstat_ms = 0; //dlsym(libc, "lstat");
		thrd_join_ms = dlsym(kernel32, "GetExitCodeThread");
		thrd_join_wait_ms = dlsym(kernel32, "WaitForSingleObject");
		thrd_create_ms = dlsym(kernel32, "CreateThread");
		thrd_detach_ms = dlsym(kernel32, "CloseHandle");
		environ = *(void**)dlsym(libc, "_environ");
		stdin = fdopen( 0, "r" );
		stdout = fdopen( 1, "a" );
		stderr = fdopen( 2, "a" );
	} else {
		vswprintf_sysv = dlsym(libc, "vswprintf");
		malloc_sysv = dlsym(libc, "malloc");
		calloc_sysv = dlsym(libc, "calloc");
		aligned_alloc_sysv = dlsym(libc, "aligned_alloc");
		aligned_free_sysv = dlsym(libc, "free");
		realloc_sysv = dlsym(libc, "realloc");
		free_sysv = dlsym(libc, "free");
		fflush_sysv = dlsym(libc, "fflush");
		thrd_yield_sysv = dlsym(libc, "thrd_yield");
		thrd_current_sysv = dlsym(libc, "thrd_current");
		thrd_sleep_sysv = dlsym(libc, "thrd_sleep");
		tss_create_sysv = dlsym(libc, "tss_create");
		tss_delete_sysv = dlsym(libc, "tss_delete");
		tss_get_sysv = dlsym(libc, "tss_get");
		tss_set_sysv = dlsym(libc, "tss_set");
		fwrite_sysv = dlsym(libc, "fwrite");
		fopen_sysv = dlsym(libc, "fopen");
		fdopen_sysv = dlsym(libc, "fdopen");
		fclose_sysv = dlsym(libc, "fclose");
		ftell_sysv = dlsym(libc, "ftell");
		fseek_sysv = dlsym(libc, "fseek");
		fread_sysv = dlsym(libc, "fread");
		remove_sysv = dlsym(libc, "remove");
		//cnd_init_ms = dlsym(libc, "cnd_init");
		//cnd_broadcast_sysv = dlsym(libc, "cnd_broadcast");
		//cnd_signal_sysv = dlsym(libc, "cnd_signal");
		//cnd_wait_sysv = dlsym(libc, "cnd_wait");
		//cnd_destroy_sysv = dlsym(libc, "cnd_destroy");
		//cnd_timedwait_sysv = dlsym(libc, "cnd_timedwait");
//		mtx_init_sysv = dlsym(libc, "mtx_init");
//		mtx_lock_sysv = dlsym(libc, "mtx_lock");
//		mtx_unlock_sysv = dlsym(libc, "mtx_unlock");
//		mtx_trylock_sysv = dlsym(libc, "mtx_trylock");
//		mtx_destroy_sysv = dlsym(libc, "mtx_destroy");
		thrd_equal_sysv = dlsym(libc, "thrd_equal");
		clock_gettime_sysv = dlsym(libc, "clock_gettime");
		getentropy_sysv = dlsym(libc, "getentropy");
		realpath_sysv = dlsym(libc, "realpath");
		mkdir_sysv = dlsym(libc, "mkdir");
		stat_sysv = dlsym(libc, "stat");
		fstat_sysv = dlsym(libc, "fstat");
		getcwd_sysv = dlsym(libc, "getcwd");
		readdir_sysv = dlsym(libc, "readdir");
		closedir_sysv = dlsym(libc, "closedir");
		chdir_sysv = dlsym(libc, "chdir");
		opendir_sysv = dlsym(libc, "opendir");
		readlink_sysv = dlsym(libc, "readlink");
		rename_sysv = dlsym(libc, "rename");
		thrd_join_sysv = dlsym(libc, "thrd_join");
		thrd_create_sysv = dlsym(libc, "thrd_create");
		thrd_detach_sysv = dlsym(libc, "thrd_detach");
		truncate_sysv = dlsym(libc, "truncate");
		statvfs_sysv = dlsym(libc, "statvfs");
		lstat_sysv = dlsym(libc, "lstat");
		mmap_sysv = dlsym(libc, "mmap");
		munmap_sysv = dlsym(libc, "munmap");
		environ = *(void**)dlsym(libc, "__environ");
		stdin = dlsym(libc, "_IO_2_1_stdin_");
		stdout = dlsym(libc, "_IO_2_1_stdout_");
		stderr = dlsym(libc, "_IO_2_1_stderr_");
	}
	//external_linux_c_dprintf(1, "setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
	//printf("setting stdin=%p, stdout=%p, stderr=%p\n", stdin, stdout, stderr);
}

DLL_PUBLIC
char getc(FILE* file) {
	char ret;
	int read = fread(&ret, 1, 1, file);
	if (read == 1) { return ret; }
	return EOF;
}

DLL_PUBLIC
int ungetc(int c, FILE *stream) {
    UNUSED(c);
    UNUSED(stream);
    fprintf(stderr, "vsscanf not implemented\n");
    fflush(stderr);
    abort();
    return 0;
}

DLL_PUBLIC
char getchar() {
	return getc(stdin);
}


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
int vsscanf(const char *s, const char *format, va_list arg) {
    UNUSED(s);
    UNUSED(format);
    UNUSED(arg);
    fprintf(stderr, "vsscanf not implemented\n");
    fflush(stderr);
    abort();
    return 0;
}

int sscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int result = vsscanf(str, format, args);

    va_end(args);
    return result;
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
	//debug_printf("execute os free %p\n", ptr);
	if (isWin) {
		return free_ms(ptr);
	} else {
#ifdef FENCE
		if (ptr == 0) { return; }
		uint64_t oldsize = *(uint64_t*)(ptr-8);
		munmap_sysv(ptr-8, oldsize + 8);
		return;
#else
		return free_sysv(ptr);
#endif
	}
}

#define PROT_READ       1
#define PROT_WRITE      2
#define MAP_PRIVATE     2
#define MAP_ANONYMOUS   32

DLL_PUBLIC void* malloc(size_t new_size) {
	//debug_printf("execute os malloc %ld ", new_size);
	void* result = 0;
	if (isWin) {
		result = malloc_ms(new_size);
	} else {
#ifdef FENCE
		result = mmap_sysv(0, new_size + 8, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
		*(uint64_t*) result = new_size;
		result += 8;
#else
		result = malloc_sysv(new_size);
#endif
	}
	//debug_printf("= %p\n", result);
	return result;
}

DLL_PUBLIC void *calloc(size_t nmemb, size_t size) {
	//debug_printf("execute os calloc %ld x %ld ", nmemb, size);
	void* result = 0;
	if (isWin) {
		result = calloc_ms(nmemb, size);
	} else {
#ifdef FENCE
		result = malloc(nmemb * size);
		memset(result, 0, nmemb * size);
#else
		result = calloc_sysv(nmemb, size);
#endif
	}
	//debug_printf("= %p\n", result);
	return result;
}

DLL_PUBLIC
void* aligned_alloc(size_t alignment, size_t new_size) {
	//debug_printf("execute os aligned_alloc alignment %d size %d", alignment, new_size);
	void* ret=0;
	if (isWin) {
		ret = aligned_alloc_ms(new_size, alignment);
	} else {
		ret = aligned_alloc_sysv(alignment, new_size);
	}
	//debug_printf("= %p\n", ret);
	return ret;
}

DLL_PUBLIC
void aligned_free(void* ptr) {
	//debug_printf("execute os aligned_free %p\n", ptr);
	if (isWin) {
		return aligned_free_ms(ptr);
	} else {
		return free_sysv(ptr);
	}
}

DLL_PUBLIC
void *realloc(void *ptr, size_t size) {
	//debug_printf("execute os realloc %p %p\n", ptr, size);
	if (isWin) {
		return realloc_ms(ptr, size);
	} else {
#ifdef FENCE
		if (ptr == 0) { return malloc(size); }
		uint64_t oldsize = *(uint64_t*)(ptr-8);
		void* res = malloc(size);
		memcpy(res, ptr, oldsize > size ? size : oldsize);
		return res;
#else
		return realloc_sysv(ptr, size);
#endif
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
	debug_printf("execute os fflush\n");
	if (isWin) {
		return fflush_ms(file);
	} else {
		return fflush_sysv(file);
	}
}

DLL_PUBLIC int remove(const char *pathname) {
	debug_printf("execute os remove\n");
	if (isWin) {
		int ret = remove_ms(pathname);
		if (ret == 0) { return 0; }
		return _rmdir_ms(pathname);
	} else {
		return remove_sysv(pathname);
	}
}

static const char basealias[] = "/_zwolf/";
#define basealias_len (sizeof(basealias) - 1)

DLL_PUBLIC
FILE *fopen(const char *filenameOrig, const char *mode) {
	debug_printf("execute os fopen %s (%s)\n", filenameOrig, mode);
	int filelen = strlen(filenameOrig);

	const char* filename = filenameOrig;
	
	if (strncmp(basealias, filename, basealias_len) == 0) {
		char* base = getenv("ZWOLF_RUNDIR");
		if (base) {
			int baselen = strlen(base);

			char * filename_tmp = __builtin_alloca(baselen + filelen - basealias_len + 1);
			memcpy(filename_tmp, base, baselen);
			memcpy(filename_tmp + baselen, filenameOrig + basealias_len, filelen - basealias_len + 1);
			filename = filename_tmp;
		}
	}

	if (isWin) {
		uintptr_t len = 0;
		int32_t error = base_fs_tonativepathlen(filename, &len);
		if (error != SUCCESS) { 
			errno = error;
			return NULL;
		}
		uint16_t* nativepath = __builtin_alloca(len * 2);
		error = base_fs_tonativepath(filename, nativepath);
		if (error != SUCCESS) {
			errno = error;
			return NULL;
		}
		uint16_t mode_ms[4] = { 0, 0, 0, 0 };
		for (int i = 0 ; *mode != '\0' && i < 3 ; i++) {
			mode_ms[i] = mode[i];
		}
		return _wfopen_ms(nativepath, mode_ms);
	} else {
		return fopen_sysv(filename, mode);
	}
}

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

DLL_PUBLIC int fputc(int c, FILE *stream) {
	const int written = fwrite(&c, 1, 1, stream);
	if (written == 1) { return c; }
	return EOF;
}

DLL_PUBLIC
void thrd_yield(void) {
	debug_printf("execute os thrd_yield\n");
	if (isWin) {
		return thrd_yield_ms();
	} else {
		return thrd_yield_sysv();
	}
}

DLL_PUBLIC int thrd_equal(thrd_t lhs, thrd_t rhs ) {
	debug_printf("execute os thrd_equal\n");
	if (isWin) {
		return GetThreadId_ms(lhs) == GetThreadId_ms(rhs);
	} else {
		return thrd_equal_sysv(lhs, rhs);
	}
}

DLL_PUBLIC thrd_t thrd_current(void) IMPLEMENT(thrd_current)

DLL_PUBLIC int thrd_sleep(const struct timespec* duration, struct timespec* remaining ) {
	debug_printf("execute os thrd_sleep\n");
	if (isWin) {
		// long milliseconds = duration->tv_sec * 1000 + duration->tv_nsec / 1000000;
		uint64_t milliseconds = duration->tv_sec * 1000 + duration->tv_nsec / 1000000;
		uint64_t result = thrd_sleep_ms(milliseconds, true);

		if (remaining != NULL) {
			remaining->tv_sec = result / 1000;
			remaining->tv_nsec = (result % 1000) * 1000000;
		}
    
		return 0;
	} else {
		return thrd_sleep_sysv(duration, remaining);
	}
}

struct ms_thrd_func_state {
	thrd_start_t f;
	void* arg;
};
__attribute((ms_abi)) int ms_thrd_func(void * arg) { // ****ing calling convention
	struct ms_thrd_func_state* state = (struct ms_thrd_func_state*)arg;
	thrd_start_t f = state->f;
	void* arg2 = state->arg;
	free(arg);
	return f(arg2);
}
DLL_PUBLIC int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
	debug_printf("execute os thrd_create\n");
	if (isWin) {
		struct ms_thrd_func_state* state = malloc(sizeof(struct ms_thrd_func_state));
		state->f = func;
		state->arg = arg;
		size_t hThread = thrd_create_ms(NULL, 0, ms_thrd_func, state, 0, NULL);

		if (hThread == NULL) {
			// Thread creation failed
			return -1;
		}

		// Store the handle in thrd_t
	*thr = hThread;

		return 0;
	} else {
		return thrd_create_sysv(thr, func, arg);
	}
}

DLL_PUBLIC int thrd_join( thrd_t thr, int *res ) {
	debug_printf("execute os thrd_join\n");
	if (isWin) {
		uint32_t INFINITE = -1;
		uint32_t result = thrd_join_wait_ms(thr, INFINITE);

		uint32_t WAIT_OBJECT_0 = 0;
		if (result == WAIT_OBJECT_0) {
			// Thread has terminated successfully

			if (res != NULL) {
				// Get the thread's exit code
				int32_t exitCode;
				thrd_join_ms(thr, &exitCode);

				// Store the exit code in the provided pointer
				*res = (int)exitCode;
			}

			// Close the thread handle
			thrd_detach(thr);

			return 0;
		}
		else {
			// Error occurred while waiting for the thread
			return -1;
		}
	} else {
		return thrd_join_sysv(thr, res);
	}
}

DLL_PUBLIC void call_once(once_flag* flag, void (*func)(void))
{
    if (__atomic_test_and_set(flag, __ATOMIC_ACQUIRE)) {
        // Another thread has already executed the function, do nothing.
        return;
    }
    
    // Call the initialization function
    func();
    
    // Clear the flag after initialization
    //__atomic_clear(flag, __ATOMIC_RELEASE);
}

DLL_PUBLIC int tss_create(tss_t* tss_key, tss_dtor_t destructor) {
	if (isWin) {
		*tss_key = tss_create_ms();

		if (tss_set(*tss_key, NULL) != thrd_success) {
			tss_delete(*tss_key);
			return -1;  // TSS initialization failed
		}

		if (destructor != NULL) {
			debug_printf("no tss destructor on windows\n");
		}

		return 0;  // Success
	} else {
		return tss_create_sysv(tss_key, destructor);
	}
}
DLL_PUBLIC void tss_delete(tss_t tss_id) IMPLEMENT(tss_delete, tss_id)
DLL_PUBLIC void *tss_get(tss_t tss_key) IMPLEMENT(tss_get, tss_key)
DLL_PUBLIC int tss_set(tss_t tss_id, void *val) {
	debug_printf("execute os tss_set\n");
	if (isWin) {
		return tss_set_ms(tss_id, val) ? thrd_success : thrd_error;
	} else {
		return tss_set_sysv(tss_id, val);
	}
}


//DLL_PUBLIC int cnd_init(cnd_t* cond) {
//	debug_printf("execute os cnd_init\n");
//	if (isWin) {
//		cnd_init_ms(cond);
//		return thrd_success;
//	} else {
//		return cnd_init_sysv(cond);
//	}
//}
//DLL_PUBLIC int cnd_broadcast( cnd_t *cond ) {
//	debug_printf("execute os cnd_broadcast\n");
//	if (isWin) {
//		cnd_broadcast_ms(cond);
//		return thrd_success;
//	} else {
//		return cnd_broadcast_sysv(cond);
//	}
//}
//DLL_PUBLIC int cnd_wait( cnd_t* cond, mtx_t* mutex ) {
//	debug_printf("execute os cnd_wait\n");
//	if (isWin) {
//		SleepConditionVariableCS(cond, (mtx_t*)*mutex, -1);
//		return thrd_success;
//	} else {
//		return cnd_wait_sysv(cond, (mtx_t*)*mutex);
//	}
//}
//DLL_PUBLIC int cnd_signal(cnd_t *cond ) {
//	debug_printf("execute os cnd_signal\n");
//	if (isWin) {
//		cnd_signal_ms(cond);
//		return thrd_success;
//	} else {
//		return cnd_signal_sysv(cond);
//	}
//}
//DLL_PUBLIC void cnd_destroy(cnd_t* cond ) {
//	debug_printf("execute os cnd_destroy\n");
//	if (isWin) {
//		// nothing to do
//	} else {
//		return cnd_destroy_sysv(cond);
//	}
//}
//DLL_PUBLIC int cnd_timedwait( cnd_t*  cond, mtx_t*  mutex, const struct timespec*  time_point ) {
//	debug_printf("execute os cnd_wait\n");
//	if (isWin) {
//		uint32_t milliseconds = (uint32_t)(time_point->tv_sec * 1000) + (uint32_t)(time_point->tv_nsec / 1000000);
//		return SleepConditionVariableCS(cond, (mtx_t*)*mutex, milliseconds) == 0 ? thrd_timedout : thrd_success;
//	} else {
//		return cnd_timedwait_sysv(cond, (mtx_t*)*mutex, time_point);
//	}
//}

DLL_PUBLIC int stat(const char * p, struct stat * statbuf) {
	debug_printf("execute os stat on path %s buf %p\n", p, statbuf);

	void * pathname = 0;
	int ret = -1;
	tonativepath(p, &pathname);

	if (isWin) {
		struct windows_stat ls;
		ret = _wstat_ms(pathname, (struct stat*)&ls);
		if (ret == -1) { errno = zwolf_errno(); }
		statbuf->st_mode = ls.st_mode;        /* File type and mode */
		statbuf->st_dev = ls.st_dev;
		statbuf->st_ino = rand();
		statbuf->st_nlink = ls.st_nlink;
		statbuf->st_size = ls.st_size;
		statbuf->st_mtim.tv_sec = ls.wst_mtime / 1000;
		statbuf->st_mtim.tv_nsec = (ls.wst_mtime % 1000) * 1000000;
		debug_printf("stat returned %d %p mode %lx\n", ret, statbuf, ls.st_mode);
	} else {
		struct linux_stat ls;
		ret = stat_sysv(pathname, (struct stat*)&ls);
		if (ret == -1) { errno = zwolf_errno(); }
		statbuf->st_mode = ls.st_mode;        /* File type and mode */
		statbuf->st_dev = ls.st_dev;
		statbuf->st_ino = ls.st_ino;
		statbuf->st_nlink = ls.st_nlink;
		statbuf->st_size = ls.st_size;
		statbuf->st_mtim = ls.st_mtim;
		debug_printf("stat returned %d %p mode %lx\n", ret, statbuf, ls.st_mode);
	}
	if (pathname) { free(pathname); }
	return ret;
}
DLL_PUBLIC int fstat(int fd, struct stat * statbuf) {
	debug_printf("execute os fstat on fd %d buf %p\n", fd, statbuf);
	if (isWin) {
		struct windows_stat ls;
		const int ret = fstat_ms(fd, (struct stat*)&ls);
		if (ret == -1) { errno = zwolf_errno(); }
		statbuf->st_mode = ls.st_mode;        /* File type and mode */
		statbuf->st_dev = ls.st_dev;
		statbuf->st_ino = rand();
		statbuf->st_nlink = ls.st_nlink;
		statbuf->st_size = ls.st_size;
		statbuf->st_mtim.tv_sec = ls.wst_mtime / 1000;
		statbuf->st_mtim.tv_nsec = (ls.wst_mtime % 1000) * 1000000;
		debug_printf("stat returned %d %p mode %lx inode %ld \n", ret, statbuf, ls.st_mode, statbuf->st_ino);
		return ret;
	} else {
		struct linux_stat ls;
		const int ret = fstat_sysv(fd, (struct stat*)&ls);
		if (ret == -1) { errno = zwolf_errno(); }
		statbuf->st_mode = ls.st_mode;        /* File type and mode */
		statbuf->st_dev = ls.st_dev;
		statbuf->st_ino = ls.st_ino;
		statbuf->st_nlink = ls.st_nlink;
		statbuf->st_size = ls.st_size;
		statbuf->st_mtim = ls.st_mtim;
		debug_printf("stat returned %d %p mode foo %ld\n", ret, statbuf, ls.st_mode);
		return ret;
	}
}
DLL_PUBLIC int clock_gettime(clockid_t clockid, struct timespec *tp) IMPLEMENT(clock_gettime, clockid, tp)
DLL_PUBLIC int getentropy(void *buffer, size_t length) IMPLEMENT(getentropy, buffer, length)
DLL_PUBLIC char* realpath(const char * path, char * resolved_path) {
	debug_printf("execute os realpath %s = ", path);
	if (isWin) {
		void * native_path = 0;
		char* ret_path = 0;
		uint32_t err = tonativepath(path, &native_path);
		if (err) { goto error; }
		uint16_t * ret = _wfullpath_ms(0, native_path, 0);
		if (ret == 0) { err = 1; goto error; }
		err = alloc_windows_path(ret, &ret_path);
		if (err) { goto error; }


		if (resolved_path) {
			if (strlen(ret_path) > PATH_MAX) { err = 1; goto error; }
			strcpy(resolved_path, ret_path);
			free(ret_path);
		}

		free(native_path);
		debug_printf("%s\n", resolved_path);
		return resolved_path;
error:
		free(ret_path);
		free(native_path);
		errno = err;
		return 0;

	} else {
		char* ret = realpath_sysv(path, resolved_path);
		debug_printf("%s\n", ret);
		return ret;
	}
}
struct win_dir {
	struct dirent linux_dirent;
	WIN32_FIND_DATAW win_dirent;
	int32_t handle;
	bool initialized;
	uint16_t path[MAX_WIN_PATH];
};
DLL_PUBLIC int mkdir(const char *pathname, mode_t mode) {
	debug_printf("execute os mkdir\n");
	if (isWin) {
		// nothing to do
		debug_printf("execute os mkdir %p %s \n", _mkdir_ms, pathname);
		return _mkdir_ms(pathname);
	} else {
		return mkdir_sysv(pathname, mode);
	}
}
DLL_PUBLIC char* getcwd(char *buf, size_t size) {
	debug_printf("getcwd\n");
	int32_t err = 0;
	if (isWin) {
		uint16_t buffer[4096];
		uint16_t * ret = wgetcwd_ms(buffer, 4096);
		if (ret == 0) { err = 1; }

		char * path = 0;
		err = alloc_windows_path(buffer, &path);
		if (err) { errno = err; return 0; }

		debug_printf("getcwd path %s\n", path);

		size_t len = strlen(path);
		if (buf != 0) {
			if (size <= len) {
				errno = err;
				free(path);
				return 0;
			} else {
				strcpy(buf, path);
				free(path);
			}
			return buf;
		}
		return path;
	} else {
		return getcwd_sysv(buf, size);
	}
}

DLL_PUBLIC struct dirent* readdir(DIR * dir) {
	debug_printf("execute os readdir %p\n", dir);
	if (isWin) {
		struct win_dir* windir = (struct win_dir*)dir;
		if (windir->initialized) {
			bool hasNext = FindNextFileW_ms(windir->handle, &windir->win_dirent);
			if (!hasNext) {
				return 0;
			}
			//strncpy((char*)&windir->linux_dirent.d_name, windir->win_dirent.name, 256);
			utf16to8(windir->win_dirent.name, &windir->linux_dirent.d_name);
			return &windir->linux_dirent;
		} else {
			int32_t handle = FindFirstFileW_ms(windir->path, &windir->win_dirent);
			if (handle == -1) {
				return 0;
			}
			//strncpy((char*)&windir->linux_dirent.d_name, windir->win_dirent.name, 256);
			utf16to8(windir->win_dirent.name, &windir->linux_dirent.d_name);
			windir->handle = handle;
			windir->initialized = true;
			return &windir->linux_dirent;
		}
	} else {
		return readdir_sysv(dir);
	}
}
DLL_PUBLIC int closedir(DIR* dir) {
	debug_printf("execute os opendir\n");
	if (isWin) {
		free(dir);
		return 0;
	} else {
		return closedir_sysv(dir);
	}
}
DLL_PUBLIC int chdir(const char *path) IMPLEMENT(chdir, path)
DLL_PUBLIC DIR* opendir(const char * path) {
	debug_printf("execute os opendir %s = ", path);

	void * pathname = 0;
	DIR* ret = 0;
	tonativepath(path, &pathname);

	if (isWin) {
		struct win_dir* dir = calloc(1, sizeof(struct win_dir));
		uint16_t * winpath = pathname;
		size_t len = 0;
		for (; winpath[len] != 0; len++) {}
		if (len >= MAX_WIN_PATH - 3) {
			fprintf(stderr, "path too long");
			fflush(stderr);
			abort();
		}
		memcpy(&dir->path, winpath, len * 2);
		dir->path[len] = '\\';
		dir->path[len+1] = '*';
		dir->path[len+2] = '\0';
		debug_printf("%p\n", dir);
		ret = (void*)dir;
	} else {
		ret = opendir_sysv(pathname);
		debug_printf("%p\n", ret);
	}

	if (pathname) { free(pathname); }
	return ret;
}
DLL_PUBLIC ssize_t readlink(const char * pathname, char * buf, size_t bufsiz) IMPLEMENT(readlink, pathname, buf, bufsiz)
DLL_PUBLIC int rename(const char *oldpath, const char *newpath) IMPLEMENT(rename, oldpath, newpath)
DLL_PUBLIC int thrd_detach( thrd_t thr ) IMPLEMENT(thrd_detach, thr)
DLL_PUBLIC int truncate(const char *path, off_t length) IMPLEMENT(truncate, path, length)
DLL_PUBLIC int statvfs(const char * path, struct statvfs * buf) IMPLEMENT(statvfs, path, buf)
DLL_PUBLIC int lstat(const char * pathname, struct stat * statbuf) {
	debug_printf("execute os stat on path %s buf %p\n", pathname, statbuf);
	if (isWin) {
		struct windows_stat ls;
		const int ret = stat_ms(pathname, (struct stat*)&ls);
		if (ret == -1) { errno = zwolf_errno(); }
		statbuf->st_mode = ls.st_mode;        /* File type and mode */
		statbuf->st_dev = ls.st_dev;
		statbuf->st_ino = ls.st_ino;
		statbuf->st_nlink = ls.st_nlink;
		statbuf->st_size = ls.st_size;
		statbuf->st_mtim.tv_sec = ls.wst_mtime / 1000;
		statbuf->st_mtim.tv_nsec = (ls.wst_mtime % 1000) * 1000000;
		debug_printf("stat returned %d %p mode %lx\n", ret, statbuf, ls.st_mode);
		return ret;
	} else {
		struct linux_stat ls;
		const int ret = lstat_sysv(pathname, (struct stat*)&ls);
		if (ret == -1) { errno = zwolf_errno(); }
		statbuf->st_mode = ls.st_mode;        /* File type and mode */
		statbuf->st_dev = ls.st_dev;
		statbuf->st_ino = ls.st_ino;
		statbuf->st_nlink = ls.st_nlink;
		statbuf->st_size = ls.st_size;
		statbuf->st_mtim = ls.st_mtim;
		debug_printf("stat returned %d %p mode %lx\n", ret, statbuf, ls.st_mode);
		return ret;
	}
}

int ftruncate(int fd, uint64_t length) {
	int ret = base_fs_truncate(fd, length);
	if (ret == SUCCESS) { return 0; }
	errno = ret;
	return -1;
}

//DLL_PUBLIC void mtx_destroy(mtx_t *mutex ) {
//	debug_printf("execute os mtx_destroy\n");
//	if (mutex == NULL) { return; }
//	if (isWin) {
//		mtx_destroy_ms((mtx_t*)*mutex);
//	} else {
//		mtx_destroy_sysv((mtx_t*)*mutex);
//	}
//	free(*mutex);
//}

//int mtx_init_if_needed(mtx_t* mutex) {
//	debug_printf("execute os mtx_init_if_needed %p at %p\n", *mutex, mutex);
//	if (*mutex == NULL) {
//		mtx_t m = NULL;
//		void* null = NULL;
//		int ret = mtx_init(&m, 0);
//		if (!__atomic_compare_exchange_n(mutex, &null, m, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
//			mtx_destroy(&m);
//		}
//		return ret;
//	}
//	return thrd_success;
//}
//DLL_PUBLIC int mtx_init(mtx_t* mutex, int type) {
//	debug_printf("execute os mtx_init %p at %p\n", *mutex, mutex);
//	mtx_t mtx = malloc(80);
//	debug_printf("malloced %p\n", mtx);
//	if (!mtx) { return thrd_error; }
//	int ret;
//	if (isWin) {
//		ret = mtx_init_ms((mtx_t*)mtx, type);
//	} else {
//		ret = mtx_init_sysv((mtx_t*)mtx, type);
//	}
//	if (ret != thrd_success) {
//		debug_printf("failed to init\n\n\n");
//		free(mtx);
//	}
//	*mutex = mtx;
//	return ret;
//}
//{ UNUSED(mutex); UNUSED(type); return 0; }

//DLL_PUBLIC int mtx_lock(mtx_t* mutex) {
//	UNUSED(mutex);
//	debug_printf("execute os mtx_lock %p at %p\n", *mutex, mutex);
//	mtx_init_if_needed(mutex);
//	if (isWin) {
//		mtx_lock_ms((mtx_t*)*mutex);
//		return thrd_success;
//	} else {
//		return mtx_lock_sysv((mtx_t*)*mutex);
//	}
//	return 0;
//}
//DLL_PUBLIC int mtx_trylock(mtx_t *mutex ) {
//	debug_printf("execute os mtx_trylock %p at %p\n", *mutex, mutex);
//	mtx_init_if_needed(mutex);
//	if (isWin) {
//		return !mtx_trylock_ms((mtx_t*)*mutex);
//	} else {
//		return mtx_trylock_sysv((mtx_t*)*mutex);
//	}
//}
//DLL_PUBLIC int mtx_unlock(mtx_t* mutex) {
//	debug_printf("execute os mtx_unlock %p at %p\n", *mutex, mutex);
//	if (*mutex == NULL) { return thrd_error; }
//	if (isWin) {
//		mtx_unlock_ms((mtx_t*)*mutex);
//		return thrd_success;
//	} else {
//		return mtx_unlock_sysv((mtx_t*)*mutex);
//	}
//}

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
void __tls_get_addr() {
    fprintf(stderr, "__tls_get_addr not implemented\n");
    fflush(stderr);
    abort();
}

struct atexit {
	void (*func)(void*);
	void* arg;
};
size_t at_exit_cnt = 0;
size_t at_exit_size = 0;
struct atexit* at_exit_entries = 0;


DLL_PUBLIC
int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle) {
    if (at_exit_entries == 0) {
	    at_exit_entries = malloc(10 * sizeof(struct atexit));
	    at_exit_size = 10;
    }
    if (at_exit_cnt >= at_exit_size) {
	    at_exit_size *= 2;
	    struct atexit* tmp = malloc(at_exit_size * sizeof(struct atexit));
	    memcpy(tmp, at_exit_entries, at_exit_cnt * sizeof(struct atexit));
	    free(at_exit_entries);
	    at_exit_entries = tmp;
    }
    at_exit_entries[at_exit_cnt].func = func;
    at_exit_entries[at_exit_cnt].arg = arg;
    at_exit_cnt ++ ;
    UNUSED(dso_handle); // todo
    return 0;
}

__attribute__((destructor)) void fini() {
	debug_printf("fini %d\n", at_exit_cnt);
	for (size_t i = 0 ; i < at_exit_cnt; i++ ){
		at_exit_entries[i].func(at_exit_entries[i].arg);
	}
}


DLL_PUBLIC
int __cxa_thread_atexit_impl(void (*func) (void *), void * arg, void * dso_handle) {
    UNUSED(dso_handle); // todo
    UNUSED(func);
    UNUSED(arg);
    debug_printf("cxa_atexit not implemented properly\n");
    return 0;
}
DLL_PUBLIC
int __cxa_thread_atexit(void (*func) (void *), void * arg, void * dso_handle) {
    UNUSED(dso_handle); // todo
    UNUSED(func);
    UNUSED(arg);
    debug_printf("cxa_atexit not implemented properly\n");
    return 0;
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
int puts(const char *s) {
	return fputs(s, stdout);
}

DLL_PUBLIC
char* strerror_r(int errnum, char* buf, size_t buflen) {
    UNUSED(errnum);
    UNUSED(buf);
    UNUSED(buflen);
    return "unknown error";
}

DLL_PUBLIC
char* strerror(int errnum) {
    UNUSED(errnum);
    return "unknown error";
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
    // Check for valid base
    if (base < 0 || (base != 0 && (base < 2 || base > 36))) {
        errno = EINVAL;  // Invalid argument
        return 0;
    }

    // Handle optional leading whitespace
    while (*str == ' ' || (*str >= '\t' && *str <= '\r')) {
        str++;
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

    return result;
}

DLL_PUBLIC
long long int strtoll(const char* str, char** endptr, int base) {
    return (long long int)strtol(str, endptr, base);
}

DLL_PUBLIC
unsigned long long int strtoull(const char* str, char** endptr, int base) {
    return (unsigned long long int)strtoul(str, endptr, base);
}

//DLL_PUBLIC
//int errno = 0;

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
    debug_printf("wcrtomb not implemented %s %d %p\n", s, wc, ps);
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
    fprintf(stderr, "mbrtowc not implemented\n");
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
    fprintf(stderr, "mbsnrtowcs not implemented\n");
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


DLL_PUBLIC size_t wcslen(const wchar_t *s) {
    size_t length = 0;
    while (*s != L'\0') {
        ++length;
        ++s;
    }
    return length;
}

DLL_PUBLIC int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }
    return 0;
}

DLL_PUBLIC int wcscoll(const wchar_t *s1, const wchar_t *s2) {
    while (*s1 && (*s1 == *s2)) {
        ++s1;
        ++s2;
    }
    
    return (*s1 > *s2) ? 1 : ((*s1 < *s2) ? -1 : 0);
}

wchar_t* wcscpy(wchar_t* dest, const wchar_t* src) {
    wchar_t* d = dest;
    while ((*d++ = *src++))
        ;
    return dest;
}

wchar_t* wcsncpy(wchar_t* dest, const wchar_t* src, size_t n) {
    wchar_t* d = dest;
    size_t i = 0;
    while (i < n && (*d++ = *src++))
        i++;
    while (i < n) {
        *d++ = L'\0';
        i++;
    }
    return dest;
}

DLL_PUBLIC size_t wcsxfrm(wchar_t *dest, const wchar_t *src, size_t n) {
    size_t srcLen = wcslen(src);
    
    if (n <= srcLen) {
        // Truncated copy if the buffer size is smaller or equal to the source length
        wcsncpy(dest, src, n - 1);
        dest[n - 1] = L'\0';
        return srcLen;
    } else {
        // Full copy if the buffer size is larger than the source length
        wcscpy(dest, src);
        return srcLen;
    }
}

DLL_PUBLIC int iswlower(wint_t wc) {
    // Basic Latin alphabet (A-Z)
    if (wc >= 0x61 && wc <= 0x7A)
        return 1;
    return 0;
}

DLL_PUBLIC int iswupper(wint_t wc) {
    return (wc >= 'A' && wc <= 'Z');
}

DLL_PUBLIC int iswprint(wint_t wc) {
    return (wc >= 0x20 && wc <= 0x7E);
}

DLL_PUBLIC int iswblank(wint_t wc) {
    return (wc == ' ' || wc == '\t');
}

DLL_PUBLIC int iswcntrl(wint_t wc) {
    return (wc <= 0x1F) || (wc == 0x7F);
}

DLL_PUBLIC int iswalpha(wint_t wc) {
    return ((wc >= 'A' && wc <= 'Z') || (wc >= 'a' && wc <= 'z'));
}

DLL_PUBLIC wint_t towlower(wint_t wc) {
    if (wc >= L'A' && wc <= L'Z')
        return wc + 32;
    return wc;
}

DLL_PUBLIC int iswxdigit(wint_t wc) {
    return (wc >= L'0' && wc <= L'9') || (wc >= L'A' && wc <= L'F') || (wc >= L'a' && wc <= L'f');
}

DLL_PUBLIC int iswpunct(wint_t wc) {
    return (wc >= 33 && wc <= 47) || (wc >= 58 && wc <= 64) || (wc >= 91 && wc <= 96) || (wc >= 123 && wc <= 126);
}


locale_t uselocale(locale_t locale) {
    UNUSED(locale);
    debug_printf("uselocale not implemented\n");
    return 1;
}
locale_t newlocale(int category_mask, const char *locale, locale_t base) {
    UNUSED(category_mask);
    UNUSED(locale);
    UNUSED(base);
    debug_printf("newlocale not implemented\n");
    return 1;
}
void freelocale(locale_t locobj) {
    UNUSED(locobj);
    debug_printf("freelocale not implemented\n");
}
char *setlocale(int category, const char *locale) {
    UNUSED(category);
    UNUSED(locale);
    debug_printf("setlocale not implemented\n");
    return 0;
}

struct lconv* localeconv() {
    fprintf(stderr, "localeconv not implemented\n");
    fflush(stderr);
    abort();
    return 0;
}

DLL_PUBLIC int mtime(const char *pathname, struct timespec * time) {
    UNUSED(pathname);
    UNUSED(time);
    fprintf(stderr, "mtime not implemented\n");
    fflush(stderr);
    abort();
    return 0;
}

DLL_PUBLIC
_Noreturn void exit(int status)
{
	zwolf_exit(status);
}

//void print_addr(void * ptr) {
//	const char * lib = 0;
//	size_t offset = 0;
//	__resolve(ptr, &lib, &offset);
//	if (lib) {
//        	debug_printf("\t%p %s(%p)\n", ptr, lib, offset);
//	} else {
//        	debug_printf("\t%p\n", ptr);
//	}
//}
//#define MAX_BACKTRACE_FRAMES 128
//DLL_PUBLIC
//void print_backtrace()
//{
//	debug_printf("Backtrace:\n");
//        print_addr(__builtin_return_address(0));
//        print_addr(__builtin_return_address(1));
//        print_addr(__builtin_return_address(2));
//        print_addr(__builtin_return_address(3));
//        print_addr(__builtin_return_address(4));
//        print_addr(__builtin_return_address(5));
//        print_addr(__builtin_return_address(6));
//        print_addr(__builtin_return_address(7));
//}



char* getenv(const char* name) {
    if (name == NULL || name[0] == '\0') {
        return NULL; // Invalid input: NULL pointer or empty string
    }

    size_t name_length = strlen(name);

    for (char** env = environ; *env != NULL; env++) {
        if (strncmp(*env, name, name_length) == 0 && (*env)[name_length] == '=') {
            // Found the environment variable with the matching name
            return *env + name_length + 1;
        }
    }

    // Environment variable not found
    return NULL;
}

#define NO_IMPL(name) \
{ \
	debug_printf("not implemented %s\n", #name); \
	abort(); \
}

DLL_PUBLIC int access(const char *pathname, int mode) {
	debug_printf("access %s\n", pathname);
	uintptr_t fd = 0;
	int32_t error = 0;
	if (mode == F_OK) { mode = R_OK; }
	error = base_fs_open(pathname, &fd,
			((mode & R_OK) ? BASE_FS_OPEN_READ : 0) |
			((mode & W_OK) ? BASE_FS_OPEN_WRITE : 0)); // ignore execute
	if (error == SUCCESS) {
		debug_printf("access granted\n");
		base_fs_close(fd);
		return 0;
	}

	errno = error;
	return -1;
}
DLL_PUBLIC int eaccess(const char *pathname, int mode) { return access(pathname, mode); }

DLL_PUBLIC int getpwuid_r(uid_t uid, struct passwd * pwd, char * buf, size_t buflen, struct passwd ** result) NO_IMPL(getpwuid_r)
DLL_PUBLIC uid_t getuid(void) NO_IMPL(getuid)
DLL_PUBLIC int dup2(int oldfd, int newfd) NO_IMPL(dup2)
DLL_PUBLIC pid_t fork(void) NO_IMPL(fork)
DLL_PUBLIC int sigemptyset(sigset_t *set) {
	debug_printf("sigemptyset not implemented properly\n");
	return 0;
}

DLL_PUBLIC void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) NO_IMPL(mmap)
DLL_PUBLIC int munmap(void *addr, size_t length) NO_IMPL(munmap)
DLL_PUBLIC int chmod(const char *pathname, mode_t mode) NO_IMPL(chmod)
DLL_PUBLIC int fchmod(int fd, mode_t mode) NO_IMPL(fchmod)
DLL_PUBLIC int sigfillset(sigset_t *set) {
	debug_printf("sigfillset not implemented properly\n");
	return 0;
}
DLL_PUBLIC int raise(int sig) NO_IMPL(raise)
DLL_PUBLIC pid_t getpid(void) NO_IMPL(getpid)

DLL_PUBLIC int rand_r(unsigned int *seedp) NO_IMPL(rand_r)
static unsigned long next = 1;
#define RAND_MAX 32767

int rand() {
    next = 1103515245 * next + 12345;
    return next;
}

void srand(unsigned int seed) {
    next = seed;
}


DLL_PUBLIC int sigaddset(sigset_t *set, int signum) NO_IMPL(sigaddset)
DLL_PUBLIC int sigprocmask(int how, const sigset_t * set, sigset_t * oldset) {
	debug_printf("sigfprocmask not implemented properly\n");
	return 0;
}
//DLL_PUBLIC int fstat(int fd, struct stat *statbuf) {
//	if (true || fd<=2) {
//		statbuf->st_mode = 0;
//		statbuf->st_dev = 20;
//		statbuf->st_ino = 9;
//		statbuf->st_nlink = 1;
//		statbuf->st_size = 0;
//		statbuf->st_mtim.tv_sec = 0;
//		statbuf->st_mtim.tv_nsec = 0;
//		statbuf->st_atim.tv_sec = 0;
//		statbuf->st_atim.tv_nsec = 0;
//		statbuf->st_ctim.tv_sec = 0;
//		statbuf->st_ctim.tv_nsec = 0;
//		statbuf->st_uid = 0;         /* User ID of owner */
//		statbuf->st_gid = 0;
//		return 0;
//	}
//	debug_printf("not implemented fstat\n"); \
//	abort(); \
//}

DLL_PUBLIC pid_t wait(int *wstatus) NO_IMPL(wait)
DLL_PUBLIC pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage) NO_IMPL(wait4)
DLL_PUBLIC pid_t waitpid(pid_t pid, int *wstatus, int options) NO_IMPL(waitpid)
DLL_PUBLIC int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options) NO_IMPL(waitid)
DLL_PUBLIC int kill(pid_t pid, int sig) NO_IMPL(kill)
DLL_PUBLIC int execve(const char *pathname, char *const argv[], char *const envp[]) NO_IMPL(execve)
DLL_PUBLIC int execv(const char *pathname, char *const argv[]) NO_IMPL(execv)


DLL_PUBLIC int symlink(const char *target, const char *linkpath) NO_IMPL(symlink)
DLL_PUBLIC int link(const char *oldpath, const char *newpath) NO_IMPL(link)

DLL_PUBLIC int statfs(const char *path, struct statfs *buf) NO_IMPL(statfs)
DLL_PUBLIC mode_t umask(mode_t mask) NO_IMPL(umask)

DLL_PUBLIC uint64_t getsid(pid_t pid) NO_IMPL(getsid)	
DLL_PUBLIC time_t time(time_t *timer) NO_IMPL(time)	
DLL_PUBLIC int fchown(int fd, uid_t owner, gid_t group) NO_IMPL(fchown)	
DLL_PUBLIC int fcntl(int fd, int cmd, ... /* arg */ ) NO_IMPL(fcntl)
DLL_PUBLIC int madvise(void *addr, size_t length, int advice) NO_IMPL(madvise)	
DLL_PUBLIC int getpwnam_r(const char * name, struct passwd * pwd, char * buf, size_t buflen, struct passwd ** result) NO_IMPL(getpwnam_r)
DLL_PUBLIC int fstatfs(int fd, struct statfs *buf) NO_IMPL(fstatfs)
DLL_PUBLIC void _exit(int status) {
	zwolf_exit(status);
}
DLL_PUBLIC struct tm *localtime_r(const time_t *timer, struct tm* result) NO_IMPL(localtime_r)
DLL_PUBLIC int usleep(useconds_t usec) NO_IMPL(usleep)
DLL_PUBLIC int sigaction(int signum, const struct sigaction * act, struct sigaction * oldact) {
	debug_printf("not implemented sigaction\n");
	return 0;
}
DLL_PUBLIC unsigned int alarm(unsigned int seconds) NO_IMPL(alarm)
DLL_PUBLIC struct tm *gmtime(const time_t *timer) NO_IMPL(gmtime)
DLL_PUBLIC char *asctime(const struct tm *timeptr) NO_IMPL(asctime)
DLL_PUBLIC struct tm *localtime(const time_t *timep) NO_IMPL(localtime)
DLL_PUBLIC int dladdr(const void *addr, Dl_info *info) NO_IMPL(dladdr)

//DLL_PUBLIC  NO_IMPL()

DLL_PUBLIC void perror(const char *s) {
	fprintf(stderr, "%s: errno %s", s, strerror(errno));
}

DLL_PUBLIC
int open(const char *pathname, int flags, ...) {
	uintptr_t fd;
	uint32_t ret = base_fs_open(pathname, &fd, flags);
	if (ret == 0) {
		return fd;
	} else {
		errno = ret;
		return -1;
	}
}

DLL_PUBLIC
int shm_open(const char *name, int oflag, int mode) {
	debug_printf("shm_open not implemented");
	__builtin_trap();
}

DLL_PUBLIC
char *fgets(char * s, int size, FILE * stream) {
	debug_printf("fgets not implemented");
	__builtin_trap();
}

DLL_PUBLIC
int scanf(const char *format, ...) {
	debug_printf("scanf not implemented");
	__builtin_trap();
}
DLL_PUBLIC
int mprotect(void *addr, size_t len, int prot) {
	debug_printf("mprotect not implemented");
	__builtin_trap();
}
DLL_PUBLIC
int shm_unlink(const char *name) {
	debug_printf("shm_unlink not implemented");
	__builtin_trap();
}

DLL_PUBLIC
int ferror(FILE *stream) {
	return 0;
}

