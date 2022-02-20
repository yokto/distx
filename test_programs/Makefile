BUILDDIR := ../build
LIBFOOBAR := $(BUILDDIR)/foobar/lib/libfoobar.so
HELLOC := $(BUILDDIR)/helloc/bin/helloc
LIBC := $(BUILDDIR)/libc/lib/libc.so
HELLORUST := $(BUILDDIR)/hellorust/bin/hellorust
HELLOCPP := $(BUILDDIR)/hellocpp/bin/hellocpp

all: $(HELLOC) $(HELLORUST) $(HELLOCPP)

#nolibc: foobar.so nolibc.c ../lib/* Makefile
#	gcc -shared -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fvisibility=hidden -fPIC -o nolibc -lfoobar -L. nolibc.c

$(HELLOC): $(LIBFOOBAR) $(LIBC) helloc.c Makefile helloc.map foobar.h
	mkdir -p $(dir $(HELLOC))
	gcc \
		-Wl,--no-undefined \
		-Wl,-soname,helloc:helloc/bin/helloc \
		-Wl,--version-script=helloc.map \
		-shared -nostdlib -fvisibility=hidden \
		-fPIC \
		-o $(HELLOC) \
		-fno-builtin-printf \
		-lfoobar -L$(dir $(LIBFOOBAR)) \
		-lc -L$(dir $(LIBC)) \
		helloc.c 

$(LIBFOOBAR): foobar.c Makefile foobar.map
	mkdir -p $(dir $(LIBFOOBAR))
	gcc -Wl,-soname,foobar:foobar/lib/libfoobar.so -Wl,--version-script=foobar.map -nostdlib -shared -o $(LIBFOOBAR) foobar.c

$(LIBC): libc.c Makefile
	mkdir -p $(dir $(LIBC))
	gcc \
		-Wl,-soname,c:libc/lib/libc.so \
		-Wl,--version-script=libc.map \
		-nostdlib -shared \
		-Wl,-Bsymbolic-functions \
		-o $(LIBC) \
		-lc -L$(dir $(LIBC)) \
		libc.c

$(HELLORUST): hellorust.rs Makefile
	mkdir -p $(dir $(HELLORUST))
	rustc hellorust.rs --crate-type cdylib -o $(HELLORUST)

$(HELLOCPP): hellocpp.cpp Makefile hellocpp.map
	mkdir -p $(dir $(HELLOCPP))
	g++ -Wl,-soname,hellocpp:hellocpp/bin/hellocpp -shared -Wl,--version-script=hellocpp.map -nolibc -fPIC -lc -L -o $(HELLOCPP) hellocpp.cpp 
