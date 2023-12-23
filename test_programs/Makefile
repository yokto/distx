ZWOLF ?= ${PWD}/_zwolf
CC := ${ZWOLF}/llvm/x86_64/bin/clang
CXX := ${ZWOLF}/llvm/x86_64/bin/clang++
BUILDDIR := ${ZWOLF}/test_programs# $(realpath $(dir ${PWD}))
DEPDIR := ${ZWOLF}
LIBFOOBAR := $(BUILDDIR)/lib/libfoobar.so
HELLOC := $(BUILDDIR)/bin/helloc
LIBC := $(DEPDIR)/xlibc/lib/libc.so
#HELLORUST := $(BUILDDIR)/bin/hellorust
HELLOCPP := $(BUILDDIR)/hellocpp/bin/hellocpp
#TEST_IOSTREAM := $(BUILDDIR)/bin/test_iostream
CONSTANTS := $(BUILDDIR)/bin/constants

TESTCPP_SOURCES := $(wildcard testcpp_*.cpp)
TESTCPP_EXECS := $(patsubst %.cpp, $(BUILDDIR)/x86_64/bin/%, $(TESTCPP_SOURCES))
TESTCPP_EXECS += $(patsubst %.cpp, $(BUILDDIR)/aarch64/bin/%, $(TESTCPP_SOURCES))

TESTC_SOURCEST := $(wildcard testc_*.c)
TESTC_SOURCES = $(filter-out testc_zlib.c, $(TESTC_SOURCEST))
TESTC_EXECS := $(patsubst %.c, $(BUILDDIR)/x86_64/bin/%, $(TESTC_SOURCES))
TESTC_EXECS += $(patsubst %.c, $(BUILDDIR)/aarch64/bin/%, $(TESTC_SOURCES))

TESTCLIB_SOURCES := $(wildcard testclib_*.c)
TESTCLIB_EXECS := $(patsubst testclib_%.c, $(BUILDDIR)/x86_64/lib/libclib_%.so, $(TESTCLIB_SOURCES))
TESTCLIB_EXECS += $(patsubst testclib_%.c, $(BUILDDIR)/aarch64/lib/libclib_%.so, $(TESTCLIB_SOURCES))

TESTCGUI_SOURCES := $(wildcard testcgui_*.c)
TESTCGUI_EXECS := $(patsubst %.c, $(BUILDDIR)/x86_64/bin/%, $(TESTCGUI_SOURCES))
#TESTCGUI_EXECS := $(patsubst %.cpp, $(BUILDDIR)/aarch64/bin/%, $(TESTCGUI_SOURCES))

TESTQT_SOURCES := $(wildcard testqt_*.cpp)
TESTQT_EXECS := $(patsubst %.cpp, $(BUILDDIR)/x86_64/bin/%, $(TESTQT_SOURCES))

qt: $(TESTQT_EXECS)

all: \
	$(TESTCPP_EXECS) \
	$(TESTC_EXECS) \
	$(TESTCLIB_EXECS)
	#$(TESTCGUI_EXECS)

test:
	../build/load_elf _zwolf/test_programs/bin/testc_empty
	../build/load_elf _zwolf/test_programs/bin/testc_exit
	../build/load_elf _zwolf/test_programs/bin/testcpp_vector

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
		-Wl,-soname,/test_programs/bin/libhelloc.so \
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
	${CC} -Wl,-soname,/test_programs/lib/libfoobar.so -Wl,--version-script=foobar.map -nostdlib -shared -o $(LIBFOOBAR) foobar.c \
		-Wl,--hash-style=sysv

#$(HELLORUST): hellorust.rs Makefile
#	mkdir -p $(dir $(HELLORUST))
#	rustc hellorust.rs --crate-type cdylib -o $(HELLORUST)


#$(HELLOCPP): hellocpp.cpp Makefile hellocpp.map
#	mkdir -p $(dir $(HELLOCPP))
#	${CXX} \
#		hellocpp.cpp \
#		-g \
#		-Wl,-soname,/hellocpp/bin/libhellocpp.so \
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
#		"-Wl,-soname,/test_programs/bin/testc_zlib" \
#		-o "$@" "$<"

$(BUILDDIR)/x86_64/bin/testc_%: testc_%.c
	mkdir -p "$(BUILDDIR)/x86_64/bin"
	${CC} -femulated-tls -g \
		-lm \
		-target x86_64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/x86_64/bin/testc_$*" \
		-o "$@" "$<"

$(BUILDDIR)/aarch64/bin/testc_%: testc_%.c
	mkdir -p "$(BUILDDIR)/aarch64/bin"
	${CC} -femulated-tls -g \
		-lm \
		-target aarch64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/aarch64/bin/testc_$*" \
		-o "$@" "$<"

$(BUILDDIR)/x86_64/lib/libclib_%.so: testclib_%.c
	mkdir -p "$(BUILDDIR)/x86_64/lib"
	${CC} -femulated-tls -g \
		-lm \
		-shared \
		-target x86_64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/x86_64/lib/libclib_$*.so" \
		-o "$@" "$<"

$(BUILDDIR)/aarch64/lib/libclib_%.so: testclib_%.c
	mkdir -p "$(BUILDDIR)/aarch64/lib"
	${CC} -femulated-tls -g \
		-lm \
		-shared \
		-target aarch64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/aarch64/lib/libclib_$*.so" \
		-o "$@" "$<"

$(BUILDDIR)/x86_64/bin/testcgui_%: testcgui_%.c
	mkdir -p "$(BUILDDIR)/x86_64/bin"
	${CC} -femulated-tls -g \
		-lm \
		-lxgui \
		-L$(DEPDIR)/xgui/x86_64/lib \
		-target x86_64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/x86_64/bin/testcgui_$*" \
		-o "$@" "$<"

$(BUILDDIR)/x86_64/bin/testcpp_%: testcpp_%.cpp
	mkdir -p "$(BUILDDIR)/x86_64/bin"
	${CXX} -femulated-tls -g \
		"-Wl,--no-undefined" \
		-target x86_64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/x86_64/bin/testcpp_$*" \
		-o "$@" "$<"

$(BUILDDIR)/aarch64/bin/testcpp_%: testcpp_%.cpp
	mkdir -p "$(BUILDDIR)/aarch64/bin"
	${CXX} -femulated-tls -g \
		"-Wl,--no-undefined" \
		-target aarch64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/aarch/bin/testcpp_$*" \
		-o "$@" "$<"

$(CONSTANTS): constants.c
	mkdir -p "$(BUILDDIR)/bin"
	gcc -o $(CONSTANTS) constants.c

$(BUILDDIR)/x86_64/bin/testqt_%: testqt_%.cpp
	mkdir -p "$(BUILDDIR)/x86_64/bin"
	${CXX} -g \
		"-I${ZWOLF}/qtbase/common/include" \
		"-I${ZWOLF}/qtbase/common/include/QtCore" \
		"-lQt6Core" \
		"-lQt6Gui" \
		"-lQt6Widgets" \
		"-L${ZWOLF}/qtbase/x86_64/lib" \
		"-Wl,--no-undefined" \
		-target x86_64-unknown-linux-zwolf \
		"-Wl,-soname,/test_programs/x86_64/bin/testqt_$*" \
		-o "$@" "$<"
