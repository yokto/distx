#include "foobar.h"

int foobar_foo() {
	return 42;
}

__asm__(".symver foobar_baz_1, foobar_baz@foobar_1.0.0, remove");
int foobar_baz_1() {
	return 43;
}

int foobar_baz() {
	return 43;
}

int foobar_bar = 32;
