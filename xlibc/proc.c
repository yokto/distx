#include <common.h>
#include <base/types.h>
#include <stdbool.h>

int (*linux_execve)(const char *pathname, char *const argv[], char *const envp[]);
uint32_t (*linux_fork)(void);

static bool isWin;

void init_proc(bool iswin, void* lib) {
	isWin = iswin;
	if (isWin) {
//		win_wopen = __dlsym(lib, "_wopen");
	} else {
		linux_fork = __dlsym(lib, "fork");
		linux_execve = __dlsym(lib, "execve");
	}
}

DLL_PUBLIC
int32_t base_proc_exec(const char *path, char *const argv[], char *const envp[], uintptr_t* id) {
	if (isWin) {
		__debug_printf("exec win \n");
		__builtin_trap();
	} else {
		uint32_t pid = linux_fork();
		if (pid == 0) {
			return linux_execve(path, argv, envp);
		} else {
			*id = pid;
			return 0;
		}
	}
}
