CC := __zwolf_build__/llvm/bin/clang
CXX := __zwolf_build__/llvm/bin/clang++
BUILDDIR := __zwolf_run__/test_programs# $(realpath $(dir ${PWD}))
DEPDIR := __zwolf_run__# $(realpath $(dir ${PWD})/..)
LIBFOOBAR := $(BUILDDIR)/lib/libfoobar.so
HELLOC := $(BUILDDIR)/bin/helloc
LIBC := $(DEPDIR)/xlibc/lib/libc.so
#LIBBASE := $(BUILDDIR)/libbase/lib/libbase.so
HELLORUST := $(BUILDDIR)/hellorust/bin/hellorust
HELLOCPP := $(BUILDDIR)/hellocpp/bin/hellocpp
#TEST_IOSTREAM := $(BUILDDIR)/bin/test_iostream
CONSTANTS := $(BUILDDIR)/bin/constants

TESTCPP_SOURCES := $(wildcard testcpp_*.cpp)
TESTCPP_EXECS := $(patsubst %.cpp, $(BUILDDIR)/bin/%, $(TESTCPP_SOURCES))
TESTC_SOURCES := $(wildcard testc_*.c)
TESTC_EXECS := $(patsubst %.c, $(BUILDDIR)/bin/%, $(TESTC_SOURCES))

all: $(HELLORUST) \
	$(LIBBASE) \
	$(CONSTANTS) \
	$(TESTCPP_EXECS) \
	$(TESTC_EXECS)

test: test 
	../build/load_elf __zwolf_run__/test_programs/bin/testc_empty
	../build/load_elf __zwolf_run__/test_programs/bin/testc_exit
	../build/load_elf __zwolf_run__/test_programs/bin/testcpp_vector

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


#$(HELLOCPP): hellocpp.cpp Makefile hellocpp.map
#	mkdir -p $(dir $(HELLOCPP))
#	${CXX} \
#		hellocpp.cpp \
#		-g \
#		-Wl,-soname,hellocpp/bin/libhellocpp.so \
#		-Wl,--version-script=hellocpp.map \
#		-Wl,-Bsymbolic \
#		-Wl,-Bsymbolic-functions \
#		-shared \
#		-nolibc \
#		-fPIC \
#		-lc -L$(dir $(LIBC)) \
#		-o $(HELLOCPP)

#libbase:
#	$(MAKE) -C libbase/src

$(BUILDDIR)/bin/testc_zlib: testc_zlib.c
	mkdir -p "$(BUILDDIR)/bin"
#	${CC} -femulated-tls -g \
#		"-lz" \
#		"-L/__zwolf_builddir__/zlib/lib" \
#		"-I/__zwolf_builddir__/zlib/include" \
#		"-Wl,-soname,test_programs/bin/testc_zlib" \
#		-o "$@" "$<"

$(BUILDDIR)/bin/testc_%: testc_%.c
	mkdir -p "$(BUILDDIR)/bin"
	${CC} -femulated-tls -g \
		"-Wl,-soname,test_programs/bin/testc_$*" \
		-o "$@" "$<"

$(BUILDDIR)/bin/testcpp_%: testcpp_%.cpp
	mkdir -p "$(BUILDDIR)/bin"
	${CXX} -femulated-tls -g \
		"-Wl,-soname,test_programs/bin/testcpp_$*" \
		-o "$@" "$<"

$(CONSTANTS): constants.c
	mkdir -p "$(BUILDDIR)/bin"
	gcc -o $(CONSTANTS) constants.c
