#include "base.h"

#define OS_LINUX 1
#define OS_WINDOWS 0

//#define UTF8TO16

#define FLAG_LINUX(f) (f == 0 ? f : f & 0x3 - 1)

int external_elfator_os();

ssize_t external_windows_c__open(const char *pathname, int32_t flags) __attribute((ms_abi));
ssize_t external_linux_c_open(const char *pathname, int32_t flags);
int32_t base_open(const char *pathname, int32_t flags) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_open(pathname, FLAG_LINUX(flags));
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c__open(pathname, FLAG_LINUX(flags));
	}
}

ssize_t external_windows_c__write(int32_t, const void* buv, size_t count) __attribute((ms_abi));
ssize_t external_linux_c_write(int32_t, const void* buv, size_t count);
ssize_t base_write(int32_t fd, const void *buf, size_t count) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_write(fd, buf, count);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c__write(fd, buf, count);
	}
}

ssize_t external_windows_c__isatty(int32_t) __attribute((ms_abi));
ssize_t external_linux_c_isatty(int32_t);
int32_t base_isatty(int32_t fd) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_isatty(fd);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c__isatty(fd);
	}
}

int external_linux_c_getpagesize();
size_t base_memory_granularity() {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_getpagesize();
	} else {
		return 0x10000; // it's probably correct but should still be fixed
	}
}

void* external_windows_c_aligned_alloc(size_t alignment, size_t size) __attribute((ms_abi));
void* external_linux_c_aligned_alloc(size_t alignment, size_t size);
void* base_aligned_alloc(size_t alignment, size_t size) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_aligned_alloc(alignment, size);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_aligned_alloc(alignment, size);
	}
}

void* external_windows_c_realloc(void* ptr, size_t size) __attribute((ms_abi));
void* external_linux_c_realloc(void* ptr, size_t size);
void* base_realloc(void* ptr, size_t size) {
	write(2, "we would really like to get rid of realloc. It ads complexity without any significant benefit");
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_realloc(ptr, size);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_realloc(ptr, size);
	}
}

void* external_windows_c_calloc(size_t n, size_t size) __attribute((ms_abi));
void* external_linux_c_calloc(size_t n, size_t size);
void* base_calloc(size_t n, size_t size) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_calloc(n, size);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_calloc(n, size);
	}
}

void external_windows_c_free(void * ptr) __attribute((ms_abi));
void external_linux_c_free(void * ptr);
void base_free(void* ptr) {
	if (external_elfator_os() == OS_LINUX) {
		return external_linux_c_free(ptr);
	} else if (external_elfator_os() == OS_WINDOWS) {
		return external_windows_c_free(ptr);
	}
}
