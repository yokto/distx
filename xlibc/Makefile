CC := __zwolf_build__/llvm/bin/clang
INSTALL := __zwolf_run__/xlibc/
LIBC := $(INSTALL)lib/libc.so
LIBM := $(INSTALL)lib/libm.so

HEADERS = $(addprefix $(INSTALL),$(wildcard include/*.h)) $(addprefix $(INSTALL),$(wildcard include/*/*.h))

all: $(LIBC) $(HEADERS) # $(LIBM)

$(LIBC): libc.c printf.c string.c time.c unicode.c fs.c qsort.c proc.c thread_impl.c Makefile
	mkdir -p $(dir $(LIBC))
	echo ${CC}
	${CC} \
		-g \
		-Wall -Wextra \
		-Wl,-soname,xlibc/lib/libc.so \
		-Wl,--export-dynamic-symbol=__* \
		-Wno-unused-parameter \
		-nolibc -shared \
		-femulated-tls \
		-Ilibc \
		-I. \
		-fPIC \
		-Iinclude \
		-o $(LIBC) \
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
$(INSTALL)include/%.h: include/%.h
	mkdir -p $(dir $(INSTALL)include/$*h)
	@echo '#include "$*.h"' | ${CC} -x c --analyze -Iinclude -nostdinc -
	cp $< $@
