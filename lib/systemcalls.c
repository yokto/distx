#include "types.h"

#define O_RDONLY        0
#define O_WRONLY        1
#define O_RDWR          2

typedef unsigned int mode_t;

ssize_t read(int fd, void* buf, size_t count) {
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"movl	%1, %%edi;" // rdi;" // fd
			"movq	%2, %%rsi;" // buffer
			"movq	%3, %%rdx;" // count
			"movq	$0, %%rax;" // systemcall 0 read
			"syscall;"
			"movq	%%rax, %0;" // ret 
			: "=r" (ret)
			: "r" (fd), "r" (buf), "r" (count)
	   );
	return ret;
}

ssize_t write(int fd, void* buf, size_t count) {
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"movl	%1, %%edi;" // rdi fd
			"movq	%2, %%rsi;" // buffer
			"movq	%3, %%rdx;" // count
			"movq	$1, %%rax;" // systemcall 1 write
			"syscall;"
			"movq	%%rax, %0;" // ret 
			: "=r" (ret)
			: "r" (fd), "r" (buf), "r" (count)
	   );
	return ret;
}

void exit2(int status) {
	asm(
			"movl %0, %%edi;"
			"movq $60,%%rax;"
			"syscall;"
			:: "r" (status)
	   );
}

int open(const char *pathname, int flags, mode_t mode) {
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"movq	%1, %%rdi;" // pathname
			"movl	%2, %%esi;" // flags
			"movl	%3, %%edx;" // mode
			"movq	$2, %%rax;" // systemcall 2 open
			"syscall;"
			"movq	%%rax, %0;" // ret 
			: "=r" (ret)
			: "r" (pathname), "r" (flags), "r" (mode)
	   );
	return ret;
}

// RDI, RSI, RDX, RCX, R8, R9
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	void *ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"movq	%1, %%rdi;" // addr
			"movq	%2, %%rsi;" // length
			"movl	%3, %%edx;" // prot
			"movl	%4, %%ecx;" // flags
			"movl	%5, %%r8d;" // fd
			"movq	%6, %%r9;" // offset
			"movq	$9, %%rax;" // systemcall 9 mmap
			"syscall;"
			"movq	%%rax, %0;" // ret 
			: "=r" (ret)
			: "r" (addr), "r" (length), "r" (prot), "r" (flags), "r" (fd), "r" (offset)
	   );
	return ret;
}

#define alloca __builtin_alloca

// not really a systemcall
// only one %p argument
printf(const int fd, const char* const string, const long num) {
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
			if (pos_print >= length-1) {
				exit2(3);
				exit2(print_string[pos_print] + 1);
			}

	}
	write(fd, print_string, length);
}
