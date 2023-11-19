#include <stdlib.h>
#include <stdio.h>
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
	printf("fini foo\n");
	free(foo);
}

int main() {
	return 42;
}

