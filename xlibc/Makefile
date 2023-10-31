CC := __zwolf_build__/llvm/bin/clang
INSTALL := __zwolf_run__/xlibc/
LIBC := $(INSTALL)x86_64/lib/libc.so $(INSTALL)arm/lib/libc.so
LIBM := $(INSTALL)x86_64/lib/libm.so

HEADERS = $(addprefix $(INSTALL)common/,$(wildcard include/*.h)) $(addprefix $(INSTALL)common/,$(wildcard include/*/*.h))

all: $(LIBC) $(HEADERS) # $(LIBM)

$(INSTALL)%/lib/libc.so: libc.c printf.c string.c time.c unicode.c fs.c qsort.c proc.c thread_impl.c Makefile
	mkdir -p $(INSTALL)$*/lib
	echo ${CC}
	${CC} \
		-g \
		-Wall -Wextra \
		-Wl,-soname,xlibc/$*/lib/libc.so \
		-Wl,--export-dynamic-symbol=__* \
		--target=$*-unknown-linux-zwolf \
		-Wno-unused-parameter \
		-nolibc -shared \
		-femulated-tls \
		--unwindlib=none \
		-Ilibc \
		-I. \
		-fPIC \
		-Iinclude \
		-o $@ \
		libc.c \
		printf.c \
		string.c \
		proc.c \
		unicode.c \
		time.c \
		fs.c \
		qsort.c thread_impl.c

#               -Werror \

#$(LIBM): libm.c Makefile libm.map
#	mkdir -p $(dir $(LIBC))
#	${CC} \
#		-g \
#		-Wall -Wextra -Werror \
#		-Wl,-soname,xlibc/lib/libm.so \
#		-nostdlib -shared \
#		-fno-stack-protector \
#		-fPIC \
#		-fno-builtin \
#		-Ilibc \
#		-Iinclude \
#		-o $(LIBM) \
#		libm.c \
#		-lc -L$(dir $(LIBC)) 

# copy headers
$(INSTALL)common/include/%.h: include/%.h
	mkdir -p $(dir $(INSTALL)common/include/$*h)
	@echo '#include "$*.h"' | ${CC} -x c --analyze -Iinclude -nostdinc -
	cp $< $@
