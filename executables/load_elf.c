#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#if defined(WIN32)
	#include <memoryapi.h>
	#include <windows.h>
	#include <sysinfoapi.h>
#else
	#include <sys/mman.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <dlfcn.h>
#endif

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
	Elf64_Half* versym;
	Elf64_Verdef* verdef;
	Elf64_Verneed* verneed;
	char* strtab;
	Elf64_Rela* rela;
	size_t rela_count;
	Elf64_Rela* rela_plt;
	size_t rela_plt_count;
	Elf64_Rela* pltgot;
	size_t gotsz;
	Elf64_Word* hash;
	size_t symtab_count;
	char* name;
	char* path;
	size_t init_arraysz;
	size_t* init_array;
	size_t fini_arraysz;
	size_t* fini_array;
} loaded_lib;

typedef struct loaded_libs {
	loaded_lib * libs;
	int libs_count;
	int vec_length;
} loaded_libs;

__attribute__((sysv_abi)) uint32_t get_os() {
#if defined(WIN32)
	return 0;
#else
	return 1;
#endif
}

int alloc_loaded_libs(loaded_libs* libs) {
	libs->libs = calloc(8, sizeof(loaded_lib));
	libs->libs_count = 0;
	libs->vec_length = 8;
}

int free_loaded_libs(loaded_libs* libs) {
	free(libs->libs);
	libs->libs = 0;
	libs->libs_count = 0;
	libs->vec_length = 0;
}

loaded_lib* add_loaded_lib(loaded_libs* libs) {
	if (libs->libs_count >= libs->vec_length) {
		int new_vec_length = libs->vec_length * 2;
		loaded_lib* new_libs = calloc(new_vec_length, sizeof(loaded_lib));
		memcpy(new_libs, libs->libs, libs->vec_length * sizeof(loaded_lib));
		free(libs->libs);
		libs->libs = new_libs;
		libs->vec_length = new_vec_length;
	}
	libs->libs_count++;
	return &libs->libs[libs->libs_count-1];
}

void * reserve_memory(size_t size) {
	size = roundup_pagesize(size);
#if defined(WIN32)
// unfortunately we cant use this because of dwAllocationGranularity
//	void* memory = VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
//	printf("foo %p\n\n\n", memory);
//	fflush(stdout);
//	VirtualFree(memory, size, MEM_RELEASE);
	void* memory = VirtualAlloc(0, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (memory == 0) {
		fprintf(stderr, "can't map memory");
		return 0;
	}
#else
	void* memory = mmap(
			0,
			size,
			PROT_READ,
			MAP_PRIVATE | MAP_DENYWRITE | MAP_ANONYMOUS,
			-1,
			0);
	if (memory == MAP_FAILED) {
		fprintf(stderr, "can't map memory\n");
		return 0;
	}
#endif
	return memory;
}


loaded_lib* load(char* lib_path, loaded_libs* libs) {
	for (int i = 0 ; i < libs->libs_count ; i++) {
		if (libs->libs[i].path && strcmp(libs->libs[i].path, lib_path) == 0) {
			printf("already loaded library %s\n", lib_path);
					fflush(stdout);
			return &libs->libs[i];
		}
	}
	loaded_lib* lib = add_loaded_lib(libs);
	memset(lib, 0, sizeof(loaded_lib));
	printf("loading library %s\n", lib_path);

	// needs to happen before recursive call to load
	lib->path = malloc(strlen(lib_path) + 1);
	strcpy(lib->path, lib_path);

	FILE* fd = fopen(lib_path, "rb");
	if (fd == 0) {
		fprintf(stderr, "can't open %s", lib_path);
		exit(-1);
	}
	ElfN_Ehdr elf_header;
	if (fread(&elf_header, sizeof(ElfN_Ehdr), 1, fd) != 1) {
		fprintf(stderr, "can't read elfheader %s", lib_path);
		exit(-1);
	}
	if (elf_header.e_phnum > 32) {
		fprintf(stderr, "can't read more than 32 program headers in %s", lib_path);
		exit(-1);
	}
	Elf64_Phdr phs[32];
	int phnum = elf_header.e_phnum;
	if (fread(phs, sizeof(Elf64_Phdr), phnum, fd) != phnum) {
		fprintf(stderr, "can't read program headers %s", lib_path);
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
		fprintf(stderr, "library seems to be empty %s", lib_path);
		return 0;
	} // nothing to do?

#if defined(WIN32)
	// unfortunately we cant use this because of dwAllocationGranularity
	//HANDLE h = CreateFile(lib_path, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE mapping = CreateFileMapping(
	//		h,
	//		0,
	//		PAGE_EXECUTE_WRITECOPY,
	//		0,
	//		0,
	//		0
	//		);
#endif

	void * memory = reserve_memory(maxAddr - minAddr);
	lib->base = memory;

	for (int i = 0 ; i < phnum ; i++) {
		const Elf64_Phdr *ph = &phs[i];
		if (ph->p_type == PT_LOAD) {
			const size_t maddr = rounddown_pagesize((size_t)memory + ph->p_vaddr);
			const size_t msize = roundup_pagesize((size_t)memory + ph->p_vaddr + ph->p_memsz) - maddr;
			const size_t file_offset = rounddown_pagesize(ph->p_offset);
			if (ph->p_offset & (PAGE_SIZE-1) != ph->p_vaddr & (PAGE_SIZE-1)) {
				fprintf(stderr, "addresses don't match %s", lib_path);
				exit(-1);
			}
#if defined(WIN32)
// unfortunately we cant use this because of dwAllocationGranularity
//			void* m = MapViewOfFileEx(
//					mapping,
//					FILE_MAP_READ,
//					file_offset >> 32,
//					file_offset & 0xffffffff,
//					msize,
//					maddr 
//					);
//
			if (fseek(fd, ph->p_offset, SEEK_SET) != 0) {
				fprintf(stderr, "can't seek in %s\n", lib_path);
				exit(-1);
			}
			if (fread((void*) memory + ph->p_vaddr, ph->p_filesz, 1, fd) != 1) {
				fprintf(stderr, "can't read section %s\n", lib_path);
				exit(-1);
			}
#else
			mmap(
				(void*)maddr,
				msize,
				PROT_READ | PROT_WRITE | PROT_EXEC, // todo remove wrong ones
				MAP_PRIVATE | MAP_DENYWRITE | MAP_FIXED,
				fileno(fd),
				file_offset);
#endif
		}	
		if (ph->p_type == PT_DYNAMIC) {
			lib->dynamic = (Elf64_Dyn*)(memory + ph->p_vaddr);
			Elf64_Dyn* dynamic;
			for (dynamic = lib->dynamic; dynamic->d_tag != DT_NULL; dynamic++) {
				if (dynamic->d_tag == DT_STRTAB) {
					lib->strtab = (char*)(memory + dynamic->d_un.d_ptr);
				}
			}

			for (dynamic = lib->dynamic; dynamic->d_tag != DT_NULL; dynamic++) {
				if (dynamic->d_tag == DT_SYMTAB) {
					lib->symtab = (Elf64_Sym*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_SONAME) {
					char * soname = lib->strtab + dynamic->d_un.d_ptr;
					size_t lib_name_len = strchr(soname, ':') - soname;
					lib->name = malloc(lib_name_len + 1);
					strncpy(lib->name, soname, lib_name_len);
					lib->name[lib_name_len] = '\0';
				}
				if (dynamic->d_tag == DT_HASH) {
					lib->hash = (Elf64_Word*)(memory + dynamic->d_un.d_ptr);
					//printf("symtabbuckets %i\n", ((Elf64_Word*)(memory + dynamic->d_un.d_ptr))[0]);
					//printf("symtabcount %i\n", lib->symtab_count);
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
				if (dynamic->d_tag == DT_NEEDED) {
					char* needed = lib->strtab + dynamic->d_un.d_ptr;
					char* colon = strchr(needed, ':');
					if (!colon) {
						fprintf(stderr, "library's soname has no colon %s\n", needed);
						//exit(-1);
					} else {
						loaded_lib * lib = load(colon + 1, libs);
					}
				}
				if (dynamic->d_tag == DT_VERSYM) {
					lib->versym = (Elf64_Half*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_VERNEED) {
					lib->verneed = (Elf64_Verneed*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_VERDEF) {
					lib->verdef = (Elf64_Verdef*)(memory + dynamic->d_un.d_ptr);
				}
				if (dynamic->d_tag == DT_INIT_ARRAY) {
					lib->init_array = memory + dynamic->d_un.d_ptr;
				}
				if (dynamic->d_tag == DT_INIT_ARRAYSZ) {
					lib->init_arraysz = dynamic->d_un.d_val;
				}
				if (dynamic->d_tag == DT_FINI_ARRAY) {
					lib->fini_array = memory + dynamic->d_un.d_ptr;
				}
				if (dynamic->d_tag == DT_FINI_ARRAYSZ) {
					lib->fini_arraysz = dynamic->d_un.d_val;
				}
			}
		}
	}

	if (!lib->name) {
		fprintf(stderr, "library %s has no soname", lib_path);
	}

	return lib;
}

// find the version string of symbol
// this are some truely ugly structures
char * verdefstr(size_t sym_idx, loaded_lib* lib) {
	Elf64_Verdef * v = lib->verdef;
	for(int i = 1 ; i < lib->versym[sym_idx] ; i++) {
		v = (void*)v + v->vd_next;
	}
	Elf64_Verdaux * aux = (Elf64_Verdaux*)((void*)v + v->vd_aux);

	char * ver = &lib->strtab[aux->vda_name];
	//printf("version name %s\n", v);
	return ver;
}

// find the version string of symbol
// this are some truely ugly structures
char * verneedstr(size_t sym_idx, loaded_lib* lib) {
	Elf64_Verneed * v = lib->verneed;
	printf("needed index %zd residex %d \n", sym_idx, lib->versym[sym_idx]);

	size_t version = lib->versym[sym_idx];
	while (true) {
		Elf64_Vernaux * aux = (Elf64_Vernaux*)((void*)v + v->vn_aux);
		while (true) {
			if (aux->vna_other == version) {
				return &lib->strtab[aux->vna_name];
			}
			if (aux->vna_next == 0) { break; }
			aux = (void*)aux + aux->vna_next;
		}

		if (v->vn_next == 0) { break; }
		v = (void*)v + v->vn_next;
	}
}

Elf64_Half lookup(char* name, char * version, loaded_lib* lib) {
	const uint32_t nbucket = ((uint32_t*)lib->hash)[0];
	const uint32_t nchain = ((uint32_t*)lib->hash)[1];
	if (nbucket == 0) { return 0; }
	const unsigned long hash = elf_Hash(name);

	Elf64_Half idx = lib->hash[2 + (hash % nbucket)];
	printf("looking for %s in library %s\n",name, lib->name);
	while (idx != 0) {
		printf("name %s, foundname %s\n", name, lib->strtab + lib->symtab[idx].st_name);
		if (strcmp(name, lib->strtab + lib->symtab[idx].st_name) == 0) {
			char* version2 = verdefstr(idx, lib);
			//printf("wanted version %s, existing version %s\n", version, version2);
			if (!version || strcmp(version2, version) == 0) {
				//printf("found version %s\n", version, version2);
				return idx;
			}
		}
		idx = lib->hash[2 + nbucket + idx];
	}
}


void* dlopen2(char* name) {
#if defined(WIN32)
	printf("getting lib %s\n", name);
	return LoadLibrary("msvcrt.dll");
#else
	return dlopen(name, RTLD_NOW);
#endif
}

void* dlsym2(void* lib, char* name) {
#if defined(WIN32)
	printf("getting sym %s from lib %p\n", name, lib);
	return GetProcAddress(lib, name);
#else
	return dlsym(lib, name);
#endif
}

void dlclose2(void* lib) {
#if defined(WIN32)
	return;
#else
	dlclose(lib);
	return;
#endif
}

#if defined(WIN32)
#define EXTERNAL_PREFIX "external_windows_c_"
#define EXTERNAL_PREFIX_LEN 19
#else
#define EXTERNAL_PREFIX "external_linux_c_"
#define EXTERNAL_PREFIX_LEN 17
#endif

void* findSymbol(Elf64_Rela * rela, loaded_lib * lib, loaded_libs * libs) {
	int sym = ELF64_R_SYM(rela->r_info);
	int type = ELF64_R_TYPE(rela->r_info);

	//printf("rela info %lx, offset %lx, addend %lx, sym %lx, type %lx\n", rela->r_info, rela->r_offset, rela->r_addend, sym, type);
	if (sym == 0 && rela->r_addend != 0) {
		return lib->base + rela->r_addend;
	}
	char* sym_name = lib->strtab + lib->symtab[sym].st_name;
	//printf("linking symbol name: %s\n", sym_name);

	void* ret = 0;
	if (strncmp(sym_name, EXTERNAL_PREFIX, EXTERNAL_PREFIX_LEN) == 0) {
		void* libc = dlopen2("libc.so.6");
		if (!libc) {
			fprintf(stderr, "could not open libc\n");
			exit(-1);
		}
		void* val = dlsym2(libc, sym_name+ EXTERNAL_PREFIX_LEN);
		if (!val) {
			fprintf(stderr, "could not load external %s\n", sym_name+ EXTERNAL_PREFIX_LEN);
			exit(-1);
		}
		//printf("setting name %s, at %x from base %x to %x", sym_name, rela->r_offset, lib->base, val);
		ret = val;
		dlclose2(libc);
	} else if (strcmp(sym_name, "external_elfator_os") == 0) {
		ret = &get_os;

	} else if (strncmp(sym_name, "external_", 9) == 0) {
		// ignore
	} else {
		printf("version \n");
		char * version = verneedstr(sym, lib);
		size_t lib_len = strchr(version, '_') - version;
		for (size_t i = 0; i < libs->libs_count; i++) {
			loaded_lib* l = &libs->libs[i];
			if (strncmp(version, l->name, lib_len) == 0) {
				Elf64_Half f = lookup(sym_name, version, l);
				void * f_addr = l->base + l->symtab[f].st_value;
				//printf("setting name %s, at %x from base %x to %x\n", sym_name, rela->r_offset, lib->base, f_addr);
				ret = f_addr;
			}
		}
	}
	printf("setting symbol %s to %p\n", sym_name, ret);
	return ret;
}

void link(loaded_lib* lib, loaded_libs* libs) {
	printf("linking library %s\n", lib->path);
	for (int i = 0 ; i < lib->rela_count ; i++) {
		Elf64_Rela * rela = lib->rela + i;
		void** offsetTableLoc = lib->base + rela->r_offset;

		printf("linking symbol\n");
		*offsetTableLoc = findSymbol(rela, lib, libs);
		printf("offset: %lx, info %lx, addend: %lx\n", lib->rela[i].r_offset, lib->rela[i].r_info, lib->rela[i].r_addend);
	}
	for (int i = 0 ; i < lib->rela_plt_count ; i++) {
		printf("linking function\n");
		Elf64_Rela * rela = lib->rela_plt + i;
		void** offsetTableLoc = lib->base + rela->r_offset;

		*offsetTableLoc = findSymbol(rela, lib, libs);
	}



}

void init_libs(loaded_libs* libs) {
	for (int i = 0; i < libs->libs_count; i++) {
		loaded_lib* lib = &libs->libs[i];
		if (lib->init_array) {
			for (int j = 0; j * sizeof(void*) < lib->init_arraysz ; j++) {
				// this is already absolute because of relocation
				size_t rel_f = lib->init_array[j];
				((void (*)())(rel_f))();
			}
		}
	}
}

void fini_libs(loaded_libs* libs) {
	for (int i = 0; i < libs->libs_count; i++) {
		loaded_lib* lib = &libs->libs[i];
		if (lib->fini_array) {
			for (int j = 0; j * sizeof(void*) < lib->fini_arraysz ; j++) {
				// this is already absolute because of relocation
				size_t rel_f = lib->fini_array[j];
				((void (*)())(rel_f))();
			}
		}
	}
}


void main(int argc, char ** argv) {
	if (argc != 2) {
		fprintf(stderr, "./loadelf file");
	}

	realloc(0, 234);
	loaded_libs libs;
	alloc_loaded_libs(&libs);
	loaded_lib* lib = load(argv[1], &libs);
	for (size_t i = 0 ; i < libs.libs_count ; i++) {
		link(&libs.libs[i], &libs);
	}
	Elf64_Half main_idx = lookup("main", 0, lib);
	init_libs(&libs);
	//printf("main idx %d\n", main_idx);

	int (*main_f)(void) __attribute__((sysv_abi));
	void * main_addr = lib->base + lib->symtab[main_idx].st_value;
	main_f = main_addr;
	int ret = main_f();
	fini_libs(&libs);
	printf("main returned %d\n", ret);
}
