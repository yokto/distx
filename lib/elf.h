#define EI_NIDENT 16

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef signed long int64_t;

typedef uint64_t ElfN_Addr;
typedef uint64_t ElfN_Off;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Xword;

// elf header
typedef struct {
	unsigned char e_ident[EI_NIDENT];
	uint16_t      e_type;
	uint16_t      e_machine;
	uint32_t      e_version;
	ElfN_Addr     e_entry;
	ElfN_Off      e_phoff;
	ElfN_Off      e_shoff;
	uint32_t      e_flags;
	uint16_t      e_ehsize;
	uint16_t      e_phentsize;
	uint16_t      e_phnum;
	uint16_t      e_shentsize;
	uint16_t      e_shnum;
	uint16_t      e_shstrndx;
} ElfN_Ehdr;

// program header entry
typedef struct {
	uint32_t   p_type;
	uint32_t   p_flags;
	Elf64_Off  p_offset;
	Elf64_Addr p_vaddr;
	Elf64_Addr p_paddr;
	uint64_t   p_filesz;
	uint64_t   p_memsz;
	uint64_t   p_align;
} Elf64_Phdr;

// probably not needed
//typedef struct {
//	uint32_t   sh_name;
//	uint32_t   sh_type;
//	uint64_t   sh_flags;
//	Elf64_Addr sh_addr;
//	Elf64_Off  sh_offset;
//	uint64_t   sh_size;
//	uint32_t   sh_link;
//	uint32_t   sh_info;
//	uint64_t   sh_addralign;
//	uint64_t   sh_entsize;
//} Elf64_Shdr;

// entry in .dynamic section
typedef struct {
	Elf64_Sxword	d_tag;
   	union {
   		Elf64_Xword	d_val;
   		Elf64_Addr	d_ptr;
	} d_un;
} Elf64_Dyn;

// dynamic tags (.dynamic section)
#define DT_NULL         0               /* Marks end of dynamic section */
#define DT_NEEDED       1               /* Name of needed library */
#define DT_PLTRELSZ     2               /* Size in bytes of PLT relocs */
#define DT_PLTGOT       3               /* Processor defined value */
#define DT_HASH         4               /* Address of symbol hash table */
#define DT_STRTAB       5               /* Address of string table */
#define DT_SYMTAB       6               /* Address of symbol table */
#define DT_RELA         7               /* Address of Rela relocs */
#define DT_RELASZ       8               /* Total size of Rela relocs */
#define DT_RELAENT      9               /* Size of one Rela reloc */
#define DT_STRSZ        10              /* Size of string table */
#define DT_SYMENT       11              /* Size of one symbol table entry */
#define DT_INIT         12              /* Address of init function */
#define DT_FINI         13              /* Address of termination function */
#define DT_SONAME       14              /* Name of shared object */
#define DT_RPATH        15              /* Library search path (deprecated) */
#define DT_SYMBOLIC     16              /* Start symbol search here */
#define DT_REL          17              /* Address of Rel relocs */
#define DT_RELSZ        18              /* Total size of Rel relocs */
#define DT_RELENT       19              /* Size of one Rel reloc */
#define DT_PLTREL       20              /* Type of reloc in PLT */
#define DT_DEBUG        21              /* For debugging; unspecified */
#define DT_TEXTREL      22              /* Reloc might modify .text */
#define DT_JMPREL       23              /* Address of PLT relocs */
#define DT_BIND_NOW     24              /* Process relocations of object */
#define DT_INIT_ARRAY   25              /* Array with addresses of init fct */
#define DT_FINI_ARRAY   26              /* Array with addresses of fini fct */
#define DT_INIT_ARRAYSZ 27              /* Size in bytes of DT_INIT_ARRAY */
#define DT_FINI_ARRAYSZ 28              /* Size in bytes of DT_FINI_ARRAY */
#define DT_RUNPATH      29              /* Library search path */
#define DT_FLAGS        30              /* Flags for the object being loaded */
#define DT_ENCODING     32              /* Start of encoded range */
#define DT_PREINIT_ARRAY 32             /* Array with addresses of preinit fct*/
#define DT_PREINIT_ARRAYSZ 33           /* size in bytes of DT_PREINIT_ARRAY */
#define DT_SYMTAB_SHNDX 34              /* Address of SYMTAB_SHNDX section */
#define DT_NUM          35              /* Number used */
#define DT_LOOS         0x6000000d      /* Start of OS-specific */
#define DT_HIOS         0x6ffff000      /* End of OS-specific */
