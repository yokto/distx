CC ?= gcc
BUILDDIR := $(realpath $(dir ${PWD}))
DEPDIR := $(realpath $(dir ${PWD})/..)
LIBFOOBAR := $(BUILDDIR)/lib/libfoobar.so
HELLOC := $(BUILDDIR)/bin/helloc
LIBC := $(DEPDIR)/xlibc/lib/libc.so
#GLIBC := $(BUILDDIR)/glibc/lib/libc.so
#LIBBASE := $(BUILDDIR)/libbase/lib/libbase.so
HELLORUST := $(BUILDDIR)/hellorust/bin/hellorust
HELLOCPP := $(BUILDDIR)/hellocpp/bin/hellocpp
THREAD := $(BUILDDIR)/bin/thread
#HELLOGLIBC := $(BUILDDIR)/helloglibc/bin/helloglibc
CONSTANTS := $(BUILDDIR)constants

#.PHONY: libbase

all: $(HELLOC) $(HELLORUST) $(HELLOCPP) $(LIBBASE) $(CONSTANTS) $(THREAD) # $(HELLOGLIBC) #libbase

#nolibc: foobar.so nolibc.c ../lib/* Makefile
#	gcc -shared -nostdlib '-Wl,--dynamic-linker=/home/silvio/stuff/sources/elf/executables/load_elf.so' -fvisibility=hidden -fPIC -o nolibc -lfoobar -L. nolibc.c

# -fno-stack-protection is for aarch64
$(HELLOC): $(LIBFOOBAR) $(LIBC) helloc.c Makefile helloc.map foobar.h
	mkdir -p $(dir $(HELLOC))
	${CC} \
		-g \
		-I $(DEPDIR)/xlibc/include \
		helloc.c \
		-Wl,--no-undefined \
		-Wl,-soname,test_programs/bin/libhelloc.so \
		-Wl,--version-script=helloc.map \
		-Wl,-Bsymbolic \
		-Wl,-Bsymbolic-functions \
		-Wl,--hash-style=sysv \
		-fno-stack-protector \
		-nostdlib \
		-shared \
		-fno-builtin \
		-fPIC \
		-o $(HELLOC) \
		-lfoobar -L$(dir $(LIBFOOBAR)) \
		-lc -L$(dir $(LIBC))

$(LIBFOOBAR): foobar.c Makefile foobar.map
	mkdir -p $(dir $(LIBFOOBAR))
	${CC} -Wl,-soname,test_programs/lib/libfoobar.so -Wl,--version-script=foobar.map -nostdlib -shared -o $(LIBFOOBAR) foobar.c \
		-Wl,--hash-style=sysv

$(HELLORUST): hellorust.rs Makefile
	mkdir -p $(dir $(HELLORUST))
	rustc hellorust.rs --crate-type cdylib -o $(HELLORUST)

$(HELLOCPP): hellocpp.cpp Makefile hellocpp.map
	mkdir -p $(dir $(HELLOCPP))
	g++ \
		hellocpp.cpp \
		-g \
		-Wl,-soname,hellocpp/bin/libhellocpp.so \
		-Wl,--version-script=hellocpp.map \
		-Wl,-Bsymbolic \
		-Wl,-Bsymbolic-functions \
		-shared \
		-nolibc \
		-fPIC \
		-lc -L$(dir $(LIBC)) \
		-o $(HELLOCPP)

#libbase:
#	$(MAKE) -C libbase/src

#$(HELLOGLIBC): helloglibc.c Makefile helloglibc.map
#	mkdir -p $(dir $(HELLOGLIBC))
#	${CC} \
#		helloglibc.c \
#		-Wl,--no-undefined \
#		-Wl,-soname,helloglibc/bin/libhelloglibc.so \
#		-Wl,--version-script=helloglibc.map \
#		-Wl,-Bsymbolic \
#		-Wl,-Bsymbolic-functions \
#		-Wl,--hash-style=sysv \
#		-nostdlib \
#		-shared \
#		-fPIC \
#		-o $(HELLOGLIBC) \
#		-lc -L$(dir $(GLIBC)) \
#		-lbase -L$(dir $(LIBBASE))

$(THREAD): thread.c
	${CC} -g -o $(THREAD) thread.c

$(CONSTANTS): constants.c
	gcc -o $(CONSTANTS) constants.c
