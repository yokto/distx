#pragma once
#include <base/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t base_proc_exec(const char *path, char *const argv[], char *const envp[], uintptr_t* id);

#ifdef __cplusplus
}
#endif
