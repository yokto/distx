all: nolibc withlibc

#nolibc: foobar.so nolibc.c ../lib/* Makefile
#	gcc -shared -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fvisibility=hidden -fPIC -o nolibc -lfoobar -L. nolibc.c

withlibc: foobar.so withlibc.c Makefile
	gcc -shared -nostdlib -fvisibility=hidden -fPIC -o withlibc -lfoobar -L. withlibc.c 

foobar.so: foobar.c Makefile
	gcc -nostdlib -shared -o libfoobar.so foobar.c

