#!/bin/bash
ZWOLF=${ZWOLF:-$PWD/_zwolf}
export CC="$ZWOLF/${HOST_PREFIX}llvm-x86_64/bin/clang"
export CXX="$ZWOLF/${HOST_PREFIX}llvm-x86_64/bin/clang++"
echo CC = $CC
for arch in x86_64 aarch64
do
	mkdir -p build-compiler-rt-${arch}
	(
		cd build-compiler-rt-${arch}
		cmake ../runtimes \
			"-DLLVM_ENABLE_RUNTIMES=compiler-rt" \
			"-DLLVM_ENABLE_PER_TARGET_RUNTIME_DIR=ON" \
			"-DCMAKE_BUILD_TYPE=Release" \
			"-DCMAKE_C_COMPILER=${CC}" \
			"-DCMAKE_SYSTEM_NAME=zwolf" \
			"-DCMAKE_INSTALL_PREFIX=/_zwolf/${HOST_PREFIX}llvm-rt-${arch}" \
			"-DCMAKE_INSTALL_INCLUDEDIR=/_zwolf/${HOST_PREFIX}llvm-rt-common/include" \
			"-DCMAKE_C_COMPILER_TARGET=${arch}-unknown-linux-zwolf" \
			"-DCMAKE_ASM_COMPILER_TARGET=${arch}-unknown-linux-zwolf" \
			"-DCMAKE_ASM_FLAGS=-target ${arch}-unknown-linux-zwolf" \
			"-DLLVM_DEFAULT_TARGET_TRIPLE=${arch}-unknown-linux-zwolf" \
			"-DLLVM_TARGET_TRIPLE=${arch}-unknown-linux-zwolf" \
			"-DDEFAULT_SYSROOT=/_zwolf" \
			"-DCOMPILER_RT_BUILD_BUILTINS=ON" \
			"-DCOMPILER_RT_BUILD_LIBFUZZER=OFF" \
			"-DCOMPILER_RT_BUILD_MEMPROF=OFF" \
			"-DCOMPILER_RT_BUILD_PROFILE=OFF" \
			"-DCOMPILER_RT_BUILD_SANITIZERS=OFF" \
			"-DCOMPILER_RT_BUILD_XRAY=OFF" \
			"-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON" \
			"-DCOMPILER_RT_INSTALL_LIBRARY_DIR:PATH=lib" \
			"-DCMAKE_C_COMPILER_FORCED=TRUE" \
			"-DCMAKE_CXX_COMPILER_FORCED=TRUE" \
			"-G" "Ninja"
		ninja compiler-rt
		ninja install
	)
done
