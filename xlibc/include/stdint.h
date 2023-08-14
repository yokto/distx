#ifndef STDINT_H
#define STDINT_H
#ifdef __cplusplus
extern "C" {
#endif

typedef __UINT8_TYPE__ uint8_t;
typedef __INT8_TYPE__ int8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __INT16_TYPE__ int16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __INT32_TYPE__ int32_t;
typedef __UINT64_TYPE__ uint64_t;
typedef __INT64_TYPE__ int64_t;

typedef __INTPTR_TYPE__ intptr_t;
typedef __UINTPTR_TYPE__ uintptr_t;

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

#define UINT8_MAX   0xff
#define UINT16_MAX  0xffff
#define UINT32_MAX  0xffffffff
#define UINT64_MAX  0xffffffffffffffff
#define INT8_MAX   ((int8_t)0x7f)
#define INT16_MAX  ((int16_t)0x7fff)
#define INT32_MAX  ((int32_t)0x7fffffff)
#define INT64_MAX  ((int64_t)0x7fffffffffffffff)
#define UINT8_MIN   0
#define UINT16_MIN  0
#define UINT32_MIN  0
#define UINT64_MIN  0
#define INT8_MIN   ((int8_t)-0x80)
#define INT16_MIN  ((int16_t)-0x8000)
#define INT32_MIN  ((int32_t)-0x80000000)
#define INT64_MIN  ((int64_t)-0x8000000000000000)
#define SIZE_MAX __SIZE_MAX__

#define UINT8_C(x)   (x)
#define UINT16_C(x)  (x)
#define UINT32_C(x)  ((x) + (UINT32_MAX - UINT32_MAX))
#define UINT64_C(x)  ((x) + (UINT64_MAX - UINT64_MAX))

#define INT8_C(x)    (x)
#define INT16_C(x)   (x)
#define INT32_C(x)   ((x) + (INT32_MAX - INT32_MAX))
#define INT64_C(x)   ((x) + (INT64_MAX - INT64_MAX))

#define PRIu8 "u"
#define PRIu16 "u"
#define PRIu32 "u"
#define PRIu64 "lu"
#define PRIuPTR "lu"

#define PRIX8 "X"
#define PRIX16 "X"
#define PRIX32 "X"
#define PRIX64 "lX"
#define PRIXPTR "lX"

#define PRIx8 "x"
#define PRIx16 "x"
#define PRIx32 "x"
#define PRIx64 "lx"
#define PRIxPTR "lx"

#define PRIo8 "o"
#define PRIo16 "o"
#define PRIo32 "o"
#define PRIo64 "lo"
#define PRIoPTR "lo"

#define PRId8 "d"
#define PRId16 "d"
#define PRId32 "d"
#define PRId64 "ld"
#define PRIdPTR "ld"

#ifdef __cplusplus
}
#endif
#endif // STDINT_H
