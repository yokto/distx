/*
 * this is mostly taken form musl c
 */
#include <stdint.h> 
#include <common.h>

#define MATH_BUILTIN_1(name) \
	DLL_PUBLIC double name(double x) { return concat(____, name)(x); } \
	DLL_PUBLIC float concat(name, f)(float x) { return concat3(____, name, f)(x); } \
	DLL_PUBLIC long double concat(name, l)(long double x) { return concat3(____, name, l)(x); }


DLL_PUBLIC double modf(double value, double *iptr) {
    if (value >= 0) {
        *iptr = (double)(long long)value;  // Extract integer part for positive value
        return value - *iptr;              // Fractional part is the difference
    } else {
        *iptr = (double)(long long)(value - 1);  // Extract integer part for negative value
        return value - *iptr + 1.0;              // Fractional part is the difference + 1
    }
}
DLL_PUBLIC float modff(float x, float *iptr) { return ____modff(x, iptr); }
DLL_PUBLIC long double modfl(long double x, long double *iptr) { return ____modfl(x, iptr); }

DLL_PUBLIC double atan2(double y, double x) { return ____atan2(y, x); }
DLL_PUBLIC float atan2f(float y, float x) { return ____atan2f(y, x); }
DLL_PUBLIC long double atan2l(long double y, long double x) { return ____atan2l(y, x); }

DLL_PUBLIC double ldexp(double x, int exp) { return ____ldexp(x, exp); };
DLL_PUBLIC float ldexpf(float x, int exp) { return ____ldexpf(x, exp); };
DLL_PUBLIC long double ldexpl(long double x, int exp) { return ____ldexpl(x, exp); };

DLL_PUBLIC double round(double x) { return ____round(x); }

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

