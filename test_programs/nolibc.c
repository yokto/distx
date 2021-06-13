#include "../lib/systemcalls.c"

extern foo;

const char* const hello_world = "hello world";

void _start() {

	const int written = write(1, (void*)hello_world, 11);

	char c;
      	read(0, &c, 1);

	exit2(written);
}

