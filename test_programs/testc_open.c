#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	fopen("/", "r");
	fopen("/foo/", "r");
	fopen("/bin/", "r");
	fopen("/_zwolf/../..", "r");
	fopen("/_zwolf/bar", "r");
	fopen("/_zwolf/..", "r");
	open("foo-noexist", O_RDWR);
	printf("errno %d \n", errno);
	return 0;
}

