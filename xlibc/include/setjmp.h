#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
#ifdef __x86_64__
  __UINT64_TYPE__ rbx;
  __UINT64_TYPE__ rbp;
  __UINT64_TYPE__ r12;
  __UINT64_TYPE__ r13;
  __UINT64_TYPE__ r14;
  __UINT64_TYPE__ r15;
  __UINTPTR_TYPE__ rsp;
  __UINTPTR_TYPE__ rip;
#else
  int rbx;
#endif
} __jmp_buf;

typedef __jmp_buf jmp_buf[1];
int setjmp(jmp_buf buf);
void longjmp(jmp_buf env, int val);


#ifdef __cplusplus
}
#endif
