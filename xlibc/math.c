#include <stdint.h> 
#include <common.h>

#define MATH_BUILTIN_1(name) \
	DLL_PUBLIC double name(double x) { return concat(__builtin_, name)(x); } \
	DLL_PUBLIC float concat(name, f)(float x) { return concat3(__builtin_, name, f)(x); } \
	DLL_PUBLIC long double concat(name, l)(long double x) { return concat3(__builtin_, name, l)(x); }

DLL_PUBLIC double modf(double x, double *iptr) { return __builtin_modf(x, iptr); }
DLL_PUBLIC float modff(float x, float *iptr) { return __builtin_modff(x, iptr); }
DLL_PUBLIC long double modfl(long double x, long double *iptr) { return __builtin_modfl(x, iptr); }

DLL_PUBLIC double atan2(double y, double x) { return __builtin_atan2(y, x); }
DLL_PUBLIC float atan2f(float y, float x) { return __builtin_atan2f(y, x); }
DLL_PUBLIC long double atan2l(long double y, long double x) { return __builtin_atan2l(y, x); }

DLL_PUBLIC double ldexp(double x, int exp) { return __builtin_ldexp(x, exp); };
DLL_PUBLIC float ldexpf(float x, int exp) { return __builtin_ldexpf(x, exp); };
DLL_PUBLIC long double ldexpl(long double x, int exp) { return __builtin_ldexpl(x, exp); };

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

DLL_PUBLIC double round(double x) { return __builtin_round(x); }

