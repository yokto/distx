DISTX_PREFIX ?= "distx.org_2024-"
DISTX_ARCH ?= "x86_64"
DISTX_DEP ?= "${PWD}/_distx"
DISTX_INSTALL ?= "${PWD}/_distx"


all:
	mkdir -p ${DISTX_INSTALL}/${DISTX_PREFIX}xbuild-x86_64/bin
	mkdir -p ${DISTX_INSTALL}/${DISTX_PREFIX}xbuild-aarch64/bin
	${DISTX_DEP}/${DISTX_PREFIX}xload-${DISTX_ARCH}/bin/xload_linux ${DISTX_DEP}/${DISTX_PREFIX}llvm-${DISTX_ARCH}/bin/clang++ main.cpp -Wl,--soname=/${DISTX_PREFIX}xbuild-x86_64/bin/xbuild -o ${DISTX_INSTALL}/${DISTX_PREFIX}xbuild-x86_64/bin/xbuild
	${DISTX_DEP}/${DISTX_PREFIX}xload-${DISTX_ARCH}/bin/xload_linux ${DISTX_DEP}/${DISTX_PREFIX}llvm-${DISTX_ARCH}/bin/clang++ --target=aarch64-unknown-linux-zwolf main.cpp -Wl,--soname=/${DISTX_PREFIX}xbuild-aarch64/bin/xbuild -o ${DISTX_INSTALL}/${DISTX_PREFIX}xbuild-aarch64/bin/xbuild

