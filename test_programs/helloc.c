#include <stdio.h>
#include "foobar.h"

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

const char* const hello_world = "hello world\n";

DLL_PUBLIC int main() {

	//return foobar_foo();
//	foobar_baz();
//	foobar_foo();
	printf(hello_world);

	printf("foo = %d\n", foobar_foo());
	printf("foo = %d\n", foobar_baz());
      	getchar();

	//foobar_bar++;
	return 42;
}

