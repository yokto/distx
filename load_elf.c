#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



// Allocates RWX memory of given size and returns a pointer to it. On failure,
// prints out the error and returns NULL.
void* alloc_executable_file(size_t size, int fd) {
	void* ptr = mmap(0, size,
			PROT_READ | PROT_EXEC,
			MAP_PRIVATE // | MAP_ANONYMOUS
			, fd, 0);
	if (ptr == (void*)-1) {
		perror("mmap");
		return NULL;
	}
	return ptr;
}

const size_t SIZE = 1024;
typedef long (*JittedFunc)(long);

// Allocates RWX memory directly.
void run_from_rwx() {
	int fd = open("text", O_RDONLY);
	if (fd <= 0) {
		printf("could not open file");
		return -1;
	}
	void* m = alloc_executable_file(SIZE, fd);

	JittedFunc func = m + 0x1105 - 0x1050;
	int result = func(2);
	printf("result = %d\n", result);
}

void main() {
	run_from_rwx();
}
