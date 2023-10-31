#ifndef LINK_H
#define LINK_H
#ifdef __cplusplus
extern "C" {
#endif
// A big part of this file was copied from glibc
/* This file defines standard ELF types, structures, and macros.
   Copyright (C) 1995-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <sys/types.h>

#if __SIZEOF_POINTER__ == 32
#define __ELF_NATIVE_CLASS 32
#else
#define __ELF_NATIVE_CLASS 64
#endif

#define ElfW(type)        _ElfW (Elf, __ELF_NATIVE_CLASS, type)
#define _ElfW(e,w,t)        _ElfW_1 (e, w, _##t)
#define _ElfW_1(e,w,t)        e##w##t

#define EI_NIDENT 16

#include <stdint.h>

typedef uint64_t ElfN_Addr;
typedef uint64_t ElfN_Off;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Xword;
typedef uint32_t Elf64_Word;
typedef uint16_t Elf64_Half;

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

// entry in .dynamic section
typedef struct {
	Elf64_Sxword	d_tag;
   	union {
   		Elf64_Xword	d_val;
   		Elf64_Addr	d_ptr;
	} d_un;
} Elf64_Dyn;

// .symtab
typedef struct {
        Elf64_Word      st_name;
        unsigned char   st_info;
        unsigned char   st_other;
        Elf64_Half      st_shndx;
        Elf64_Addr      st_value;
        Elf64_Xword     st_size;
} Elf64_Sym;

typedef struct {
        Elf64_Addr      r_offset;
        Elf64_Xword     r_info;
} Elf64_Rel;

typedef struct {
        Elf64_Addr      r_offset;
        Elf64_Xword     r_info;
        Elf64_Sxword    r_addend;
} Elf64_Rela;

typedef struct {
        Elf64_Half      vd_version;
        Elf64_Half      vd_flags;
        Elf64_Half      vd_ndx;
        Elf64_Half      vd_cnt;
        Elf64_Word      vd_hash;
        Elf64_Word      vd_aux;
        Elf64_Word      vd_next;
} Elf64_Verdef;
 
typedef struct {
        Elf64_Word      vda_name;
        Elf64_Word      vda_next;
} Elf64_Verdaux;

typedef struct {
        Elf64_Half      vn_version;
        Elf64_Half      vn_cnt;
        Elf64_Word      vn_file;
        Elf64_Word      vn_aux;
        Elf64_Word      vn_next;
} Elf64_Verneed;

typedef struct {
        Elf64_Word      vna_hash;
        Elf64_Half      vna_flags;
        Elf64_Half      vna_other;
        Elf64_Word      vna_name;
        Elf64_Word      vna_next;
} Elf64_Vernaux;

#define ELF64_R_SYM(info)             ((info)>>32)
#define ELF64_R_TYPE(info)            ((Elf64_Word)(info))
#define ELF64_R_INFO(sym, type)       (((Elf64_Xword)(sym)<<32)+ \
                                        (Elf64_Xword)(type))

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


/* The versioning entry types.  The next are defined as part of the
   GNU extension.  */
#define DT_VERSYM       0x6ffffff0

#define DT_RELACOUNT    0x6ffffff9
#define DT_RELCOUNT     0x6ffffffa

/* These were chosen by Sun.  */
#define DT_FLAGS_1      0x6ffffffb      /* State flags, see DF_1_* below.  */
#define DT_VERDEF       0x6ffffffc      /* Address of version definition
                                           table */
#define DT_VERDEFNUM    0x6ffffffd      /* Number of version definitions */
#define DT_VERNEED      0x6ffffffe      /* Address of table with needed
                                           versions */
#define DT_VERNEEDNUM   0x6fffffff      /* Number of needed versions */
#define DT_VERSIONTAGIDX(tag)   (DT_VERNEEDNUM - (tag)) /* Reverse order! */


#define PT_NULL         0               /* Program header table entry unused */
#define PT_LOAD         1               /* Loadable program segment */
#define PT_DYNAMIC      2               /* Dynamic linking information */
#define PT_INTERP       3               /* Program interpreter */
#define PT_NOTE         4               /* Auxiliary information */
#define PT_SHLIB        5               /* Reserved */
#define PT_PHDR         6               /* Entry for header table itself */
#define PT_TLS          7               /* Thread-local storage segment */
#define PT_NUM          8               /* Number of defined types */
#define PT_GNU_EH_FRAME 0x6474e550
#define PT_ARM_EXIDX    0x70000001

uint32_t
elf_Hash(const unsigned char *name)
{
    uint32_t h = 0, g;

        while (*name)
        {
             h = (h << 4) + *name++;
             if (g = h & 0xf0000000)
                  h ^= g >> 24;
                   h &= ~g;
        }
        return h;
}


struct dl_phdr_info
  {
    ElfW(Addr) dlpi_addr;
    const char *dlpi_name;
    const ElfW(Phdr) *dlpi_phdr;
    ElfW(Half) dlpi_phnum;

    /* Note: Following members were introduced after the first
       version of this structure was available.  Check the SIZE
       argument passed to the dl_iterate_phdr callback to determine
       whether or not each later member is available.  */

    /* Incremented when a new object may have been added.  */
    __extension__ unsigned long long int dlpi_adds;
    /* Incremented when an object may have been removed.  */
    __extension__ unsigned long long int dlpi_subs;

    /* If there is a PT_TLS segment, its module ID as used in
       TLS relocations, else zero.  */
    size_t dlpi_tls_modid;

    /* The address of the calling thread's instance of this module's
       PT_TLS segment, if it has one and it has been allocated
       in the calling thread, otherwise a null pointer.  */
    void *dlpi_tls_data;
  };


int dl_iterate_phdr (int (*callback) (struct dl_phdr_info *info,
                                    size_t size, void *data), void *data);

//typedef struct
//{
//  const char *dli_fname;        /* File name of defining object.  */
//  void *dli_fbase;              /* Load address of that object.  */  
//  const char *dli_sname;        /* Name of nearest symbol.  */
//  void *dli_saddr;              /* Exact value of nearest symbol.  */  
//} Dl_info;

#ifdef __cplusplus
}
#endif
#endif // LINK_H
