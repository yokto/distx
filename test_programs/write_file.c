#include "../lib/systemcalls.c"

void _start() {
	int fd = open("foo", O_WRONLY, 0);
	write(fd, "foo", 3);
	exit2(fd);
}
