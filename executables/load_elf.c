#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <stdbool.h>

#include "../lib/elf.h"

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
	Elf64_Sym* symtab;
	char* strtab;
	Elf64_Rela* rela;
	size_t rela_count;
	Elf64_Rela* rela_plt;
	size_t rela_plt_count;
	Elf64_Rela* pltgot;
	size_t gotsz;
	Elf64_Word* hash;
	size_t symtab_count;
	char name[32]; // TODO: fix
} loaded_lib;

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
int load(char* lib_name, loaded_lib* lib) {
	memset(lib, 0, sizeof(lib));
	printf("loading library ",0);
	printf(lib_name,0);
	printf("\n", 0);
	FILE* fd = fopen(lib_name, "r");
	if (fd == 0) {
		fprintf(stderr, "can't open ", 0);
		fprintf(stderr, lib_name, 0);
		exit(-1);
	}
	ElfN_Ehdr elf_header;
	if (fread(&elf_header, sizeof(ElfN_Ehdr), 1, fd) != 1) {
		fprintf(stderr, "can't read elfheader ", 0);
		fprintf(stderr, lib_name, 0);
		exit(-1);
	}
	if (elf_header.e_phnum > 32) {
		fprintf(stderr, "can't read more than 32 program headers in ", 0);
		fprintf(stderr, lib_name, 0);
		exit(-1);
	}
	Elf64_Phdr phs[32];
	int phnum = elf_header.e_phnum;
	if (fread(phs, sizeof(Elf64_Phdr), phnum, fd) != phnum) {
		fprintf(stderr, "can't read program headers ", 0);
		fprintf(stderr, lib_name, 0);
		exit(-1);
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
		fprintf(stderr, "library seems to be empty ", 0);
		fprintf(stderr, lib_name, 0);
		return -1;
	} // nothing to do?
	// mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
	void* memory = mmap(
		0,
		roundup_pagesize(maxAddr - minAddr),
		PROT_READ,
		MAP_PRIVATE | MAP_DENYWRITE,
		fileno(fd),
		0);
	if (memory == MAP_FAILED) {
		fprintf(stderr, "can't map memory", 0);
		fprintf(stderr, lib_name, 0);
		exit(-1);
	}

	strncpy(lib->name, lib_name, 32);
	lib->base = memory;

	for (int i = 0 ; i < phnum ; i++) {
		const Elf64_Phdr *ph = &phs[i];
		if (ph->p_type == PT_LOAD) {
			const size_t maddr = rounddown_pagesize((size_t)memory + ph->p_vaddr);
			const size_t msize = roundup_pagesize((size_t)memory + ph->p_vaddr + ph->p_memsz) - maddr;
			if (ph->p_offset & (PAGE_SIZE-1) != ph->p_vaddr & (PAGE_SIZE-1)) {
				fprintf(stderr, "addresses don't match", 0);
				fprintf(stderr, lib_name, 0);
				exit(-1);
			}
			mmap(
				(void*)maddr,
				msize,
				PROT_READ | PROT_WRITE | PROT_EXEC, // todo remove wrong ones
				MAP_PRIVATE | MAP_DENYWRITE | MAP_FIXED,
				fileno(fd),
				rounddown_pagesize(ph->p_offset));
		}	
		if (ph->p_type == PT_DYNAMIC) {
			lib->dynamic = (Elf64_Dyn*)(memory + ph->p_vaddr);
			Elf64_Dyn* dynamic;
			for (dynamic = lib->dynamic; dynamic->d_tag != DT_NULL; dynamic++) {
				if (dynamic->d_tag == DT_SYMTAB) {
					lib->symtab = (Elf64_Sym*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_HASH) {
					lib->hash = (Elf64_Word*)(memory + dynamic->d_un.d_ptr);
					//printf("symtabbuckets %i\n", ((Elf64_Word*)(memory + dynamic->d_un.d_ptr))[0]);
					//printf("symtabcount %i\n", lib->symtab_count);
				}
				if (dynamic->d_tag == DT_STRTAB) {
					lib->strtab = (char*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_RELA) {
					lib->rela = (Elf64_Rela*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_RELASZ) {
					lib->rela_count = dynamic->d_un.d_val / sizeof(Elf64_Rela);
				}
				if (dynamic->d_tag == DT_PLTREL) {
					if (dynamic->d_un.d_val != DT_RELA) {
						fprintf(stderr, "can only read DT_RELA at the moment\n");
						exit(-1);
					}
				}
				if (dynamic->d_tag == DT_PLTRELSZ) {
					lib->rela_plt_count = dynamic->d_un.d_val / sizeof(Elf64_Rela);
					//printf("sz %x\n", dynamic->d_un.d_val);
				}
				if (dynamic->d_tag == DT_JMPREL) {
					lib->rela_plt = (Elf64_Rela*)(memory + dynamic->d_un.d_ptr);
					//printf("rela_plt %x\n", dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_PLTGOT) {
					lib->pltgot = (Elf64_Rela*)(memory + dynamic->d_un.d_ptr);
				}
			}
		}
	}

	return 0;
}

Elf64_Half lookup(char* name, loaded_lib* lib) {
	const uint32_t nbucket = ((uint32_t*)lib->hash)[0];
	const uint32_t nchain = ((uint32_t*)lib->hash)[1];
	if (nbucket == 0) { return 0; }
	const unsigned long hash = elf_Hash(name);

	Elf64_Half idx = lib->hash[2 + (hash % nbucket)];
	while (idx != 0) {
		if (strcmp(name, lib->strtab + lib->symtab[idx].st_name) == 0) {
			return idx;
		}
		idx = lib->hash[2 + nbucket + idx];
	}
}

void link(loaded_lib* lib) {
	loaded_lib libfoobar;
	bool libfoobar_loaded = false;
	for (int i = 0 ; i < lib->rela_count ; i++) {
		printf("offset: %lx, info %lx, addend: %lx\n", lib->rela[i].r_offset, lib->rela[i].r_info, lib->rela[i].r_addend);
	}
	for (int i = 0 ; i < lib->rela_plt_count ; i++) {
		Elf64_Rela * rela = lib->rela_plt + i;
		//printf("offset: %lx, info %lx, addend: %lx\n", rela->r_offset, rela->r_info, rela->r_addend);
		int sym = ELF64_R_SYM(rela->r_info);
		char* sym_name = lib->strtab + lib->symtab[sym].st_name;
		//printf("symbol name: %s\n", sym_name);

		if (strncmp(sym_name, "c_", 2) == 0) {
			void* libc = dlopen("libc.so.6", RTLD_NOW);
			if (!libc) {
				fprintf(stderr, "could not open libc\n");
				exit(-1);
			}
			void* val = dlsym(libc, sym_name+2);
			if (!val) {
				fprintf(stderr, "could not load getchar\n");
				exit(-1);
			}
			//printf("setting name %s, at %x from base %x to %x", sym_name, rela->r_offset, lib->base, val);
			void** offsetTableLoc = lib->base + rela->r_offset;
			*offsetTableLoc = val;
			dlclose(libc);
		}
		if (strncmp(sym_name, "foobar_", 7) == 0) {
			if (!libfoobar_loaded) {
				load("libfoobar.so", &libfoobar);
				libfoobar_loaded = true;
			}
			Elf64_Half f = lookup(sym_name, &libfoobar);
			void * f_addr = libfoobar.base + libfoobar.symtab[f].st_value;
			//printf("setting name %s, at %x from base %x to %x", sym_name, rela->r_offset, lib->base, f_addr);
			void** offsetTableLoc = lib->base + rela->r_offset;
			*offsetTableLoc = f_addr;
		}
	}



}

void main(int argc, char ** argv) {
	if (argc != 2) {
		fprintf(stderr, "./loadelf file");
	}

	loaded_lib lib;
	load(argv[1], &lib);
	link(&lib);
	Elf64_Half main_idx = lookup("main", &lib);
	//printf("main idx %d\n", main_idx);

	void * main_addr = lib.base + lib.symtab[main_idx].st_value;
	int ret = ((int (*)())main_addr)();
	//printf("main returned %d\n", ret);
	
//	FILE * fp = fopen(argv[1], "r"A)
//	if (!fp) {
//		printf("could not open %s", argv[1]);	
//	}
//
//	long ph_num;
//	Elf64_Phdr * ph;
//	void * entry;
//
//	Elf64_Addr programOffset;
//	Elf64_Addr dynamicAddr;
//	for (int i = 0; i < ph_num; i++) {
//		const Elf64_Phdr * h = ph + i;
//		if (h->p_type == PT_DYNAMIC) {
//			dynamicAddr = h->p_vaddr;
//		}
//		if (h->p_type == PT_PHDR) {
//			programOffset = (Elf64_Addr)ph - h->p_vaddr;
//		}
//	}
//
//	Elf64_Dyn* dynamic_orig = (Elf64_Dyn*)(programOffset + dynamicAddr);
//	Elf64_Dyn* dynamic;
//	void* strtab;
//	for (dynamic = dynamic_orig; dynamic->d_tag != DT_NULL; dynamic++) {
//		if (dynamic->d_tag == DT_STRTAB) {
//			strtab = programOffset + dynamic->d_un.d_ptr;
//		}
//	}
//
//	for (dynamic = dynamic_orig; dynamic->d_tag != DT_NULL; dynamic++) {
//		//printf(1, "\ntag:",0);
//		//printf(1, "%p", dynamic->d_tag);
//		//printf(1, "\nval:",0);
//		//printf(1, "%p", dynamic->d_un.d_val);
//		if (dynamic->d_tag == DT_NEEDED) {
//			loaded_lib * lib = load(strtab + dynamic->d_un.d_ptr);
//			for (Elf64_Dyn* dynamic2 = lib->dynamic; dynamic2->d_tag != DT_NULL; dynamic2++) {
//			}
//		}
//	}
//
//	asm(
//			"movq	%0, %%rsp;"
//			"jmp	%1;"
//			:
//			: "r" (stack), "r" (entry)
//	   );
//	exit(ph_num);
}
