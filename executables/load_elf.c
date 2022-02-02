//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/mman.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
#include "../lib/systemcalls.c"
#include "../lib/elf.h"
#include "../lib/auxv.h"

static const size_t MAX_SIZE_T = 0xffffffffffffffff;
static const size_t PAGE_SIZE = 0x1000;

static size_t roundup_pagesize(size_t s) {
	return ((s - 1 + PAGE_SIZE) & (~(PAGE_SIZE-1)));
}
static size_t rounddown_pagesize(size_t s) {
	return (s & (~(PAGE_SIZE-1)));
}

typedef struct loaded_lib {
	void* base;
	Elf64_Dyn* dynamic;
	char name[32]; // TODO: fix
} loaded_lib;

static const int loaded_lib_max = 100;
static int loaded_lib_next = 0; 
static loaded_lib loaded_libs[100];


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
loaded_lib* load(char* lib_name) {
	printf(1, "loading library ",0);
	printf(1, lib_name,0);
	printf(1, "\n", 0);
	const int fd = open(lib_name, O_RDONLY, 0);
	if (fd < 0) {
		printf(2, "can't open ", 0);
		printf(2, lib_name, 0);
		exit2(-1);
	}
	ElfN_Ehdr elf_header;
	if (read(fd, &elf_header, sizeof(ElfN_Ehdr)) != sizeof(ElfN_Ehdr)) {
		printf(2, "can't read elfheader ", 0);
		printf(2, lib_name, 0);
		exit2(-1);
	}
	if (elf_header.e_phnum > 32) {
		printf(2, "can't read more than 32 program headers in ", 0);
		printf(2, lib_name, 0);
		exit2(-1);
	}
	Elf64_Phdr phs[32];
	int phnum = elf_header.e_phnum;
	if (read(fd, phs, sizeof(Elf64_Phdr) * phnum) != sizeof(Elf64_Phdr) * phnum) {
		printf(2, "can't read program headers ", 0);
		printf(2, lib_name, 0);
		exit2(-1);
	}
	size_t maxAddr = 0;
	size_t minAddr = MAX_SIZE_T;
	for (int i = 0 ; i < phnum ; i++) {
		const Elf64_Phdr *ph = &phs[i];
		if (ph->p_type == PT_LOAD) {
			size_t max = ph->p_vaddr + ph->p_memsz;
			size_t min = ph->p_vaddr;
			maxAddr = max > maxAddr ? max : maxAddr;
			minAddr = min < minAddr ? min : minAddr;
		}	
	}
	if (maxAddr <= minAddr) {
		printf(2, "library seems to be empty ", 0);
		printf(2, lib_name, 0);
		return;
	} // nothing to do?
	// mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	void* memory = mmap(
		0,
		roundup_pagesize(maxAddr - minAddr),
		PROT_READ,
		MAP_PRIVATE | MAP_DENYWRITE,
		fd,
		0);
	if (memory == -1) {
		printf(2, "can't map memory", 0);
		printf(2, lib_name, 0);
		exit2(-1);
	}

	loaded_lib* lib = &loaded_libs[loaded_lib_next++];
	strncpy(&lib->name, lib_name, 32);
	lib->base = memory;

	for (int i = 0 ; i < phnum ; i++) {
		const Elf64_Phdr *ph = &phs[i];
		if (ph->p_type == PT_LOAD) {
			const size_t maddr = rounddown_pagesize(memory + ph->p_vaddr);
			const size_t msize = roundup_pagesize(memory + ph->p_vaddr + ph->p_memsz) - maddr;
			if (ph->p_offset & (PAGE_SIZE-1) != ph->p_vaddr & (PAGE_SIZE-1)) {
				printf(2, "addresses don't match", 0);
				printf(2, lib_name, 0);
				exit2(-1);
			}
			mmap(
				maddr,
				msize,
				PROT_READ | PROT_WRITE | PROT_EXEC, // todo remove wrong ones
				MAP_PRIVATE | MAP_DENYWRITE | MAP_FIXED,
				fd,
				rounddown_pagesize(ph->p_offset));
		}	
		if (ph->p_type == PT_DYNAMIC) {
			lib->dynamic = (Elf64_Dyn*)(memory + ph->p_vaddr);
		}
	}

	return lib;
}

void _start() {
	long * stack;
	asm(
			"movq	%%rbp, %0;"
			: "r=" (stack)
			:
	   );
	//exit2((int)stack[1]);
	long * p = stack + 1; // point to argc
	p++; // point to argv
	while (*p) {
		p++;
	}
	p++; // point to envp
	while (*p) { // loop through env
		p++;
	}
	p++;
	while (!(*p)) { // loop through padding
		p++;
	}

	long ph_num;
	Elf64_Phdr * ph;
	void * entry;
	do { // loop through aux vector
		if (*p == AT_ENTRY) { // entry point key
			entry = (void*)*(p+1);
		}
		p += 2;
		if (*p == AT_PHNUM) {
			ph_num = *(p+1);
		}
		if (*p == AT_PHDR) {
			ph = (void*)*(p+1);
		}
	} while(*p);

	Elf64_Addr programOffset;
	Elf64_Addr dynamicAddr;
	for (int i = 0; i < ph_num; i++) {
		const Elf64_Phdr * h = ph + i;
		if (h->p_type == PT_DYNAMIC) {
			dynamicAddr = h->p_vaddr;
		}
		if (h->p_type == PT_PHDR) {
			programOffset = (Elf64_Addr)ph - h->p_vaddr;
		}
	}

	Elf64_Dyn* dynamic_orig = (Elf64_Dyn*)(programOffset + dynamicAddr);
	Elf64_Dyn* dynamic;
	void* strtab;
	for (dynamic = dynamic_orig; dynamic->d_tag != DT_NULL; dynamic++) {
		if (dynamic->d_tag == DT_STRTAB) {
			strtab = programOffset + dynamic->d_un.d_ptr;
		}
	}

	for (dynamic = dynamic_orig; dynamic->d_tag != DT_NULL; dynamic++) {
		//printf(1, "\ntag:",0);
		//printf(1, "%p", dynamic->d_tag);
		//printf(1, "\nval:",0);
		//printf(1, "%p", dynamic->d_un.d_val);
		if (dynamic->d_tag == DT_NEEDED) {
			loaded_lib * lib = load(strtab + dynamic->d_un.d_ptr);
			for (Elf64_Dyn* dynamic2 = lib->dynamic; dynamic2->d_tag != DT_NULL; dynamic2++) {
			}
		}
	}

	asm(
			"movq	%0, %%rsp;"
			"jmp	%1;"
			:
			: "r" (stack), "r" (entry)
	   );
	exit2(ph_num);
}
