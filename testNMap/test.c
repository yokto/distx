#include <sys/mman.h>
#include <fcntl.h>

#include <stdio.h>

int main() {
	int length = 582945520;
	const int fd = open("/home/silvio/Downloads/WildCard.mp3", O_RDONLY);
	if (fd < 0) { return -1; }
	const char* addr = mmap(0, 582945520, PROT_READ, MAP_PRIVATE, fd, 0);
	printf("allocated %p", addr);
	char res = 0;
	for (int i = 0; i < length ; i = i + 4096) {
		res += addr[i];
	}
	getchar();
	return 42;
}
