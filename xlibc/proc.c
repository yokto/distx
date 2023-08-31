#include <common.h>
#include <base/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

int (*linux_execve)(const char *pathname, char *const argv[], char *const envp[]);
uint32_t (*linux_fork)(void);
uint32_t (*linux_waitpid)(uint32_t pid, int *wstatus, int options);

static bool isWin;

void init_proc(bool iswin, void* lib) {
	isWin = iswin;
	if (isWin) {
//		win_wopen = __dlsym(lib, "_wopen");
	} else {
		linux_fork = __dlsym(lib, "fork");
		linux_execve = __dlsym(lib, "execve");
		linux_waitpid = __dlsym(lib, "waitpid");
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
			size_t count = 0;
			for (char* const* args = argv; *args != 0; args++) { count++; }
			char ** argv2 = alloca((count+2) * sizeof(char*));
			argv2[count+1] = 0;
			argv2[0] = getenv("ZWOLF_EXECUTABLE");
			for (size_t c = 0; c < count; c++) { argv2[c+1] = argv[c]; }
			for (size_t c = 0; c <= count; c++) { __debug_printf("arg %s\n", argv2[c]); }
			return linux_execve(argv2[0], argv2, envp);
		} else {
			*id = pid;
			return 0;
		}
	}
}

DLL_PUBLIC
int32_t base_proc_wait(uintptr_t pid, uint8_t* exit_code) {
	if (isWin) {
		__debug_printf("wait win \n");
		__builtin_trap();
	} else {
		int status = 0;
		uint32_t ret = linux_waitpid(pid, &status, 0);
		if (ret != pid) {
			return EINVAL;
		} else {
			*exit_code = (uint8_t)status;
			return 0;
		}
	}
}
