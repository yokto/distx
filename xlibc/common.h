#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define UNUSED(x) (void)(x)
void __exit(int ret) __attribute__ ((weak));
int __write(char* str) __attribute__ ((weak));
int __resolve(void* ptr, const char ** libfile, size_t* offset);
int __debug_printf(char* restrict fmt, ...);
extern int __errno();
void* __dlsym(void* handle, char* name) __attribute__ ((weak));
void* __dlopen(char* name) __attribute__ ((weak));

#define concatH(X, Y) X ## Y
#define concat(X, Y) concatH(X, Y)
#define concatH3(X, Y, Z) X ## Y ## Z
#define concat3(X, Y, Z) concatH3(X, Y, Z)

#endif // COMMON_H
