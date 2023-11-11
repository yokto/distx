#include <fs.h>

#include "base/fs.h"
#include "common.h"
#include "stdlib.h"
#include "errno.h"
#include "fs_windows.h"
#include "fs_linux.h"
#include "string.h"
#include "unicode.h"

#define SUCCESS 0

static bool isWin = 0;

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
	debug_printf("open %s flags %d\n", path, flags);

	uint32_t error = SUCCESS;
	void * nativepath = NULL;

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
			debug_printf("open path %s\n", nativepath);
			int ret = linux_open(nativepath, flag, 0666);
			debug_printf("open errno %d\n", __errno());
			debug_printf("open fd %d\n", ret);
			if (ret >= 0) {
				*fd = ret;
			} else {
				error = __errno();
			}
		}
	} while (false);
	if (nativepath) { free(nativepath); }
	debug_printf("open failed with %d\n", error);
	return error;
}

int32_t base_fs_truncate(uintptr_t fd, int64_t length) {
	debug_printf("truncate %d\n", fd);
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
	debug_printf("seek %d\n", fd);
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
	debug_printf("write %d\n", fd);
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
	debug_printf("read %d\n", fd);
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
	debug_printf("close %d", fd);
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
		*length = strlen(pathname) + 1;
		return SUCCESS;
	}
}

static const char basealias[] = "/__zwolf_run__/";
#define basealias_len (sizeof(basealias) - 1)
int32_t tonativepath(const char* filenameOrig, void** output) {
	*output = NULL;
	int filelen = strlen(filenameOrig);

	const char* filename = filenameOrig;
	
	if (strncmp(basealias, filename, basealias_len) == 0) {
		char* base = getenv("ZWOLF_RUNDIR");
		if (base) {
			int baselen = strlen(base);

			char* filename_tmp = __builtin_alloca(baselen + filelen - basealias_len + 1);
			memcpy(filename_tmp, base, baselen);
			memcpy(filename_tmp + baselen, filenameOrig + basealias_len, filelen - basealias_len + 1);
			filename = filename_tmp;
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

// needs to be freed
// takes an ABSOLUTE windows path and gives back segments
int32_t alloc_windows_path(uint16_t* orig_path, char** outpath) {
	int32_t err = 0;
	uintptr_t len = 0;
	err = utf16to8len(orig_path, &len);
	if (err) { return err; }

	char * path = alloca(len + 1);
	path[0] = '/';
	err = utf16to8(orig_path, path + 1);
	if (err) { free(path); return err; }
	for (char* p = path; true; p++) {
		if (p[0] == '\\') {
			p[0] =  p[1] == '\0' ?
				'\0' :
				'/';
		}
	}
	*outpath = path;
	return err;
}
