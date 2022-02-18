#include <iostream>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

int* foolbar() {
	std::cout << "hello world2";
	return new int[3];
}

int *foo = foolbar();

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
	std::cout << "hello world";
	return 0;
}
