#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define UNUSED(x) (void)(x)

int debug_printf(char* restrict fmt, ...);

#define concatH(X, Y) X ## Y
#define concat(X, Y) concatH(X, Y)
#define concatH3(X, Y, Z) X ## Y ## Z
#define concat3(X, Y, Z) concatH3(X, Y, Z)

#endif // COMMON_H
