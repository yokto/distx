ZWOLF ?= _zwolf
CC := $(ZWOLF)/llvm/x86_64/bin/clang
INSTALL := $(ZWOLF)/xgui
XGUI := $(INSTALL)/x86_64/lib/libxgui.so $(INSTALL)/aarch64/lib/libxgui.so

HEADERS = $(addprefix $(INSTALL)common/,$(wildcard include/*.h)) $(addprefix $(INSTALL)common/,$(wildcard include/*/*.h))

all: $(XGUI) $(HEADERS)

$(INSTALL)/%/lib/libxgui.so: xgui.c xgui_windows.c
	mkdir -p $(INSTALL)/$*/lib
	echo ${CC}
	${CC} \
		-g \
		-Wall -Wextra \
		-Wl,-soname,/xgui/$*/lib/libxgui.so \
		-Wl,--export-dynamic-symbol=__* \
		-Wl,--no-undefined \
		--target=$*-unknown-linux-zwolf \
		-Wno-unused-parameter \
		-shared \
		--unwindlib=none \
		-lzwolf \
		-L "$(ZWOLF)/xlibc/$*/lib" \
		-I. \
		-fPIC \
		-Iinclude \
		-o $@ \
		xgui.c \
		xgui_windows.c

# copy headers
$(INSTALL)common/include/%.h: include/%.h
	mkdir -p $(dir $(INSTALL)common/include/$*h)
	@echo '#include "$*.h"' | ${CC} -x c --analyze -Iinclude -nostdinc -
	cp $< $@
