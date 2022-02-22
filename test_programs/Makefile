CC ?= gcc
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
	${CC} \
		helloc.c \
		-Wl,--no-undefined \
		-Wl,-soname,helloc:helloc/bin/helloc \
		-Wl,--version-script=helloc.map \
		-Wl,-Bsymbolic \
		-Wl,-Bsymbolic-functions \
		-Wl,--hash-style=sysv \
		-nostdlib \
		-shared \
		-fPIC \
		-o $(HELLOC) \
		-fno-builtin-printf \
		-lfoobar -L$(dir $(LIBFOOBAR)) \
		-lc -L$(dir $(LIBC))

$(LIBFOOBAR): foobar.c Makefile foobar.map
	mkdir -p $(dir $(LIBFOOBAR))
	${CC} -Wl,-soname,foobar:foobar/lib/libfoobar.so -Wl,--version-script=foobar.map -nostdlib -shared -o $(LIBFOOBAR) foobar.c \
		-Wl,--hash-style=sysv

$(LIBC): libc.c Makefile
	mkdir -p $(dir $(LIBC))
	${CC} \
		-Wl,-soname,c:libc/lib/libc.so \
		-Wl,--version-script=libc.map \
		-Wl,-Bsymbolic \
		-Wl,-Bsymbolic-functions \
		-Wl,--hash-style=sysv \
		-nostdlib -shared \
		-fPIC \
		-fno-builtin \
		-o $(LIBC) \
		libc.c \
		-lc -L$(dir $(LIBC)) 

$(HELLORUST): hellorust.rs Makefile
	mkdir -p $(dir $(HELLORUST))
	rustc hellorust.rs --crate-type cdylib -o $(HELLORUST)

$(HELLOCPP): hellocpp.cpp Makefile hellocpp.map
	mkdir -p $(dir $(HELLOCPP))
	g++ \
		hellocpp.cpp \
		-Wl,-soname,hellocpp:hellocpp/bin/hellocpp \
		-Wl,--version-script=hellocpp.map \
		-Wl,-Bsymbolic \
		-Wl,-Bsymbolic-functions \
		-shared \
		-nolibc \
		-fPIC \
		-lc -L$(dir $(LIBC)) \
		-o $(HELLOCPP)
