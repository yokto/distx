CC ?= gcc
LIBBASE := ../lib/libbase.so

all: $(LIBBASE)
$(LIBBASE): *
	mkdir -p $(dir $(LIBBASE))
	${CC} \
		-Wl,-soname,libbase/lib/libbase.so \
		-Wl,--version-script=libbase.map \
		-Wl,-Bsymbolic \
		-Wl,-Bsymbolic-functions \
		-Wl,--hash-style=sysv \
		-nostdlib -shared \
		-fPIC \
		-fno-builtin \
		-Ilibbase \
		-o $(LIBBASE) \
		base.c
