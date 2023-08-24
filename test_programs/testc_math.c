#include <math.h>
#include <stdio.h>
float foo(float i) {
	return sinf(i);
}
int main() {
	return foo(3.2);
}
