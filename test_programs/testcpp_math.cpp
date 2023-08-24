#include <cmath>
#include <stdio.h>
int main(int argc, char** argv) {
	if (argc != 2) {
		printf("./testcpp_math 1.2341\n");
		return 0;
	}
	double f = atof(argv[1]);
	printf("f %d\n", (int)(100*f));
	printf("sin(f) %d\n", (int)(100*sin(f)));
	return 0;
}
