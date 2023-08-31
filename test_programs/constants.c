#define _GNU_SOURCE 1
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>
#include <threads.h>
#include <signal.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <linux/futex.h>
#include <sys/syscall.h>      /* Definition of SYS_* constants */


#define C(name) printf("#define " #name " \t%d\n", name);
#define CS(name) printf("#define " #name " \t%s\n", name);
#define S(name) printf("#define " #name "_SZ \t%d\n", sizeof(name));

int main() {
	printf("open constants\n");
	C(O_RDONLY)
	C(O_WRONLY)
	C(O_RDWR)
	C(O_CLOEXEC)
	C(O_CREAT)
	C(O_DIRECTORY)
	C(O_EXCL)
	C(O_NOCTTY)
	C(O_TMPFILE)
	C(O_TRUNC)
	C(O_APPEND)
	C(O_ASYNC)
	C(O_CLOEXEC)
	C(O_CREAT)
	C(O_DIRECT)
	C(O_DIRECTORY)
	C(O_DSYNC)
	C(O_EXCL)
	C(O_LARGEFILE)
	C(O_NOATIME)
	C(O_NOCTTY)
	C(O_NOFOLLOW)
	C(O_NONBLOCK)
	C(S_IFDIR)
	C(S_IFREG)
	C(ENOENT)
	C(ENOTDIR)
	C(EOF)
	C(thrd_success)
	C(thrd_nomem)
	C(thrd_timedout)
	C(thrd_busy)
	C(thrd_error)
	S(FILE)
	S(mtx_t)
	S(pid_t)
	S(thrd_t)
	S(tss_t)
	S(dev_t)
        S(ino_t)
        S(mode_t)
        S(nlink_t)
        S(uid_t)
        S(gid_t)
        S(dev_t)
        S(off_t)
        S(blksize_t)
        S(blkcnt_t)
        S(struct stat)

	C(SEEK_CUR)
        C(SEEK_SET)
        C(SEEK_END)
        C(O_APPEND)
        C(O_RDONLY)
        C(O_WRONLY)
        C(O_RDWR)
        C(O_CREAT)
	C(SIG_UNBLOCK)

CS(PRIu8)
CS(PRIu16)
CS(PRIu32)
CS(PRIu64)
CS(PRIX8)
CS(PRIX16)
CS(PRIX32)
CS(PRIX64)
CS(PRIx8)
CS(PRIx16)
CS(PRIx32)
CS(PRIx64)
CS(PRIxPTR)

C(SYS_futex)
C(FUTEX_WAIT_PRIVATE)
C(FUTEX_WAKE_PRIVATE)
C(ETIMEDOUT)
C(PROT_READ)
C(PROT_WRITE)
C(MAP_PRIVATE)
C(MAP_ANONYMOUS)
}
