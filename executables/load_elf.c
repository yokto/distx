//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/mman.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include "../lib/systemcalls.c"
#include "../lib/elf.h"

typedef struct loaded_lib {
	int64_t addr;
	int foo;
} loaded_lib;

static const int loaded_lib_max = 100;
static int loaded_lib_next = 0; 
static const loaded_lib loaded_libs[100];


//// Allocates RWX memory of given size and returns a pointer to it. On failure,
//// prints out the error and returns NULL.
//void* alloc_executable_file(size_t size, int fd) {
//	void* ptr = mmap(0, size,
//			PROT_READ | PROT_EXEC,
//			MAP_PRIVATE // | MAP_ANONYMOUS
//			, fd, 0);
//	if (ptr == (void*)-1) {
//		perror("mmap");
//		return NULL;
//	}
//	return ptr;
//}
//
//const size_t SIZE = 1024;
//typedef long (*JittedFunc)(long);
//
//// Allocates RWX memory directly.
//void run_from_rwx() {
//	int fd = open("text", O_RDONLY);
//	if (fd <= 0) {
//		printf("could not open file");
//		return -1;
//	}
//	void* m = alloc_executable_file(SIZE, fd);
//
//	JittedFunc func = m + 0x1105 - 0x1050;
//	int result = func(2);
//	printf("result = %d\n", result);
//}

void _start() {
//	if (argc != 2) {
//		write(2, "./load_elf <file>", 17);
//		exit2(-1);
//	}
	
	ElfN_Ehdr* elf_hdr = (ElfN_Ehdr *)0x400000;

	for (Elf64_Phdr* phdr = (elf_hdr)->e_phoff;
			e_phoff < e_phoff + e_phnum * sizeof(Elf64_Phdr);
			phdr += 1) {

	}
	
	asm(
			"jmpq	%0;" // pathname
			:
			: "r" (elf_hdr->e_entry)
	   );
	//run_from_rwx();
}
