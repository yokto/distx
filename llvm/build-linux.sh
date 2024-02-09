#!/bin/bash
set -e
set -x

if [ "$STAGE1" == "" ]
then
	STAGE1="/_zwolf"
fi

if [ "$HOST_PREFIX" == "" ]
then
	HOST_PREFIX="github.com+2024+"
fi

export CC=clang
export CXX=clang++

mkdir -p build_linux
(
	cd build_linux
	cmake ../llvm \
		"-DLLVM_ENABLE_PROJECTS=clang;lld" \
		"-DCMAKE_BUILD_TYPE=Release" \
		"-DLLVM_BUILD_LLVM_DYLIB=ON" \
                "-DLLVM_LINK_LLVM_DYLIB=ON" \
		"-DCLANG_LINK_CLANG_DYLIB=ON" \
		"-DCMAKE_C_COMPILER=clang" \
		"-DDEFAULT_SYSROOT=${STAGE1}" \
		"-DCMAKE_INSTALL_PREFIX=${STAGE1}" \
		"-DCMAKE_INSTALL_BINDIR=${HOST_PREFIX}llvm-x86_64/bin" \
		"-DCMAKE_INSTALL_LIBDIR=${HOST_PREFIX}llvm-x86_64/lib" \
		"-DCMAKE_INSTALL_LIBEXECDIR=${HOST_PREFIX}llvm-x86_64/libexec" \
		"-DCMAKE_INSTALL_MANDIR=${HOST_PREFIX}llvm-doc-common/share/man" \
		"-DCMAKE_INSTALL_DATADIR=${HOST_PREFIX}llvm-common/share" \
		"-DCMAKE_INSTALL_INCLUDEDIR=${HOST_PREFIX}llvm-common/include" \
		"-DCMAKE_INSTALL_PACKAGEDIR=${HOST_PREFIX}llvm-dev-x86_64/lib/cmake" \
		"-DLLVM_TOOLS_INSTALL_DIR=${HOST_PREFIX}llvm-x86_64/bin" \
		"-DCLANG_TOOLS_INSTALL_DIR=${HOST_PREFIX}llvm-x86_64/bin" \
		"-DLLVM_UTILS_INSTALL_DIR=${HOST_PREFIX}llvm-utils-x86_64/bin" \
		"-DCPACK_PACKAGE_INSTALL_DIRECTORY=${HOST_PREFIX}llvm-cpack-x86_64/include" \
		"-DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-unknown-linux-zwolf" \
		"-DLLVM_TARGETS_TO_BUILD=X86;AArch64" \
		"-DLLVM_INCLUDE_BENCHMARKS=OFF" \
		"-DLLVM_ENABLE_TERMINFO=OFF" \
		"-DLIBCXX_USE_COMPILER_RT=ON" \
		"-DLIBCXXABI_USE_COMPILER_RT=ON" \
		"-DCLANG_DEFAULT_LINKER=lld" \
		"-DCLANG_BUILD_TOOLS:BOOL=ON" \
		"-DCLANG_ENABLE_ARCMT=OFF" \
		"-DCOMPILER_RT_INCLUDE_TESTS=OFF" \
		"-DCLANG_DEFAULT_RTLIB=compiler-rt" \
		"-DCLANG_DEFAULT_UNWINDLIB=libunwind" \
		"-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON" \
		"-DCLANG_DEFAULT_CXX_STDLIB=libc++" \
		"-DLIBCXX_ENABLE_DEBUG_MODE=ON" \
		"-DLIBCXX_PREFIX=/${HOST_PREFIX}llvm-libcxx-" \
		"-DRT_PREFIX=/${HOST_PREFIX}llvm-rt-" \
		"-DLLVM_PREFIX=/${HOST_PREFIX}llvm-" \
		"-DXLIBC_PREFIX=/${HOST_PREFIX}xlibc-" \
		"-G" "Ninja"
	ninja
	ninja install
	rm ${STAGE1}/${HOST_PREFIX}llvm-x86_64/lib/lib*.a
)
