CC = gcc

all: load_elf load_elf_android load_elf.exe

load_elf: load_elf.c
	mkdir -p ../build
	${CC} -ldl -fPIC -fpie -o ../build/load_elf load_elf.c

load_elf.exe: load_elf.c
	mkdir -p ../build
	x86_64-w64-mingw32-gcc load_elf.c -o ../build/load_elf.exe

load_elf_android: load_elf.c
	mkdir -p ../build
	LD_LIBRARY_PATH=/run/current-system/sw/lib/ /home/silvio/Android/Sdk/ndk/23.1.7779620/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android24-clang -o ../build/load_elf_android load_elf.c
