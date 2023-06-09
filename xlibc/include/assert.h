#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define assert(x) { \
	if (!(x)) { \
		fprintf(stderr, "assert failed in file " __FILE__ " on line %d\n", __LINE__); \
		*(int*)(NULL); \
	} \
}

#ifdef __cplusplus
}
#endif
#endif // ASSERT_H
