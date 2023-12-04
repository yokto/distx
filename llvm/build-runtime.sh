#!/bin/bash
ZWOLF=${ZWOLF:-${PWD}/_zwolf}
export CC=${ZWOLF}/llvm/x86_64/bin/clang
export CXX=${ZWOLF}/llvm/x86_64/bin/clang++ 
for arch in x86_64 aarch64
do
	mkdir -p "build-runtime-$arch"
	(
		cd "build-runtime-$arch"
		cmake ../runtimes \
			"-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi;libunwind" \
			"-DCMAKE_BUILD_TYPE=Release" \
			"-DCMAKE_C_COMPILER=${CC}" \
			"-DCMAKE_SYSTEM_NAME=zwolf" \
			"-DCMAKE_C_COMPILER_TARGET=${arch}-unknown-linux-zwolf" \
			"-DCMAKE_CXX_COMPILER_TARGET=${arch}-unknown-linux-zwolf" \
			"-DCMAKE_ASM_COMPILER_TARGET=${arch}-unknown-linux-zwolf" \
			"-DLLVM_DEFAULT_TARGET_TRIPLE=${arch}-unknown-linux-zwolf" \
			"-DCMAKE_C_COMPILER_FORCED=TRUE" \
			"-DCMAKE_CXX_COMPILER_FORCED=TRUE" \
			"-DDEFAULT_SYSROOT=/_zwolf" \
			"-DCMAKE_INSTALL_PREFIX=/_zwolf/llvm-libcxx" \
			"-DCMAKE_INSTALL_LIBDIR=${arch}/lib" \
			"-DCMAKE_INSTALL_INCLUDEDIR=common/include" \
			"-DLIBCXX_INCLUDE_BENCHMARKS:BOOL=OFF" \
			"-DLIBCXX_USE_COMPILER_RT=ON" \
			"-DLIBCXX_ENABLE_DEBUG_MODE=ON" \
			"-DLIBCXXABI_USE_COMPILER_RT=ON" \
			"-DLIBCXXABI_USE_LLVM_UNWINDER=ON" \
			"-DCOMPILER_RT_INSTALL_LIBRARY_DIR=lib" \
			"-DCOMPILER_RT_USE_BUILTINS_LIBRARY=ON" \
			"-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON" \
			"-G" "Ninja"
		ninja cxx cxxabi unwind
		DESTDIR=../_zwolf_install ninja install
	)
done
