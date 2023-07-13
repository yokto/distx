#define _GNU_SOURCE 1
#include <fcntl.h>
#include <stdio.h>
#include <threads.h>

#define C(name) printf("#define " #name " \t%d\n", name);
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
	S(FILE)
	S(mtx_t)
}
