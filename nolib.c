typedef long ssize_t; // bytecount or error
typedef long size_t; // bytecount

const char* const hello_world = "hello world";

ssize_t read(int fd, void* buf, size_t count) {
	ssize_t ret;
	/* ssize_t read(int fd, const void *buf, size_t count) */
	asm(
			"movq	$0, %%rax;" // systemcall 0 read
			"movl	%1, %%edi;" // rdi;" // fd
			"movq	%2, %%rsi;" // buffer
			"movq	%3, %%rdx;" // count
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
			"movq	$1, %%rax;" // systemcall 0 read
			"movl	%1, %%edi;" // rdi;" // fd
			"movq	%2, %%rsi;" // buffer
			"movq	%3, %%rdx;" // count
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

void _start() {

	const int written = write(1, (void*)hello_world, 11);

	char c;
      	read(0, &c, 1);

	exit2(written);
}

