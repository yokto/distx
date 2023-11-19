[
	[".", "mkdir", "-p", "build-runtime"],
	["build-runtime",
		"env",
		"CC=clang",
		"CXX=clang++",
		"cmake",
		"../runtimes",
		"-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi;libunwind",
		"-DCMAKE_BUILD_TYPE=Release",
		"-DCMAKE_C_COMPILER=clang",
		"-DCMAKE_SYSTEM_NAME=zwolf",
		"-DCMAKE_C_COMPILER_TARGET=x86_64-unknown-linux-zwolf",
		"-DCMAKE_C_COMPILER_FORCED=TRUE",
		"-DCMAKE_CXX_COMPILER_FORCED=TRUE",
		"-DDEFAULT_SYSROOT=/home/silvio/stuff/sources/elf_release_linux",
		"-DCMAKE_INSTALL_PREFIX=/__zwolf_run__/llvm-libcxx",
		"-DCMAKE_INSTALL_LIBDIR=x86_64/lib",
		"-DCMAKE_INSTALL_INCLUDEDIR=common/include",
		"-DLIBCXX_INCLUDE_BENCHMARKS:BOOL=OFF",
		"-DLIBCXX_USE_COMPILER_RT=ON",
		"-DLIBCXX_ENABLE_DEBUG_MODE=ON",
		"-DLIBCXXABI_USE_COMPILER_RT=ON",
		"-DLIBCXXABI_USE_LLVM_UNWINDER=ON",
		"-DCOMPILER_RT_INSTALL_LIBRARY_DIR=lib",
		"-DCOMPILER_RT_USE_BUILTINS_LIBRARY=ON",
		"-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON",
		"-G", "Ninja"
	],
	["build-runtime", "ninja", "cxx", "cxxabi", "unwind"],
	["build-runtime", "env", "DESTDIR=..", "ninja", "install"],
	[".", "mkdir", "-p", "build-runtime-arm"],
	["build-runtime-arm",
		"env",
		"CC=clang",
		"CXX=clang++",
		"cmake",
		"../runtimes",
		"-DLLVM_ENABLE_RUNTIMES=libcxx;libcxxabi;libunwind",
		"-DCMAKE_BUILD_TYPE=Release",
		"-DCMAKE_C_COMPILER=clang",
		"-DCMAKE_SYSTEM_NAME=zwolf",
		"-DCMAKE_C_COMPILER_TARGET=aarch64-unknown-linux-zwolf",
		"-DCMAKE_CXX_COMPILER_TARGET=aarch64-unknown-linux-zwolf",
		"-DCMAKE_ASM_COMPILER_TARGET=aarch64-unknown-linux-zwolf",
		"-DLLVM_DEFAULT_TARGET_TRIPLE=aarch64-unknown-linux-zwolf",
		"-DCMAKE_C_COMPILER_FORCED=TRUE",
		"-DCMAKE_CXX_COMPILER_FORCED=TRUE",
		"-DDEFAULT_SYSROOT=/home/silvio/stuff/sources/elf_release_linux",
		"-DCMAKE_INSTALL_PREFIX=/__zwolf_run__/llvm-libcxx",
		"-DCMAKE_INSTALL_LIBDIR=aarch64/lib",
		"-DCMAKE_INSTALL_INCLUDEDIR=common/include",
		"-DLIBCXX_INCLUDE_BENCHMARKS:BOOL=OFF",
		"-DLIBCXX_USE_COMPILER_RT=ON",
		"-DLIBCXX_ENABLE_DEBUG_MODE=ON",
		"-DLIBCXXABI_USE_COMPILER_RT=ON",
		"-DLIBCXXABI_USE_LLVM_UNWINDER=ON",
		"-DCOMPILER_RT_INSTALL_LIBRARY_DIR=lib",
		"-DCOMPILER_RT_USE_BUILTINS_LIBRARY=ON",
		"-DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON",
		"-G", "Ninja"
	],
	["build-runtime-arm", "ninja", "cxx", "cxxabi", "unwind"],
	["build-runtime-arm", "env", "DESTDIR=..", "ninja", "install"]
]
