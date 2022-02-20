#include <stdio.h>
#include "foobar.h"

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

DLL_PUBLIC int main() {

	printf("hello world\n");
	printf("foo = %d\n", foobar_foo());
	printf("baz = %d\n", foobar_baz());
      	getchar();

	//foobar_bar++;
	return 42;
}

