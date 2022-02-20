all: load_elf load_elf.exe

load_elf: load_elf.c
	gcc -ldl -fPIC -fpie -o load_elf load_elf.c
	cp load_elf ../test_programs/build

load_elf.exe: load_elf.c
	x86_64-w64-mingw32-gcc load_elf.c -o load_elf.exe
	cp load_elf.exe ../test_programs/build
