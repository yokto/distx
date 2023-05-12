#ifndef STDINT_H
#define STDINT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned long long uint64_t;
typedef long long int64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef long intmax_t;
typedef unsigned long uintmax_t;

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

#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffff
#define UINT64_MAX  0xffffffffffffffff
#define SIZE_MAX ((size_t)UINT64_MAX)

#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  ((x) + (UINT32_MAX - UINT32_MAX))
#define UINT64_C(x)  ((x) + (UINT64_MAX - UINT64_MAX))

#define PRIu64 "llu"
#define PRIx64 "llx"
#define PRId64 "lld"
#define PRIxPTR "p"
#define PRIu8 "u"
#define PRIdPTR "p"

#ifdef __cplusplus
}
#endif
#endif // STDINT_H
