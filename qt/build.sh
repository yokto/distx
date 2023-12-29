#!/bin/bash

ZWOLF=~/stuff/sources/elf_stage2
BUILD_DIR="${ZWOLF}/build-zwolf"
SRC="${PWD}"

mkdir -p "${BUILD_DIR}"
cd "$BUILD_DIR"

CC="${ZWOLF}/llvm/x86_64/bin/clang" CXX="${ZWOLF}/llvm/x86_64/bin/clang++" "${SRC}/configure" \
	-prefix "/_zwolf/qtbase" \
	-no-opengl \
	-no-feature-thread \
	-no-feature-testlib \
	-no-sql-sqlite \
	-no-feature-network \
	-qt-freetype \
	-- \
	"-DQT_HOST_PATH=/nix/store/gqirs9d77jb50c9fasszbc42zvmd1z0c-qtbase-6.6.0" \
	"-DQT_NO_PACKAGE_VERSION_CHECK=TRUE" \
	"-DINSTALL_INCLUDEDIR=common/include" \
	"-DINSTALL_LIBDIR=x86_64/lib" \
	"-DINSTALL_BINDIR=x86_64/lib" \
	"-DINSTALL_PLUGINSDIR=x86_64/plugins" \
	"-DINSTALL_LIBEXECDIR=x86_64/libexecs" \
	"-DINSTALL_MKSPECSDIR=common/mkspecs" \
	"-DINSTALL_ARCHDATADIR=/_zwolf/qtbase/x86_64" \
	"-DINSTALL_DESCRIPTIONSDIR=common/modules" \
	"-DINSTALL_DOCDIR=common/docs"
ninja
DESTDIR="${BUILD_DIR}" ninja install
