for arch in x86_64 aarch64
do
	make clean
	CC=_zwolf/llvm/x86_64/bin/clang ARCH=${arch} LDFLAGS="-nostdlib -target ${arch}-unknown-linux-zwolf" CFLAGS="-target ${arch}-unknown-linux-zwolf" make
	cp libopenlibm.so.4.0 _zwolf_install/xlibc/${arch}/lib/libm.so
done
