#include <iostream>
#include <stdio.h>

struct Tiiii {
	Tiiii() {}
};
void f() {
	static Tiiii lsobj;
}


int main() {
	f();
	return 0;
}
