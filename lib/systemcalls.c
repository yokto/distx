#include "types.h"

#define O_RDONLY        0
#define O_WRONLY        1
#define O_RDWR          2

typedef unsigned int mode_t;

ssize_t read(int fd, void* buf, size_t count) {
	ssize_t ret;
	asm volatile(
			"syscall"
			: "=a" (ret)
			: "0" (0), "D" (fd), "S" (buf), "d" (count)
		        : "rcx", "r11", "memory"
	   );
	return ret;
}

ssize_t write(int fd, void* buf, size_t count) {
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm volatile(
			"syscall"
			: "=a" (ret)
			: "0" (1), "D" (fd), "S" (buf), "d" (count)
		        : "rcx", "r11", "memory"
	   );
	return ret;
}

void exit2(int status) {
	long ret;
	asm(
			"syscall"
			: "=a" (ret)
			: "0" (0x60), "D" (status)
		        : "rcx", "r11", "memory"
	   );
}

int open(const char *pathname, int flags, mode_t mode) {
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"syscall"
			: "=a" (ret)
			: "0" (2), "D" (pathname), "S" (flags), "d" (mode)
	   );
	return ret;
}

#define PROT_READ 0x1
#define MAP_PRIVATE 0x2
#define MAP_DENYWRITE 0x800

// RDI, RSI, RDX, RCX, R8, R9
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	void *ret;
	register long r10 asm("r10") = flags;
	register long r8 asm("r8") = fd;
	register long r9 asm("r9") = offset;
	asm(
			"syscall"
			: "=a" (ret)
			: "0" (9), "D" (addr), "S" (length), "d" (prot), "r" (r10), "r" (r8), "r" (r9)
		        : "rcx", "r11", "memory"
	   );
	return ret;
}

#define alloca __builtin_alloca

// not really a systemcall
// only one %p argument
void printf(const int fd, const char* const string, const long num) {
	int length = 0;
	int additional = 0;
	while (string[length] != '\0') {
		if (string[length] == '%' && string[length+1] == 'p') {
			additional += 16;
		}
		length++;
	}
	length ++; // for '\0'
	length += additional;

	char print_string[length];

	int pos_str = 0;
	int pos_print = 0;
	while (pos_print < length) {
		if (string[pos_str] != '%' || string[pos_str+1] != 'p') {
			print_string[pos_print] = string[pos_str];
			pos_str ++;
			pos_print ++;
			continue;
		}
		print_string[pos_print] = '0';
		print_string[pos_print+1] = 'x';
		for (int i = 0; i < 16 ; i++) {
			int n = (num >> (i * 4)) & 15;
			if (n <= 9) {
				print_string[pos_print+17-i] = (char)n + '0';
			} else {
				print_string[pos_print+17-i] = (char)(n-10) + 'a';
			}
		}
		pos_str += 2;
		pos_print += 18;
//			if (pos_print >= length-1) {
//				exit2(3);
//				exit2(print_string[pos_print] + 1);
//			}

	}
	write(fd, print_string, length);
}
