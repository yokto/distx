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
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"movq	%0, %%rdi;" // addr
			"movq	%1, %%rsi;" // length
			"movl	%2, %%edx;" // prot
			"movl	%3, %%ecx;" // flags
			"movl	%4, %%e8;" // fd
			"movq	%5, %%r9;" // offset
			"movq	$9, %%rax;" // systemcall 9 mmap
			"syscall;"
			:
			: "r" (addr), "r" (length), "r" (prot), "r" (flags), "r" (fd), "f" (offset)
	   );
	return ret;
}
