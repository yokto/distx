#ifndef SYSTYPES_H
#define SYSTYPES_H
#ifdef __cplusplus
extern "C" {
#endif

#include <base/types.h>


typedef __INTMAX_TYPE__ intmax_t;
typedef __UINTMAX_TYPE__ uintmax_t;

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;
typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;

typedef __SIZE_TYPE__ size_t;
//#ifdef __SSIZE_TYPE__
//typedef __SSIZE_TYPE__ ssize_t;
//#else
typedef signed long ssize_t;
//#endif

typedef __PTRDIFF_TYPE__ ptrdiff_t;
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

#ifdef __cplusplus
}
#endif
#endif // SYSTYPES_H
