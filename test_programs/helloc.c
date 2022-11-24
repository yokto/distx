#include <stdio.h>
#include <stdlib.h>
#include "foobar.h"

char* foo = 0;

__attribute__((constructor)) void init_foo() {
	printf("init foo\n");
	foo = malloc(4);
	foo[0] = 'f';
	foo[1] = 'o';
	foo[2] = 'o';
	foo[3] = '\0';
}

__attribute__((destructor)) void fini_foo() {
	//printf("fini foo\n");
	free(foo);
}

int main() {
//	realloc(0, 232);

	printf("hello wörld\n");
	printf("3 = %d\n", 3);
	int a = *(int*)0;
	return 3;
	printf("foo = %d\n", foobar_foo());
	printf("baz = %d\n", foobar_baz());
	getchar();

	printf("foo = %s\n", foo);

	printf("before %d\n", foobar_bar);
	foobar_bar++;
	printf("after %d\n", foobar_bar);
	return 42;
}

