#pragma once

#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    P_PID,  // Process ID
    P_LWP   // Lightweight process (thread) ID
    // Other idtype_t values may be defined based on the system
} idtype_t;

typedef struct {
    int si_signo;       // Signal number
    int si_errno;       // An errno value associated with the signal
    int si_code;        // Signal code
    pid_t si_pid;       // Sending process ID
    uid_t si_uid;       // Real user ID of sending process
    // Other signal-specific data may be defined here
} siginfo_t;


struct rusage {
	struct timeval ru_utime;
	uint64_t ru_maxrss;
	struct timeval ru_stime;
};
#define WNOHANG 1

pid_t wait(int *wstatus);
pid_t wait4(pid_t pid, int *wstatus, int options,
		struct rusage *rusage);
pid_t waitpid(pid_t pid, int *wstatus, int options);

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
/* This is the glibc and POSIX interface; see
   NOTES for information on the raw system call. */


#define WTERMSIG(status) (15) // sigterm
#define WIFSIGNALED(status) (0)
#define WEXITSTATUS(status) (0)
#define WIFEXITED(wstatus) true

#ifdef __cplusplus
}
#endif
