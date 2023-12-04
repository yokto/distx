#!/bin/bash
ZWOLF=${ZWOLF:-${PWD}/_zwolf}
export CC="clang"
export CXX="clang++"
export PATH=$"${ZWOLF}/llvm/x86_64/bin:$PATH"
TBLGEN="${ZWOLF}/llvm/x86_64/bin/tblgen"
TOOLS="${ZWOLF}/llvm/x86_64/bin"
mkdir -p build_release_archs
(
	cd build_release_archs
	cmake ../llvm \
		"-DLLVM_ENABLE_PROJECTS=clang;lld" \
		"-DCMAKE_BUILD_TYPE=Release" \
		"-DCMAKE_C_COMPILER=${CC}" \
		"-DDEFAULT_SYSROOT=/_zwolf" \
		"-DCMAKE_SYSTEM_NAME=zwolf" \
		"-DLLVM_NATIVE_TOOL_DIR=${TOOLS}" \
		"-DCMAKE_INSTALL_PREFIX=/_zwolf/llvm" \
		"-DLLVM_DEFAULT_TARGET_TRIPLE=x86_64-unknown-linux-zwolf" \
		"-DLLVM_HOST_TRIPLE=x86_64-unknown-linux-zwolf" \
		"-DLLVM_TARGETS_TO_BUILD=X86;AArch64" \
		"-DLLVM_INCLUDE_BENCHMARKS=OFF" \
		"-DLLVM_ENABLE_ZLIB=OFF" \
		"-DLLVM_ENABLE_LLD=ON" \
		"-DLLVM_ENABLE_TERMINFO=OFF" \
		"-DLIBCXX_USE_COMPILER_RT=ON" \
		"-DLIBCXXABI_USE_COMPILER_RT=ON" \
		"-DLLVM_BUILD_LLVM_DYLIB=ON" \
                "-DLLVM_LINK_LLVM_DYLIB=ON" \
		"-DCMAKE_INSTALL_BINDIR=x86_64/bin" \
		"-DLLVM_TOOLS_INSTALL_DIR=x86_64/bin" \
		"-DCMAKE_INSTALL_LIBDIR=x86_64/lib" \
		"-DCMAKE_INSTALL_LIBEXECDIR=x86_64/libexec" \
		"-DCLANG_INSTALL_PACKAGE_DIR=x86_64/cmake/clang" \
		"-DLLVM_INSTALL_PACKAGE_DIR=x86_64/cmake/clang" \
		"-DLLD_INSTALL_PACKAGE_DIR=x86_64/cmake/clang" \
		"-DCMAKE_INSTALL_INCLUDEDIR=common/include" \
		"-DCMAKE_INSTALL_DATAROOTDIR=common/share" \
		"-DCLANG_DEFAULT_LINKER=/_zwolf/llvm/x86_64/bin/lld" \
		"-DCLANG_TBLGEN_EXE=${TBLGEN}" \
		"-DCLANG_BUILD_TOOLS:BOOL=ON" \
		"-DCLANG_ENABLE_ARCMT=OFF" \
		"-DCOMPILER_RT_INCLUDE_TESTS=OFF" \
		"-DCLANG_DEFAULT_RTLIB=compiler-rt" \
		"-DCLANG_DEFAULT_UNWINDLIB=libunwind" \
		"-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON" \
		"-DCLANG_DEFAULT_CXX_STDLIB=libc++" \
		"-DLIBCXX_ENABLE_DEBUG_MODE=ON" \
		"-DLIBUNWIND_USE_COMPILER_RT=ON" \
		"-G" "Ninja"
	ninja
	DESTDIR=../_zwolf_install ninja install
)
