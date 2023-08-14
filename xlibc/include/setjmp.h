#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void* jmp_buf[20];
//static inline int setjmp (jmp_buf buf) {
//	return __builtin_setjmp (buf);
//}
#define setjmp(buf) (__builtin_setjmp(buf))
#define longjmp(env, val) (__builtin_longjmp(env, val))


#ifdef __cplusplus
}
#endif
