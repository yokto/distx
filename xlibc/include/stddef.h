#ifndef STDDEF_H
#define STDDEF_H

#define NULL 0
#define offsetof(st, m) \
    ((size_t)&(((st *)0)->m))

typedef unsigned long size_t;
typedef long ssize_t;

typedef long ptrdiff_t;
typedef int wchar_t;

#endif // STDDEF_H
