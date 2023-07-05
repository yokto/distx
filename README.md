# Elvator

## Motivation

The idea of this project is to have a loader that can run the same binaries on all operating systems.
Developers developing in compiled languages have a hard life.
They need to build their project for multiple cpu architectures (x86, x64, arm32, arm64, riscv...) and multiple operating systems.
On Linux they might even have to do it for every distribution.
Even worse, cross compilation is usually very cumbersome, so they actually need a machine for every configuration in their build infrastructure.

There are good reasons (performance/address sizes) to compile binaries for different architectures.
However what are the reasons for having to compile differently for different operating systems.
In our opinion there are none.

## Quickstart?

The loader will be given the executable.
The executable is in the format of a shared library with a main function.
The loader loads the relevant parts of the executable into memory.
Since the loader is already running with a setup stack/... and is in fact running the loaders main function it is in the right state to also run the executables main function.
So there is very little systemspecific setup needed.


In the current setup you can compile on linux with the following flag.
    -shared -nostdlib -fvisibility=hidden -fPIC

## Design considerations
    
To understand this section it is good if you have a good understanding of how programs are loaded. There is a short overview in the "Loading on Linux" section

### Where do we make the cut

A program will always need some functionality from the operating system.
The question is where we want to set this interface.

#### systemcall interface
We could set it at the systemcall interface.
This is what docker/WSL (windows subsystem linux) does.

Advantages:
- What happens in the address space is very well controlled and thus probably very similar wherever it's run.
- Can run a lot of existing applications
Disadvantages:
- Obviously tied to the systemcall interface of one operating system. 
- If run on a differnt system a lot of out of process virtualization is needed (linux kernel/xserver in case of WSL)

Docker is the less interesting case it only runs natively under linux.
Ignoring security concerns, it is actually not so different from just running a statically linked binary.
In Docker it seems uncommon to run GUI applications.
It seems most suited for applications that don't rely on too much periphery.

WSL is more intersting. It actually has a linux kernel running beside the windows kernel so the executable can actually run normal linux binaries.
This is only a general solution if all operating system providers implement this same functionality.
There is probably no way for an independent party to implement such a thing for OSX.
Because of this same reason, it is also very difficult to add to or change this interface.
Any new API would first need to be supported by all providers.
Also it is unclear how userspace drivers should work.
To avoid unnecessary overhead a program should ideally call directly to the userspace drivers provided by the system.
The system probably needs to provide linux userspace drivers that talk to virtual kernel drivers that then talk to the real kernel drivers in WSL.
I assume this is how WSL manages to support OpenGL, but I'm not sure???.
WSL seems to run it's own version of an XServer. Ideally this overhead could also be removed.

#### Fix unchanged binary in userspace

This is what wine does. The userspace will load the code of the windows binary.
For all windows system functions wine will link in implementions that use linux systemcalls.

Advantages:
- This seems to work verry well for a lot of games with very little overhead.
Disadvantages:
- Can probably never run anything without proprietary code.

There is a part of code between the libc api and the api that wine virtualizes.
This part probably consists of proprietary code linked into the executable itself and proprietary code in dlls provided by wine.

#### Create Binary in minimal form

What should the loader do and what should the executable do?

We want to decide for the following things.
- threading
  - Implementation on Linux relies on `fork()`. So we could instead have the interface at `fork()`
- initializing libraries
- memory allocation
  - This would work very well as either library or just provided by the system
- linking symbols
  - We can not put this into a dynamically linked library because it needs to run before dynamic linking
  - We don't want to put it in all executables because it would make executables much to big and be very unmodular

### Other solutions
    

How does this compare to wine?

## Loading on Linux

### Overview

Here we describe the different components involved in starting a new program.

We are mostly interested in dynamically linked programs. So this is always assumed.

1. Exit from old program (old executable). Old program calls the systemcall execve which takes the new executable as argument.
2. Kernel. The kernel loads the code of the executable and reads path to the interpreter (dynamic loader) from it. It also loads the code of the dynamic loader. It also allocates a stack and puts some arguments on it. Then it passes control to the Dynamic Loader (userspace).
3. Dynamic Loader (dl in userspace). The dynamic loads the depencencies (libraries) of the executable and links it. Then it gives control to the _start function.
4. _start (executable). The start function is actually statically linked inside the executable from crt1.o. Not sure what exactly it does. But it calls __libc_start_main in glibc.
5. __libc_start_main (glibc). Not sure what it does. But it does eventually call main of the executable.
6. main (executable)


### Exit from old program

The old program calles the execve systemcall.
This call does not return.
Instead the specified program is executed in this process (programs usually call clone before execve).

### Kernel

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

## CSU (C Start Up)

Defined in glibc/csu.
This contains some important files.
	crt1.o
	crti.o
	crtn.o

### crt1.o (c run time)

This gets added to to every executable at the beginning.
It contains the `_start` symbol.
It does not get added to `-shared` objects.
It fiddles around with the stack a bit, sets up env, auxv, runs `__libc_start_main`.
This in turn fiddles some more, calls the initializers of all the shared libraries and executes main.

In our loader these tasks are done by the loader so we (hopefully) don't need this.

### crti.o and crtn.o

They seem to do some magic with the .init .fini section.
They are only a couple of lines of assembly.
E.g in `sysdeps/x86_64/crti.S`
I think `crti` can setup `__gmon_start__` which is some kind of monitoring/progiling tool if it's present.

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

## Windows

Trying to run ELF executables on Windows we run into the following problem.
Our executables want most of their segments to be loaded contiguously.
Their alignment is the pagesize.
However under windows mapping files into memory is only possible with allignment dwAllocationGranularity.
For now we solve this by just copying the data.
This has two backdraws.

1. Multiple copies of the same library in memory.
2. The whole memory segment has to be rwx. This could be a security problem.

## Libc

After fighting with c++ on gcc a bit, it seems like it might easier to port glibc to windows and just emulate the systemcalls than to port stdlibc++ to a libc that just uses the corresponding libc calls of the os. This is because glibc has many calls that are not really standard.

## GDB

It seems gdb gets info about newly loaded librarys when the loader calls `_dl_debug_state`

### breakpoints

In gdb `target_insert_breakpoint`.
There are different breakpoints.
The only one I observed was a memory breakpoint in `default_memory_insert_break`.
GDB then writes an interupt instruction `0xCC` (x86) into the program where it wants the breakpoint.
You can't observe this in gdb itself since it changes it back at any interrupt and inserts it again when you continue running the program.

The following program will print different results depending on weather there is a breakpoint on main or not.

	#include <stdio.h>
	
	int main() {
	        printf("instruction %02hhX\n", *(char*)(&main + 4));
	        return 0;
	}

http://sourceware.org/gdb/onlinedocs/gdb/JIT-Interface.html

separate debug from exec data as follows

    objcopy --strip-debug a.out a.exe
    objcopy --only-keep-debug a.out a.dbg
    objcopy --add-gnu-debuglink="a.dbg" "a.exe"

# MISC

### Program Header

FileSiz can be shorter than MemSiz. This happens for instance for the .bss section of uninitialized variables. The loader needs to set them to 0.

# REFERENCES

### Thread local Storage

https://chao-tic.github.io/blog/2018/12/25/tls


