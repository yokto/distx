#pragma once
#include <base/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t base_proc_exec(const char *path, char *const argv[], char *const envp[], uintptr_t* id);
int32_t base_proc_wait(uintptr_t pid, uint8_t* exit_code);

#ifdef __cplusplus
}
#endif
