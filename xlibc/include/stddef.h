#ifndef STDDEF_H
#define STDDEF_H
#ifdef __cplusplus
extern "C" {
#endif

#define NULL 0
#define offsetof(st, m) \
    ((size_t)&(((st *)0)->m))

typedef __SIZE_TYPE__ size_t;
typedef signed long ssize_t;
//#ifdef __SSIZE_TYPE__
//typedef __SSIZE_TYPE__ ssize_t;
//#else
//#endif

typedef long ptrdiff_t;
typedef __WCHAR_TYPE__ wchar_t;

#ifdef __cplusplus
}
#endif
#endif // STDDEF_H
