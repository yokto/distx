nolibc: nolibc.c ../lib/* Makefile
	gcc -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fPIE -pie -o nolibc -l_nolibc -L. nolibc.c
	#gcc -nostdlib -fPIE -pie -o nolibc -l_nolibc -L. nolibc.c
