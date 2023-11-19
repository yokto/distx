#include <stdio.h>
#include <cstdlib>
class Foo {
	public:
	Foo() {
		printf("foo\n");
	}
	~Foo() {
		printf("free foo\n");
	}
};

Foo foo = Foo();

int main() {
	exit(1);
	return 0;
}
