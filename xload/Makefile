CC = gcc
DISTX_PREFIX ?= "distx.org_2024-"

all: xload xload_aarch xload.exe # load_elf_android

xload: load_elf.c
	mkdir -p _zwolf/${DISTX_PREFIX}xload-x86_64/bin
	${CC} -ldl -g -o _zwolf/${DISTX_PREFIX}xload-x86_64/bin/xload_linux load_elf.c

xload_aarch: load_elf.c
	mkdir -p _zwolf/${DISTX_PREFIX}xload-aarch64/bin
	aarch64-linux-gnu-gcc -ldl -g -o _zwolf/${DISTX_PREFIX}xload-aarch64/bin/xload_linux load_elf.c

xload.exe: load_elf.c
	mkdir -p _zwolf/${DISTX_PREFIX}xload-x86_64/bin
	x86_64-w64-mingw32-gcc load_elf.c -o _zwolf/${DISTX_PREFIX}xload-x86_64/bin/xload_windows.exe
	x86_64-w64-mingw32-gcc load_elf.c -D WIN_GUI -o _zwolf/${DISTX_PREFIX}xload-x86_64/bin/xload_windows_gui.exe

xload_android: load_elf.c
	mkdir -p ../build
	/nix/store/ycz9lysbv0hwv46v5a0nxlw15b5jf4ww-android-sdk-ndk-bundle-22.0.7026061/libexec/android-sdk/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang -o ../build/xload_android load_elf.c 
	#LD_LIBRARY_PATH=/run/current-system/sw/lib/ /home/silvio/Android/Sdk/ndk/23.1.7779620/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android24-clang -o ../build/xload_android load_elf.c
