#include <fs.h>

#include "base/fs.h"
#include "common.h"
#include "stdlib.h"
#include "errno.h"
#include "fs_windows.h"
#include "fs_linux.h"
#include "string.h"
#include "unicode.h"

static bool isWin = 0;
static int32_t tonativepath(const char* filenameOrig, const char** output);

void init_fs(bool iswin, void* lib) {
	isWin = iswin;
	if (isWin) {
		win_wopen = __dlsym(lib, "_wopen");
		win_lseeki64 = __dlsym(lib, "_lseeki64");
		win_chsize_s = __dlsym(lib, "_chsize_s");
		win_read = __dlsym(lib, "_read");
		win_write = __dlsym(lib, "_write");
		win_close = __dlsym(lib, "_close");
	} else {
		linux_open = __dlsym(lib, "open");
		linux_lseek = __dlsym(lib, "lseek");
		linux_ftruncate = __dlsym(lib, "ftruncate");
		linux_read = __dlsym(lib, "read");
		linux_write = __dlsym(lib, "write");
		linux_close = __dlsym(lib, "close");
	}
}

int32_t base_fs_open(const char *path, uintptr_t* fd, uint32_t flags) {
	__debug_printf("open %s\n", path);

	uint32_t error = SUCCESS;
	char * nativepath = NULL;

	do {
		error = tonativepath(path, &nativepath);
		if (error) { break; }
		if (isWin) {
			uint32_t flag = 0;
			if ((flags & BASE_FS_OPEN_READ) && (flags & BASE_FS_OPEN_WRITE)) { flag |= WIN_O_RDWR; }
			else if (flags & BASE_FS_OPEN_READ) { flag |= WIN_O_RDONLY; }
			else if (flags & BASE_FS_OPEN_WRITE) { flag |= WIN_O_WRONLY; }
			else {
				error = EINVAL;
				break;
			}
			if (flags & BASE_FS_OPEN_APPEND) { flag |= WIN_O_APPEND; }
			if (flags & BASE_FS_OPEN_CREATE) { flag |= WIN_O_CREAT; }
			int ret = win_wopen(nativepath, flag, 0666);
			if (ret >= 0) {
				*fd = ret;
			} else {
				error = __errno();
			}
		} else {
			uint32_t flag = 0;
			if ((flags & BASE_FS_OPEN_READ) && (flags & BASE_FS_OPEN_WRITE)) { flag |= LINUX_O_RDWR; }
			else if (flags & BASE_FS_OPEN_READ) { flag |= LINUX_O_RDONLY; }
			else if (flags & BASE_FS_OPEN_WRITE) { flag |= LINUX_O_WRONLY; }
			else {
				error = EINVAL;
				break;
			}
			if (flags & BASE_FS_OPEN_APPEND) { flag |= LINUX_O_APPEND; }
			if (flags & BASE_FS_OPEN_CREATE) { flag |= LINUX_O_CREAT; }
			__debug_printf("open ret %s\n", nativepath);
			int ret = linux_open(nativepath, flag, 0666);
			__debug_printf("open ret %d\n", __errno());
			if (ret >= 0) {
				*fd = ret;
			} else {
				error = __errno();
			}
		}
	} while (false);
	if (nativepath) { free(nativepath); }
	return error;
}

int32_t base_fs_truncate(uintptr_t fd, int64_t length) {
	__debug_printf("truncate %d\n", fd);
	if (isWin) {
		int ret = win_chsize_s(fd, length);
		if (ret == 0) {
			return ret;
		} else {
			return __errno();
		}
	} else {
		int ret = linux_ftruncate(fd, length);
		if (ret == 0) {
			return ret;
		} else {
			return __errno();
		}
	}
}

int32_t base_fs_seek(uintptr_t fd, int64_t offset, uint32_t flags, int64_t* new_offset) {
	__debug_printf("seek %d\n", fd);
	if (isWin) {
		int flag;
		if (flags == BASE_FS_SEEK_CUR) { flag = WIN_SEEK_CUR; }
		else if (flags == BASE_FS_SEEK_SET) { flag = WIN_SEEK_SET; }
		else if (flags == BASE_FS_SEEK_END) { flag = WIN_SEEK_END; }
		else { return EINVAL; }
		int ret = win_lseeki64(fd, offset, flag);
		if (ret == -1) {
			return __errno();
		} else {
			*new_offset = ret;
			return SUCCESS;
		}
	} else {
		int flag;
		if (flags == BASE_FS_SEEK_CUR) { flag = LINUX_SEEK_CUR; }
		else if (flags == BASE_FS_SEEK_SET) { flag = LINUX_SEEK_SET; }
		else if (flags == BASE_FS_SEEK_END) { flag = LINUX_SEEK_END; }
		else { return EINVAL; }
		int ret = linux_lseek(fd, offset, flag);
		if (ret == -1) {
			return __errno();
		} else {
			*new_offset = ret;
			return SUCCESS;
		}
	}
}

int32_t base_fs_write(uintptr_t fd, const void *buf, uintptr_t count, uintptr_t* written) {
	__debug_printf("write %d\n", fd);
	if (isWin) {
		int ret = win_write(fd, buf, count);
		if (ret >= 0) {
			*written = ret;
			return SUCCESS;
		} else {
			return __errno();
		}
	} else {
		int ret = linux_write(fd, buf, count);
		if (ret >= 0) {
			*written = ret;
			return SUCCESS;
		} else {
			return __errno();
		}
	}
}

int32_t base_fs_read(uintptr_t fd, void *buf, uintptr_t count, uintptr_t* read) {
	__debug_printf("read %d\n", fd);
	if (isWin) {
		int ret = win_read(fd, buf, count);
		if (ret >= 0) {
			*read = ret;
			return SUCCESS;
		} else {
			return __errno();
		}
	} else {
		int ret = linux_read(fd, buf, count);
		if (ret == 0) {
			*read = 0;
			return BASE_FS_READ_EOF;
		}
		if (ret >= 0) {
			*read = ret;
			return SUCCESS;
		} else {
			return __errno();
		}
	}
}

int32_t base_fs_close(uintptr_t fd) {
	__debug_printf("close %d", fd);
	if (isWin) {
		int ret = win_close(fd);
		if (ret == 0) {
			return SUCCESS;
		}
	} else {
		int ret = linux_close(fd);
		if (ret == 0) {
			return SUCCESS;
		}
	}
	return __errno();
}

int32_t base_fs_tonativepath(const char *pathname, void* nativepath) {
	if (isWin) {
		int32_t error;
		uint16_t* native = nativepath;
		if (pathname[0] == '/' && pathname[1] != '\0' && pathname[2] == ':') { // "/C:..." -> "C:..."
			pathname++;
		}
		error = utf8to16(pathname, native);
		for (uint16_t* it = native; *it != 0; it++) {
			if (*native == '/') { *native = '\\'; }
		}
		return error;
	} else {
		strcpy(nativepath, pathname);
		return SUCCESS;
	}
}

int32_t base_fs_tonativepathlen(const char *pathname, uintptr_t* length) {
	if (isWin) {
		if (pathname[0] == '/' && pathname[1] != '\0' && pathname[2] == ':') { // "/C:..." -> "C:..."
			pathname++;
		}
		uint32_t ret = utf8to16len(pathname, length);
		*length *= 2;
		return ret;
	} else {
		*length = strlen(pathname + 1);
		return SUCCESS;
	}
}

static const char basealias[] = "/__zwolf_basedir__/";
#define basealias_len (sizeof(basealias) - 1)
static const char buildalias[] = "/__zwolf_builddir__/";
#define buildalias_len (sizeof(buildalias) - 1)
static int32_t tonativepath(const char* filenameOrig, const char** output) {
	*output = NULL;
	int filelen = strlen(filenameOrig);

	const char* filename = filenameOrig;
	
	if (strncmp(basealias, filename, basealias_len) == 0) {
		char* base = getenv("ZWOLF_BASEDIR");
		if (base) {
			int baselen = strlen(base);

			filename = __builtin_alloca(baselen + filelen - basealias_len + 1);
			memcpy(filename, base, baselen);
			memcpy(filename + baselen, filenameOrig + basealias_len, filelen - basealias_len + 1);
		}
	}
	if (strncmp(buildalias, filename, buildalias_len) == 0) {
		char* build = getenv("ZWOLF_BUILDDIR");
		if (build) {
			int buildlen = strlen(build);

			filename = __builtin_alloca(buildlen + filelen - buildalias_len + 1);
			memcpy(filename, build, buildlen);
			memcpy(filename + buildlen, filenameOrig + buildalias_len, filelen - buildalias_len + 1);
		}
	}

	uintptr_t len = 0;
	int32_t error = base_fs_tonativepathlen(filename, &len);
	if (error != SUCCESS) {
		return error;
	}
	void* nativepath = malloc(len);
	error = base_fs_tonativepath(filename, nativepath);
	if (error != SUCCESS) {
		return error;
	}
	*output = nativepath;
	return SUCCESS;
}
