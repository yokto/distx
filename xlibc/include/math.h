#pragma once
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

#ifdef __cplusplus
}
#endif
