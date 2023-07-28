#include <errno.h>
#include <stdio.h>

int main() {
	errno = 1;
	printf("errno %d", errno);
	return 0;
}
