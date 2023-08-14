#include <stdio.h>
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
	return 0;
}
