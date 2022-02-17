#include <iostream>

int* foolbar() {
	std::cout << "hello world2";
	return new int[3];
}

int *foo = foolbar();

int main2() {
	std::cout << "hello world";
	return 0;
}
