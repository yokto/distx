CC ?= gcc
INSTALL := $(dir ${PWD})
LIBC := $(INSTALL)lib/libc.so

HEADERS = $(addprefix $(INSTALL),$(wildcard include/*.h))

all: $(LIBC) $(HEADERS)

$(LIBC): libc.c Makefile libc.map
	mkdir -p $(dir $(LIBC))
	${CC} \
		-g \
		-Wall -Wextra -Werror \
		-Wl,-soname,xlibc/lib/libc.so \
		-Wl,--version-script=libc.map \
		-Wl,--hash-style=sysv \
		-Wl,--export-dynamic-symbol=__* \
		-Wl,-Bsymbolic \
		-nostdlib -shared \
		-fno-stack-protector \
		-fPIC \
		-fno-builtin \
		-Ilibc \
		-o $(LIBC) \
		libc.c \
		-lc -L$(dir $(LIBC)) 

# copy headers
$(INSTALL)include/%.h: include/%.h
	mkdir -p $(INSTALL)include
	cp $< $@
