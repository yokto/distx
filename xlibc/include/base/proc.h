#pragma once
#include <base/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int32_t base_proc_exec(const char *path, const char *const argv[], const char *const envp[], uintptr_t* pid, uintptr_t* pidfd);
int32_t base_proc_wait(uintptr_t pidfd, uint8_t* exit_code);

#ifdef __cplusplus
}
#endif
