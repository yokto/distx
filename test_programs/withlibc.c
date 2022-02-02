#include <stdio.h>

extern int foo();
extern int bar;

const char* const hello_world = "hello world";

int main() {

	foo();
	baz();
	foo();
	const int written = write(1, (void*)hello_world, 11);

	char c;
      	read(0, &c, 1);

	return 42;
	bar++;
	return bar;
}

