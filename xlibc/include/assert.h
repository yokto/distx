#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stddef.h>

#define assert(x) { \
	if (!(x)) { \
		fprintf(stderr, "assert failed in file " __FILE__ " on line %d\n", __LINE__); \
		return *(int*)(NULL); \
	} \
}

#endif // ASSERT_H
