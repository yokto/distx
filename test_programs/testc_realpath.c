#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if (argc == 2) {
		char path[10000];
		printf("realpath %s\n", realpath(argv[1], path));
	}
}

