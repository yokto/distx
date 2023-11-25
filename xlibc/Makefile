CC := _zwolf/llvm/x86_64/bin/clang
INSTALL := _zwolf_install/xlibc/
LIBC := $(INSTALL)x86_64/lib/libc.so $(INSTALL)aarch64/lib/libc.so
LIBZWOLF := $(INSTALL)x86_64/lib/libzwolf.so $(INSTALL)aarch64/lib/libzwolf.so

HEADERS = $(addprefix $(INSTALL)common/,$(wildcard include/*.h)) $(addprefix $(INSTALL)common/,$(wildcard include/*/*.h))

all: $(LIBC) $(HEADERS) $(LIBZWOLF)

$(INSTALL)%/lib/libc.so: $(INSTALL)%/lib/libzwolf.so libc.c printf.c string.c time.c unicode.c fs.c qsort.c proc.c thread_impl.c emutls.c Makefile
	mkdir -p $(INSTALL)$*/lib
	echo ${CC}
	${CC} \
		-g \
		-Wall -Wextra \
		-Wl,-soname,xlibc/$*/lib/libc.so \
		-Wl,--export-dynamic-symbol=__* \
		-Wl,--no-undefined \
		--target=$*-unknown-linux-zwolf \
		-Wno-unused-parameter \
		-nolibc -shared \
		-femulated-tls \
		--unwindlib=none \
		-lzwolf \
		-L "${INSTALL}/$*/lib" \
		-Ilibc \
		-I. \
		-fPIC \
		-Iinclude \
		-o $@ \
		emutls.c \
		libc.c \
		printf.c \
		string.c \
		proc.c \
		unicode.c \
		time.c \
		fs.c \
		qsort.c thread_impl.c

$(INSTALL)%/lib/libzwolf.so: libzwolf/libzwolf.c
	mkdir -p $(INSTALL)$*/lib
	echo ${CC}
	${CC} \
		-g \
		-Wl,-soname,_zwolf \
		--target=$*-unknown-linux-zwolf \
		-nolibc -shared \
		--unwindlib=none \
		-Iinclude \
		-o $@ \
		libzwolf/libzwolf.c

# copy headers
$(INSTALL)common/include/%.h: include/%.h
	mkdir -p $(dir $(INSTALL)common/include/$*h)
	@echo '#include "$*.h"' | ${CC} -x c --analyze -Iinclude -nostdinc -
	cp $< $@
