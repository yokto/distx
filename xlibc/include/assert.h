#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NDEBUG
#define assert(x) ((x) ? (void)0 : __builtin_trap())
#else
#define assert(x) ((void)0)
#endif

#ifdef __cplusplus
}
#endif
#endif // ASSERT_H
