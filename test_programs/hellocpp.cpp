//#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

int* foolbar() {
	printf("foo\n");
	return (int*)realloc(0, sizeof(int) * 8);
}
//
static int *foo = foolbar();

class Foolbar {
	public:
	int a;
	int testflight();	
};
int Foolbar::testflight() {
	return a;
}

extern "C"
DLL_PUBLIC
int main() {
	Foolbar f = Foolbar();
	printf("bar\n");
	//std::cout << "hello world";
	return 42;
}
