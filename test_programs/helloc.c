#include <stdio.h>
#include "foobar.h"

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

DLL_PUBLIC int foo = 1;

DLL_PUBLIC int main() {

	printf("hello wörld\n");
	printf("foo = %d\n", foobar_foo());
	printf("baz = %d\n", foobar_baz());
      	getchar();

	printf("before %d\n", foobar_bar);
	foobar_bar++;
	printf("after %d\n", foobar_bar);
	return 42;
}

