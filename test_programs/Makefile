BUILDDIR := build
LIBFOOBAR := $(BUILDDIR)/foobar/lib/libfoobar.so
HELLOC := $(BUILDDIR)/helloc/bin/helloc

all: $(HELLOC) libhellorust.so libhellocpp.so

#nolibc: foobar.so nolibc.c ../lib/* Makefile
#	gcc -shared -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fvisibility=hidden -fPIC -o nolibc -lfoobar -L. nolibc.c

$(HELLOC): $(LIBFOOBAR) helloc.c Makefile
	mkdir -p $(dir $(HELLOC))
	gcc -Wl,-soname,helloc:helloc/lib/helloc -shared -nostdlib -fvisibility=hidden -fPIC -o $(HELLOC) -lfoobar -L$(dir $(LIBFOOBAR)) helloc.c 

$(LIBFOOBAR): foobar.c Makefile
	mkdir -p $(dir $(LIBFOOBAR))
	gcc -Wl,-soname,foobar:foobar/lib/libfoobar.so -nostdlib -shared -o $(LIBFOOBAR) foobar.c

libhellorust.so: hellorust.rs Makefile
	rustc hellorust.rs --crate-type cdylib

libhellocpp.so: hellocpp.cpp Makefile
	g++ -shared -fvisibility=hidden -fPIC -o libhellocpp.so hellocpp.cpp 
