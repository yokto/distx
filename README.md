
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

This is all for 64 bit.

1. Elf Header 64bytes
2. Program Headers 56bytes * number of headers
3. Padding to Pagesize 0x1000 (4096)
4. .text (program code)
5. Padding to Pagesize
6. .rodata (read only data)

In the programs address space there folloing parts are usually mapped
1+2 -> 0x400000
4 -> 0x401000
6 -> 0x402000
Mappings of a running program can be checked at /proc/<pid>/maps
