ZWOLF ?= ${PWD}/_distx
DISTX_PREFIX ?= distx.org_2024-
ZWOLF_INSTALL ?= ${PWD}/_distx_install
CC := ${ZWOLF}/${DISTX_PREFIX}llvm-x86_64/bin/clang
INSTALL ?= ${ZWOLF_INSTALL}/${DISTX_PREFIX}xlibc-
LIBC := $(INSTALL)x86_64/lib/libc.so $(INSTALL)aarch64/lib/libc.so
LIBZWOLF := $(INSTALL)x86_64/lib/libxload.so $(INSTALL)aarch64/lib/libxload.so

SRC := libc.c printf.c string.c time.c unicode.c fs.c qsort.c proc.c thread_impl.c emutls.c longjmp.c setjmp.c base/futex.c base/loop_impl.c


HEADERS = $(addprefix $(INSTALL)common/,$(wildcard include/*.h)) $(addprefix $(INSTALL)common/,$(wildcard include/*/*.h))

all: $(LIBC) $(HEADERS) $(LIBZWOLF)

$(INSTALL)%/lib/libc.so: $(INSTALL)%/lib/libxload.so $(SRC) Makefile
	mkdir -p $(INSTALL)$*/lib
	echo ${CC}
	${CC} \
		-g \
		-O2 \
		-Wall -Wextra \
		-Wl,-soname,/${DISTX_PREFIX}xlibc-$*/lib/libc.so \
		-Wl,--export-dynamic-symbol=__* \
		-Wl,--no-undefined \
		--target=$*-unknown-linux-zwolf \
		-Wno-unused-parameter \
		-Werror \
		-Wno-error=fuse-ld-path \
		-nolibc -shared \
		-femulated-tls \
		--unwindlib=none \
		-lxload \
		-L "${INSTALL}/$*/lib" \
		-Ilibc \
		-I. \
		-fPIC \
		-Iinclude \
		-o $@ \
		$(SRC)

$(INSTALL)%/lib/libxload.so: libxload/libxload.c
	mkdir -p $(INSTALL)$*/lib
	echo ${CC}
	${CC} \
		-g \
		-Wl,-soname,_xload \
		--target=$*-unknown-linux-zwolf \
		-nolibc -shared \
		--unwindlib=none \
		-Iinclude \
		-o $@ \
		libxload/libxload.c

# copy headers
$(INSTALL)common/include/%.h: include/%.h
	mkdir -p $(dir $(INSTALL)common/include/$*h)
	@echo '#include "$*.h"' | ${CC} -x c --analyze -Iinclude -nostdinc -
	cp $< $@
