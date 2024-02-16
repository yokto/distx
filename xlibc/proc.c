#include <common.h>
#include <zwolf.h>
#include <base/types.h>
#include <unicode.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fs.h>

int (*linux_execve)(const char *pathname, char *const argv[], char *const envp[]);
uint32_t (*linux_fork)(void);
uint32_t (*linux_waitpid)(uint32_t pid, int *wstatus, int options);
#define SYS_pidfd_open 434
typedef int pid_t;
int linux_pidfd_open(pid_t pid, unsigned int flags) {
	return linux_syscall(SYS_pidfd_open, pid, flags);
}

typedef struct _STARTUPINFOW {
  uint32_t  cb;
  uint16_t* lpReserved;
  uint16_t* lpDesktop;
  uint16_t* lpTitle;
  uint32_t  dwX;
  uint32_t  dwY;
  uint32_t  dwXSize;
  uint32_t  dwYSize;
  uint32_t  dwXCountChars;
  uint32_t  dwYCountChars;
  uint32_t  dwFillAttribute;
  uint32_t  dwFlags;
  uint16_t   wShowWindow;
  uint16_t   cbReserved2;
  char* lpReserved2;
  uintptr_t hStdInput;
  uintptr_t hStdOutput;
  uintptr_t hStdError;
} STARTUPINFOW;

typedef struct _PROCESS_INFORMATION {
  uintptr_t hProcess;
  uintptr_t hThread;
  uint16_t  dwProcessId;
  uint16_t  dwThreadId;
} PROCESS_INFORMATION;

static uintptr_t (*win_create_process_w)(
  const uint16_t*      lpApplicationName,
  const uint16_t*      lpCommandLine,
  void*                lpProcessAttributes,
  void*                lpThreadAttributes,
  bool                 bInheritHandles,
  uint32_t             dwCreationFlags,
  void*                lpEnvironment,
  void*                lpCurrentDirectory,
  STARTUPINFOW*        lpStartupInfo,
  PROCESS_INFORMATION* lpProcessInformation
) __attribute((ms_abi)) = 0;
static uintptr_t (*win_WaitForSingleObject)(uintptr_t handle, uint32_t milliseconds) __attribute((ms_abi)) = 0;
static uintptr_t (*win_GetExitCodeProcess)(uintptr_t handle, uint32_t* exit_code) __attribute((ms_abi)) = 0;


static bool isWin;

void init_proc(bool iswin, void* lib, void* kernel32) {
	isWin = iswin;
	debug_printf("xxx open\n");
	if (isWin) {
		win_create_process_w = zwolf_sym(kernel32, "CreateProcessW");
		win_WaitForSingleObject = zwolf_sym(kernel32, "WaitForSingleObject");
		win_GetExitCodeProcess = zwolf_sym(kernel32, "GetExitCodeProcess");
		debug_printf("xxx open%p\n",win_create_process_w);
	} else {
		linux_fork = zwolf_sym(lib, "fork");
		linux_execve = zwolf_sym(lib, "execve");
		linux_waitpid = zwolf_sym(lib, "waitpid");
	}
}

// todo check all the errors
uint32_t wincmdlen(char * const argv[], uintptr_t* length) {
	uintptr_t count = 0; 
	uint32_t codepoint = 0;
	uint8_t codelength = 0;
	int32_t err = 0;
	while (*argv != 0) {
		for (const char * str = *argv; *str != '\0';) {
			if (*str == '\\' || *str == '"') {
				count++; // for escaping
			}
			// for the character itself
			err = utf8decode((uint8_t*)str, -1, &codelength, &codepoint);
			str += codelength;
			uint16_t encoded[2];
			err = utf16encode(codepoint, &codelength, encoded);
			count += codelength;
		}
		count++; // for spaces between arguments (last one is null not space)
		count += 2; // for quotes
		argv++;
	}
	*length = 2 * count; // twice because it's utf16
	return err;
}
int32_t wincmd(char * const argv[], uint16_t * cmd) {
	uint32_t codepoint = 0;
	uint8_t codelength = 0;
	int32_t err = 0;
	while (*argv != 0) {
		cmd[0] = (uint16_t)'"';
		cmd++;
		const char * str = *argv;
		while (*str != '\0') {
			if (*str == '\\' || *str == '"') {
				cmd[0] = (uint16_t)'\\';
				cmd++;
			}
			err = utf8decode((uint8_t*)str, -1, &codelength, &codepoint);
			str += codelength;
			err = utf16encode(codepoint, &codelength, cmd);
			cmd += codelength;
		}
		cmd[0] = (uint16_t)'"';
		cmd++;
		cmd[0] = (uint16_t)' ';
		cmd++;
		argv++;
	}
	cmd[-1] = 0; // we assume there is at least one argument
	return err;
}

DLL_PUBLIC
int32_t base_proc_exec(const char *path, char *const argv[], char *const envp[], uintptr_t* pid, uintptr_t* pidfd) {
	debug_printf("base_proc_exec %s\n", argv[0]);
	int32_t ret = 0;
	void* native_path = 0;
	ret = tonativepath(argv[0], &native_path);

	uint32_t res_pid = 0;

	debug_printf("base_proc_exec %s\n", native_path);
	if (ret != 0) { return ret; }

	if (isWin) {
		size_t count = 0;
		for (char* const* args = argv; *args != 0; args++) { count++; }
		char ** argv2 = alloca((count+2) * sizeof(char*));
		argv2[count+1] = 0;

		// make args in utf8
		argv2[0] = getenv("ZWOLF_EXECUTABLE");
		uintptr_t len = 0;
		ret = utf16to8len(native_path, &len);
		argv2[1] = alloca(len + 1);
		ret = utf16to8(native_path, argv2[1]);
		for (size_t i = 1 ; i < count; i++) {
			argv2[i+1] = argv[i];
		}

		wincmdlen(argv2, &len);
		uint16_t * cmd = alloca(len);
		memset(cmd, 0 , len);
		wincmd(argv2, cmd);

		// just for debugging
		ret = utf16to8len(cmd, &len);
		char * cmdutf8 = alloca(len + 1);
		ret = utf16to8(cmd, cmdutf8);
		debug_printf("assembled command %s\n", cmdutf8);

		ret = utf8to16len(argv2[0], &len);
		uint16_t * file = alloca(2*len + 2);
		ret = utf8to16(argv2[0], file);

		STARTUPINFOW startup;
		memset(&startup, 0, sizeof(STARTUPINFOW));
		startup.cb = sizeof(STARTUPINFOW);
		PROCESS_INFORMATION info;
		memset(&info, 0, sizeof(PROCESS_INFORMATION));
		debug_printf("method %p\n\n\n\n", win_create_process_w);

		ret = win_create_process_w(
			file,
			cmd,
			0,
			0,
			true,
			0,
			0,
			0,
			&startup,
			&info
			);
		if (pidfd != 0) {
			*pidfd = info.hProcess;
		}
		if (pid != 0) {
			*pid = info.dwProcessId;
		}
		res_pid = info.dwProcessId;
		debug_printf("ret %d\n", ret);
		debug_printf("errno %d\n", zwolf_errno());
	} else { // linux
		res_pid = linux_fork();
		if (res_pid == 0) {
			size_t count = 0;
			for (char* const* args = argv; *args != 0; args++) { count++; }
			char ** argv2 = alloca((count+2) * sizeof(char*));
			argv2[count+1] = 0;
			argv2[1] = native_path;
			argv2[0] = getenv("ZWOLF_EXECUTABLE");
			for (size_t c = 1; c < count; c++) { argv2[c+1] = argv[c]; }
			for (size_t c = 0; c < count; c++) { debug_printf("arg %s\n", argv2[c]); }
			//for (char* const* env = envp; *env != 0; env++) {  debug_printf("env %s\n", env); }
			ret = linux_execve(argv2[0], argv2, envp);
			debug_printf("exec failed with code %d\n", ret);
			__builtin_trap();
		} else {
			if (pid != 0) {
				*pid = res_pid;
			}
			if (pidfd != 0) {
				*pidfd = res_pid;
			}
			//*pidfd = linux_pidfd_open(res_pid, 0);
			ret = 0;
		}
	}
	debug_printf("pid %d\n", res_pid);
	free(native_path);
	return ret;
}

DLL_PUBLIC
int32_t base_proc_wait(uintptr_t pid, uint8_t* exit_code) {
	int32_t err = 0;
	if (isWin) {
		debug_printf("wait for %p\n", pid);
		debug_printf("wait for %p\n", win_WaitForSingleObject);
		win_WaitForSingleObject(pid, -1);
		debug_printf("wait win \n");
		uint32_t exitCode;
		win_GetExitCodeProcess(pid, &exitCode);
		debug_printf("exit %d\n", exitCode);
		*exit_code = (uint8_t)exitCode;
	} else {
		int status = 0;
		uint32_t err = linux_waitpid(pid, &status, 0);
		if (err != pid) {
			err = EINVAL;
		} else {
			*exit_code = (uint8_t)((0xff00 & status) >> 8);
		}
	}
	debug_printf("exit code %d\n", *exit_code);
	return err;
}
