#include <stdio.h>
#include "foobar.h"

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

const char* const hello_world = "hello world\n";

DLL_PUBLIC int main() {

	//return foobar_foo();
//	foobar_baz();
//	foobar_foo();
	c_printf(hello_world);

	c_printf("foo = %d\n", foobar_foo());
	c_printf("foo = %d\n", foobar_baz());
      	c_getchar();

	//foobar_bar++;
	return 42;
}

