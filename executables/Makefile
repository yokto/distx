all: load_elf load_elf.exe

load_elf: load_elf.c
	mkdir -p ../build
	gcc -ldl -fPIC -fpie -o ../build/load_elf load_elf.c

load_elf.exe: load_elf.c
	mkdir -p ../build
	x86_64-w64-mingw32-gcc load_elf.c -o ../build/load_elf.exe
