#pragma once

#include <stdint.h> 

#ifdef __cplusplus
extern "C" {
#endif


#define FP_NAN 0
#define FP_INFINITE 1
#define FP_ZERO 2
#define FP_SUBNORMAL 3    
#define FP_NORMAL 4

#define INFINITY (__builtin_inff())
#define HUGE_VALF (__builtin_huge_valf())


// eventually remove these macros
#define concatH4321(X, Y) X ## Y
#define concat4321(X, Y) concatH4321(X, Y)
#define MATH_BUILTIN_1(name) \
	extern double name(double x); \
	extern float concat4321(name, f)(float x); \
	extern long double concat4321(name, l)(long double x);

extern double modf(double x, double *iptr);
extern float modff(float x, float *iptr);
extern long double modfl(long double x, long double *iptr);

extern double atan2(double y, double x);
extern float atan2f(float y, float x);
extern long double atan2l(long double y, long double x);

extern double ldexp(double x, int exp);
extern float ldexpf(float x, int exp);
extern long double ldexpl(long double x, int exp);

double frexp(double x, int* exp);
float frexpf(float x, int* exp);
long double frexpl(long double x, int* exp);

extern double pow(double x, double y);

MATH_BUILTIN_1(tan)
MATH_BUILTIN_1(cos)
MATH_BUILTIN_1(sin)

MATH_BUILTIN_1(tanh)
MATH_BUILTIN_1(cosh)
MATH_BUILTIN_1(sinh)

MATH_BUILTIN_1(atan)
MATH_BUILTIN_1(acos)
MATH_BUILTIN_1(asin)

MATH_BUILTIN_1(log2)
MATH_BUILTIN_1(log)
MATH_BUILTIN_1(log10)
MATH_BUILTIN_1(exp)
MATH_BUILTIN_1(exp2)

MATH_BUILTIN_1(sqrt)

MATH_BUILTIN_1(ceil)
MATH_BUILTIN_1(floor)
MATH_BUILTIN_1(rint)

extern double round(double x);

#ifdef __cplusplus
}
#endif

