all: nolibc withlibc

nolibc: foobar.so nolibc.c ../lib/* Makefile
	gcc -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fPIE -pie -o nolibc -lfoobar -L. nolibc.c

withlibc: foobar.so withlibc.c Makefile
	gcc -fPIE -pie -o withlibc -lfoobar -L. withlibc.c

foobar.so: libfoobar.c Makefile
	gcc -nostdlib -shared -o libfoobar.so libfoobar.c

