CC = gcc

all: load_elf load_elf.exe load_elf_android

load_elf: load_elf.c
	mkdir -p _zwolf/zwolf/x86_64/bin
	${CC} -ldl -g -o _zwolf/zwolf/x86_64/bin/load_elf_linux load_elf.c

load_elf.exe: load_elf.c
	mkdir -p _zwolf/zwolf/x86_64/bin
	x86_64-w64-mingw32-gcc load_elf.c -o _zwolf/zwolf/x86_64/bin/load_elf_windows.exe

load_elf_android: load_elf.c
	mkdir -p ../build
	/nix/store/ycz9lysbv0hwv46v5a0nxlw15b5jf4ww-android-sdk-ndk-bundle-22.0.7026061/libexec/android-sdk/ndk-bundle/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android29-clang -o ../build/load_elf_android load_elf.c 
	#LD_LIBRARY_PATH=/run/current-system/sw/lib/ /home/silvio/Android/Sdk/ndk/23.1.7779620/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android24-clang -o ../build/load_elf_android load_elf.c
