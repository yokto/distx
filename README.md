
# How is a program loaded

## Exit from old program

The old program calles the execve systemcall.
This call does not return.
Instead the specified program is executed in this process (programs usually call clone before execve).

## Kernel

The program specified in the execve systemcall can be either a script with she-bang or an elf executable.
We will always assume it's an elf executable.

The `load_elf_binary` function in the `binfmt_elf.c` in the kernel is responsible for loading the executable into memory.

Simplified `load_elf_binary` function:

	/* Now we do a little grungy work by mmapping the ELF image into
	   the correct location in memory. */
	for(i = 0 i++ elf_ppnt in program_headers) {
		error = elf_map(bprm->file, load_bias + vaddr, elf_ppnt,
				elf_prot, elf_flags, total_size);

	}
	if (interpreter) {
		elf_entry = load_elf_interp(interp_elf_ex,
				interpreter,
				load_bias, interp_elf_phdata,
				&arch_state);
	}

## ELF strucutre

https://stevens.netmeister.org/631/elf.html

This is all for 64 bit.

1. Elf Header 64bytes
2. Program Headers 56bytes * number of headers (this describes how things get loaded into memory / mostly for the dynamic linker)
    - irrelevant for dynamic linker
3. .interp (only if set with -Wl,--dynamic-linker=...) string pointing to dynamic linker
    - we don't use this
4. .hash (only if dynamic) hash table lookup for string -> entry in .symtab
    - we find this from .dynamic -> DT_HASH
5. .gnu.hash (only if dynamic) faster version of .hash
6. .dynsym (only if dynamic) list of symbols from other libraries
7. .dynstr (only if dynamic) strings for .dynsym
8. .rela.dyn (only if dynamic) relocation non ptl (i.e. variables). sais which part of the code this needs to be copied to
9. .rela.plt (only if dynamic) relocation for ptl (procedur linkage talbe. i.e. functions). sais which part of the code this needs to be copied to
10. Padding to Pagesize 0x1000 (4096)
11. .plt (procedure linkage table) when .text wants to call an external function it calls here. then from here it does jmpq to Global Offset Table. Then first time into dl_runtime_resolve_xsavec. Second time directly
12. .text (program code)
13. Padding to Pagesize
14. .rodata (read only data)
....
15. Padding to Pagesize 0x1000 - sizeof(.dynamic)
16. .dynamic (entries in .strtab)
17. .got.plt (starts at pagesize)
18. Section Headers 64bytes * number of headers (this actually describes all the .something sections / mostly for the static linker)

In the programs address space there folloing parts are usually mapped. Note executables compiled with -pie -fPIE are more like dynamic libraries and have ASLR (Address Space Layout Randomization).
1 - 9 -> 0x400000 (readonly) 
11 - 12 -> 0x401000 (read execute)
14 - .. -> 0x402000
Mappings of a running program can be checked at /proc/<pid>/maps

## Stack Layout

http://articles.manugarg.com/aboutelfauxiliaryvectors.html

## PLT/GOT (Procedure Linkage Table)

Assuming your code looks like this. `foo` and `bar` are external functions

    foo();
    bar();
    foo();

The assembly code will look like this

    call 0x11130 <foo.plt>
    call 0x11140 <bar.plt>
    call 0x11130 <foo.plt>

The plt will look

    0x11120    push 0x2fe2(%rip) # plt base
    0x11126    jmp *0x2fe4(%rip) # points to _dl_runtime_resolve_xsavec
    0x1112c    nopl 0x0(%rax)
    0x11130    jmp 0x22220 # this is foo@plt and points to foo@got.plt
    0x11136    push 0x0
    0x1113b    jmp 0x11120
    0x11140    jmp 0x22228 # this is bar@plt and points to bar@got.plt
    0x11146    push 0x1 # plt entry
    0x1114b    jmp 0x11120

The got will look like

    0x22220    0x11136 # this is foo@got.plt (this will be replaced to point to the actual foo)
    0x22228    0x11146 # this is bar@got.plt
