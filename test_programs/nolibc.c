#include "../lib/systemcalls.c"

extern int foo();
extern int bar;

const char* const hello_world = "hello world";

void _start() {

	const int written = write(1, (void*)hello_world, 11);

	char c;
      	read(0, &c, 1);
	foo();

	exit2(42);
	bar++;
	exit2(bar);
}

