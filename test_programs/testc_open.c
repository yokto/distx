#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	open("foo-noexist", O_RDWR);
	printf("errno %d \n", errno);
	return 0;
}

