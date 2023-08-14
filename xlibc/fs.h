#pragma once

#include <stdbool.h>
#include <base/types.h>

void init_fs(bool iswin, void * lib);

/**
 * Translates a utf8, slash separated path to a native system path and writes it into native_path
 * returns 0 on success.
 * returns a positive error number otherwise.
 */
int32_t base_fs_tonativepath(const char *pathname, void* nativepath);
/**
 * Same as base_fs_tonativepath. Except it only calculates the length.
 */
int32_t base_fs_tonativepathlen(const char *pathname, uintptr_t* length);

int32_t base_fs_fromnativepath(const void* nativepath, const char* pathname);
/**
 * Same as base_fs_tonativepath. Except it only calculates the length.
 */
int32_t base_fs_fromnativepathlen(const void* nativepath, uintptr_t* length);
