all: withlibc libhellorust.so libhellocpp.so

#nolibc: foobar.so nolibc.c ../lib/* Makefile
#	gcc -shared -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fvisibility=hidden -fPIC -o nolibc -lfoobar -L. nolibc.c

withlibc: foobar.so withlibc.c Makefile
	gcc -shared -nostdlib -fvisibility=hidden -fPIC -o withlibc -lfoobar -L. withlibc.c 

foobar.so: foobar.c Makefile
	gcc -nostdlib -shared -o libfoobar.so foobar.c

libhellorust.so: hellorust.rs Makefile
	rustc hellorust.rs --crate-type cdylib

libhellocpp.so: hellocpp.cpp Makefile
	g++ -shared -fvisibility=hidden -fPIC -o libhellocpp.so hellocpp.cpp 
