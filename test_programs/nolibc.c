#include "../lib/systemcalls.c"

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

extern int foo();
extern int bar;

const char* const hello_world = "hello world";

DLL_PUBLIC int main() {

	const int written = write(1, (void*)hello_world, 11);

	char c;
      	read(0, &c, 1);
	//foo();

	return 42;
	bar++;
	return;
}

