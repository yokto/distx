//#include <iostream>
#include <stdlib.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

int* foolbar() {
	return (int*)realloc(0, sizeof(int) * 8);
}
//
static int *foo = foolbar();
//
//class Foolbar {
//	public:
//	int a;
//	int testflight();	
//};
//int Foolbar::testflight() {
//	return a;
//}

extern "C"
DLL_PUBLIC
int main() {
	//Foolbar f = Foolbar();
	//std::cout << "hello world";
	return 42;
}
