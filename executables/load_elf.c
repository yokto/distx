#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdarg.h>

#define WARN(...) { \
	fprintf(stderr, __VA_ARGS__); \
}

#define ERR(...) { \
	fprintf(stderr, __VA_ARGS__); \
	exit(-1); \
}

//#define STRICT 1
#ifdef STRICT
#define STRICT_ERR(...) { \
	fprintf(stderr, __VA_ARGS__); \
	exit(-1); \
}
#else
#define STRICT_ERR(...) { \
	fprintf(stderr, __VA_ARGS__); \
}
#endif

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

#ifndef WIN32
#include "../lib/jit.h"
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
	bool init_started;
} loaded_lib;

typedef struct loaded_libs {
	loaded_lib * libs;
	int libs_count;
	int vec_length;
} loaded_libs;

#ifndef WIN32
struct link_map* global_libs = 0;
//void m_add(loaded_libs* libs, struct DGBLIST* new) {
//	if (!libs->m_head) {
//		libs->m_head = new;
//		new->l_next = 0;
//		new->l_prev = 0;
//	} else {
//		new->l_next = libs->m_head;
//		new->l_prev = 0;
//		libs->m_head->l_prev = new;
//		libs->m_head = new;
//	}
//}
#endif

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

#ifndef WIN32
void add_local_libs() {
	_r_debug.r_state = RT_ADD;
	printf("brk %p", _r_debug);
	fflush(stdout);
	((void(*)())(_r_debug.r_brk))();
	struct link_map* current_map = _r_debug.r_map;
	if (current_map == 0) {
		_r_debug.r_map = global_libs;
	} else {
		while (current_map->l_next != 0) {
			current_map = current_map->l_next;
		}
		current_map->l_next = global_libs;
		global_libs->l_prev = current_map;
	}
	_r_debug.r_state = RT_CONSISTENT;
	((void(*)())(_r_debug.r_brk))();


	
	printf("\n\n\n readding\n", _r_debug.r_brk);
	struct link_map* m = _r_debug.r_map;
	while (m) {
		printf("map %s\n", m->l_name);
		printf("addr %p\n", m->l_addr);
		printf("dynamic %p\n", m->l_ld);
		m = m->l_next;
	}
}
void remove_local_libs() {
	_r_debug.r_state = RT_DELETE;
	((void(*)())(_r_debug.r_brk))();
	struct link_map* current_map = _r_debug.r_map;
	if (current_map == global_libs) {
		_r_debug.r_map = 0;
	} else {
		while (current_map->l_next != global_libs) {
			current_map = current_map->l_next;
		}
		current_map->l_next = 0;
	}
	global_libs->l_prev = 0;
	_r_debug.r_state = RT_CONSISTENT;
	((void(*)())(_r_debug.r_brk))();
	


	printf("\n\n\n removing\n", _r_debug.r_brk);
	struct link_map* m = _r_debug.r_map;
	while (m) {
		printf("map %s\n", m->l_name);
		printf("addr %p\n", m->l_addr);
		printf("dynamic %p\n", m->l_ld);
		m = m->l_next;
	}
}
#endif


loaded_lib* load(char* lib_path, loaded_libs* libs) {
	for (int i = 0 ; i < libs->libs_count ; i++) {
		if (libs->libs[i].path && strcmp(libs->libs[i].path, lib_path) == 0) {
			printf("already loaded library %s\n", lib_path);
			return &libs->libs[i];
		}
	}
	loaded_lib* lib = add_loaded_lib(libs);
	memset(lib, 0, sizeof(loaded_lib));
	printf("\tloading library %s\n", lib_path);

	// needs to happen before recursive call to load
	lib->path = malloc(strlen(lib_path) + 1);
	strcpy(lib->path, lib_path);

	FILE* fd = fopen(lib_path, "rb");
	if (fd == 0) {
		ERR("can't open %s", lib_path);
	}
	ElfN_Ehdr elf_header;
	if (fread(&elf_header, sizeof(ElfN_Ehdr), 1, fd) != 1) {
		ERR("can't read elfheader %s", lib_path);
	}
	if (elf_header.e_phnum > 32) {
		ERR("can't read more than 32 program headers in %s", lib_path);
	}
	Elf64_Phdr phs[32];
	int phnum = elf_header.e_phnum;
	if (fread(phs, sizeof(Elf64_Phdr), phnum, fd) != phnum) {
		ERR("can't read program headers %s", lib_path);
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
				ERR("addresses don't match %s", lib_path);
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
				ERR("can't seek in %s\n", lib_path);
			}
			if (fread((void*) memory + ph->p_vaddr, ph->p_filesz, 1, fd) != 1) {
				ERR("can't read section %s\n", lib_path);
			}
#else
			mmap(
				(void*)maddr,
				msize,
				PROT_READ | PROT_WRITE | PROT_EXEC, // todo remove wrong ones
				MAP_PRIVATE | MAP_FIXED,
				fileno(fd),
				file_offset);
#endif
			// if p_filesz smaller than p_memsz we need to set the rest to zero. I.e. bss. section
			if (ph->p_filesz < ph->p_memsz) {
				printf("setting %p for %x to zero\n", ph->p_vaddr + ph->p_filesz, ph->p_memsz - ph->p_filesz);
				memset(memory + ph->p_vaddr + ph->p_filesz, 0, ph->p_memsz - ph->p_filesz);
			}
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
					char * last_slash = strrchr(soname, '/');
					char * lib_name = last_slash && strncmp(last_slash, "/lib", 4) == 0 ? last_slash + 4 : 0; // drop the "lib" in "libxyz.so"
					char * dot = lib_name ? strchr(lib_name, '.') : 0;
					size_t lib_name_len = dot ? dot - lib_name : 0;
					lib->name = malloc(lib_name_len + 1);
					strncpy(lib->name, lib_name, lib_name_len);
					lib->name[lib_name_len] = '\0';
					printf("libname %s\n", lib->name);
					if (lib_name_len <= 0){ 
						WARN("Could not figure out libname of %s", soname);
					}
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
						ERR("can only read DT_RELA at the moment\n");
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
					loaded_lib * lib = load(needed, libs);
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

#ifndef WIN32
	struct link_map* new_map = malloc(sizeof(struct link_map));
	memset(new_map, 0, sizeof(struct link_map));
	const char * pwd = "/home/silvio/stuff/sources/elfx86/test_programs/";
	const int pwd_len = strlen(pwd);
	const int lib_len = strlen(lib_path);
	char * full_path = malloc(pwd_len + lib_len + 1);
	memcpy(full_path, pwd, pwd_len);
	memcpy(full_path + pwd_len, lib_path, lib_len);
	full_path[lib_len + pwd_len] = '\0';
	new_map->l_name = full_path;
	new_map->l_addr = lib->base;
	//new_map->l_ld = lib->dynamic;
	new_map->l_ld = (void*)lib->dynamic;
	//m_add(libs, new_map);

	new_map->l_next = 0; // not strictly needed
	struct link_map* current_map = global_libs;
	if (current_map == 0) {
		global_libs = new_map;
	} else {
		while (current_map->l_next != 0) {
			current_map = current_map->l_next;
		}
		current_map->l_next = new_map;
		new_map->l_prev = current_map;
	}
#endif

	return lib;
}

// find the version string of symbol
// this are some truely ugly structures
char * verdefstr(size_t sym_idx, loaded_lib* lib) {
	return lib->path;
//	Elf64_Verdef * v = lib->verdef;
//	for(int i = 1 ; i < lib->versym[sym_idx] ; i++) {
//		v = (void*)v + v->vd_next;
//	}
//	Elf64_Verdaux * aux = (Elf64_Verdaux*)((void*)v + v->vd_aux);
//
//	char * ver = &lib->strtab[aux->vda_name];
//	//printf("version name %s\n", v);
//	return ver;
}

// find the version string of symbol
// this are some truely ugly structures
char * verneedstr(size_t sym_idx, loaded_lib* lib) {
	Elf64_Verneed * v = lib->verneed;
	if (!lib->verneed) {
		STRICT_ERR("could not find version for symbol %s in library %s\n",
			lib->strtab + lib->symtab[sym_idx].st_name, lib->name);
		return 0;
	}
	printf("\t\t\tneeded index %zd residex %d \n", sym_idx, lib->versym[sym_idx]);

	size_t version = lib->versym[sym_idx];
	while (true) {
		Elf64_Vernaux * aux = (Elf64_Vernaux*)((void*)v + v->vn_aux);
		while (true) {
			if (aux->vna_other == version) {
				return &lib->strtab[v->vn_file];
			}
			if (aux->vna_next == 0) { break; }
			aux = (void*)aux + aux->vna_next;
		}

		if (v->vn_next == 0) { break; }
		v = (void*)v + v->vn_next;
	}
	STRICT_ERR("could not find version for symbol %d in library %s\n", lib->strtab + lib->symtab[sym_idx].st_name, lib->name);
	return 0;
}

Elf64_Half lookup(char* name, loaded_lib* lib) {
	if (!lib->hash) {
		ERR("no hash table in %s\n", lib->name);
	}
	const uint32_t nbucket = ((uint32_t*)lib->hash)[0];
	const uint32_t nchain = ((uint32_t*)lib->hash)[1];
	if (nbucket == 0) { return 0; }
	const unsigned long hash = elf_Hash(name);

	Elf64_Half idx = lib->hash[2 + (hash % nbucket)];
	printf("\t\t\tlooking for %s in library %s\n",name, lib->name);
	while (idx != 0) {
		printf("\t\t\tname %s, foundname %s\n", name, lib->strtab + lib->symtab[idx].st_name);
		if (strcmp(name, lib->strtab + lib->symtab[idx].st_name) == 0) {
			//char* version2 = verdefstr(idx, lib);
			//printf("wanted version %s, existing version %s\n", version, version2);
			//if (!version || strcmp(version2, version) == 0) {
				//printf("found version %s\n", version, version2);
				return idx;
			//}
		}
		printf("index %d\n", idx);
		idx = lib->hash[2 + nbucket + idx];
		printf("index %d\n", idx);
	}
	ERR("name not found %s", name);
}

__attribute__((sysv_abi))
void exit2(int ret) {
	exit(ret);
}

__attribute__((sysv_abi))
int printf2(const char *restrict format, ...) {
	va_list arglist;
	va_start(arglist, format);
	vprintf(format, arglist);
	va_end(arglist);
}

__attribute__((sysv_abi))
void* dlopen2(char* name) {
	printf("dlopening %s\n", name);
#if defined(WIN32)
	printf("\t\tgetting lib %s\n", name);
	return LoadLibrary("msvcrt.dll");
#else
	remove_local_libs();
	void* res = dlopen(name, RTLD_NOW);
	add_local_libs();
	return res;
#endif
}

__attribute__((sysv_abi))
void* dlsym2(void* lib, char* name) {
	printf("dlsyming %s\n", name);
#if defined(WIN32)
	printf("\t\tgetting sym %s from lib %p\n", name, lib);
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

	printf("rela info %lx, offset %lx, addend %lx, sym %lx, type %lx\n", rela->r_info, rela->r_offset, rela->r_addend, sym, type);
	if (type == R_X86_64_RELATIVE && sym == 0 && rela->r_addend != 0) {
		return lib->base + rela->r_addend;
	}
	else if (type == R_X86_64_32 || type == R_X86_64_JUMP_SLOT || type == R_X86_64_GLOB_DAT || type == R_X86_64_64) {
		char* sym_name = lib->strtab + lib->symtab[sym].st_name;
		printf("\t\tlinking symbol: %s\n", sym_name);

		void* ret = 0;
		if (strncmp(sym_name, EXTERNAL_PREFIX, EXTERNAL_PREFIX_LEN) == 0) {
			void* libc = dlopen2("libc.so.6");
			if (!libc) {
				// on android
				libc = dlopen2("libc.so");
			}
			if (!libc) {
				ERR("could not open libc\n");
			}
			void* val = dlsym2(libc, sym_name+ EXTERNAL_PREFIX_LEN);
			if (!val) {
				ERR("could not load external %s\n", sym_name+ EXTERNAL_PREFIX_LEN);
			}
			//printf("setting name %s, at %x from base %x to %x", sym_name, rela->r_offset, lib->base, val);
			ret = val;
			dlclose2(libc);
		} else if (strcmp(sym_name, "__printf") == 0) {
			ret = &printf2;
		} else if (strcmp(sym_name, "__dlsym") == 0) {
			ret = &dlsym2;
		} else if (strcmp(sym_name, "__dlopen") == 0) {
			ret = &dlopen2;
		} else if (strcmp(sym_name, "__exit") == 0) {
			ret = &exit2;
		} else if (strncmp(sym_name, "external_", 9) == 0) {
			// ignore
		} else {
			char * file = verneedstr(sym, lib);
			if (!file) {
				file = verdefstr(sym, lib);
			}
			if (!file) {
				STRICT_ERR("could not link symbol\n", sym_name);
				return 0;
			} else {
				for (size_t i = 0; i < libs->libs_count; i++) {
					loaded_lib* l = &libs->libs[i];
					if (strcmp(file, l->path) == 0) {
						Elf64_Half f = lookup(sym_name, l);
						void * f_addr = l->base + l->symtab[f].st_value;
						//printf("setting name %s, at %x from base %x to %x\n", sym_name, rela->r_offset, lib->base, f_addr);
						printf("\t\t\tfound symbol in %s\n", l->name);
						ret = f_addr;
					}
				}
			}
		}
		printf("\t\t\tsetting symbol %s to %p with offset\n", sym_name, ret, rela->r_addend);
		return ret + rela->r_addend;
	} else {
		ERR("unknown relocation %d", type);
	}
}

void setSymbol(Elf64_Rela * rela, void* sym, loaded_lib* lib) {
	int type = ELF64_R_TYPE(rela->r_info);
	if (type == R_X86_64_32) {
		uint32_t* offsetTableLoc = (uint32_t*)(lib->base + rela->r_offset);
		*offsetTableLoc = sym;
	} else {
		uint64_t* offsetTableLoc = (uint64_t*)(lib->base + rela->r_offset);
		*offsetTableLoc = sym;
	}
}

void link(loaded_lib* lib, loaded_libs* libs) {
	printf("\tlinking library %s\n", lib->path);
	printf("\trela count %d\n", lib->rela_count);
	printf("\trela plt count %d\n", lib->rela_plt_count);
	for (int i = 0 ; i < lib->rela_count ; i++) {
		Elf64_Rela * rela = lib->rela + i;
		void * sym = findSymbol(rela, lib, libs);
		setSymbol(rela, sym, lib);

		// printf("\t\toffset: %lx, info %lx, addend: %lx\n", lib->rela[i].r_offset, lib->rela[i].r_info, lib->rela[i].r_addend);
	}
	for (int i = 0 ; i < lib->rela_plt_count ; i++) {
		Elf64_Rela * rela = lib->rela_plt + i;
		void * sym = findSymbol(rela, lib, libs);
		setSymbol(rela, sym, lib);
	}



}

void init_lib(loaded_lib* lib, loaded_libs* libs) {
	if (lib->init_started) { return; }
	lib->init_started = true;

	// init dependencies
	Elf64_Dyn* dynamic;
	for (dynamic = lib->dynamic; dynamic->d_tag != DT_NULL; dynamic++) {
		if (dynamic->d_tag == DT_NEEDED) {
			char* needed = lib->strtab + dynamic->d_un.d_ptr;
			for (int j = 0; j < libs->libs_count; j++) {
				loaded_lib* lib2 = &libs->libs[j];
				if (strcmp(lib2->path,  needed)) {
					init_lib(lib2, libs);
				}
			}
		}
	}

	// init self
	printf("\t\tinit %s\n", lib->name);
	if (lib->init_array) {
		for (int j = 0; j * sizeof(void*) < lib->init_arraysz ; j++) {
			// this is already absolute because of relocation
			size_t rel_f = lib->init_array[j];
			printf("\t\tinit %d from library %s with pointer %p\n", j, lib->name, (void*)rel_f - lib->base);
			//printf("exec init at %llx for lib %s\n", rel_f, lib->name);
			((void (*)())(rel_f))();
		}
	}
}

void init_libs(loaded_libs* libs) {
	for (int i = 0; i < libs->libs_count; i++) {
		loaded_lib* lib = &libs->libs[i];
		printf("\tstart init %s\n", lib->name);
		init_lib(lib, libs);
		printf("\tend init %s\n", lib->name);
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

int main(int argc, char ** argv) {
//#if defined(WIN32)
//	SetConsoleOutputCP(65001);
//#endif
	//printf("printf %s\n", &printf);
	printf("stdin %p, stdout %p, stderr %p\n", stdin, stdout, stderr);
	if (argc != 2) {
		ERR("./loadelf file\n");
	}

	loaded_libs libs;
	alloc_loaded_libs(&libs);

	printf("loading\n");
	loaded_lib* lib = load(argv[1], &libs);

	printf("linking\n");
	for (size_t i = 0 ; i < libs.libs_count ; i++) {
		link(&libs.libs[i], &libs);
	}
#ifndef WIN32
	printf("f %p\n", _r_debug.r_map);
	add_local_libs();
	printf("f %p\n", _r_debug.r_brk);
	printf("f %p\n", &_dl_debug_state);
#endif
	printf("start init libs\n");
	init_libs(&libs);
	printf("end init libs\n");

	Elf64_Half main_idx = lookup("main", lib);
	int (*main_f)(void) __attribute__((sysv_abi));
	void * main_addr = lib->base + lib->symtab[main_idx].st_value;
	main_f = main_addr;
	printf("main at %d %p\n", main_idx , main_addr - lib->base);
	int ret = main_f();
	printf("main returned %d\n", ret);
	printf("fini\n");
	fini_libs(&libs);
	printf("don\n");
}
