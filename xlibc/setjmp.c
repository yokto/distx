//===-- Implementation of setjmp ------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define __x86_64__ 1
#include "setjmp.h"
#include "common.h"

#define LIBC_INLINE_ASM __asm__

DLL_PUBLIC
int setjmp(__jmp_buf * buf) {
#ifdef __x86_64
  register __UINT64_TYPE__ rbx __asm__("rbx");
  register __UINT64_TYPE__ r12 __asm__("r12");
  register __UINT64_TYPE__ r13 __asm__("r13");
  register __UINT64_TYPE__ r14 __asm__("r14");
  register __UINT64_TYPE__ r15 __asm__("r15");

  // We want to store the register values as is. So, we will suppress the
  // compiler warnings about the uninitialized variables declared above.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuninitialized"
  LIBC_INLINE_ASM("mov %1, %0\n\t" : "=m"(buf->rbx) : "r"(rbx) :);
  LIBC_INLINE_ASM("mov %1, %0\n\t" : "=m"(buf->r12) : "r"(r12) :);
  LIBC_INLINE_ASM("mov %1, %0\n\t" : "=m"(buf->r13) : "r"(r13) :);
  LIBC_INLINE_ASM("mov %1, %0\n\t" : "=m"(buf->r14) : "r"(r14) :);
  LIBC_INLINE_ASM("mov %1, %0\n\t" : "=m"(buf->r15) : "r"(r15) :);
#pragma GCC diagnostic pop

  // We want the rbp of the caller, which is what __builtin_frame_address(1)
  // should return. But, compilers generate a warning that calling
  // __builtin_frame_address with non-zero argument is unsafe. So, we use
  // the knowledge of the x86_64 ABI to fetch the callers rbp. As per the ABI,
  // the rbp of the caller is pushed on to the stack and then new top is saved
  // in this function's rbp. So, we fetch it from location at which this
  // functions's rbp is pointing.
  buf->rbp = *(__UINTPTR_TYPE__ *)(__builtin_frame_address(0));

  // The callers stack address is exactly 2 pointer widths ahead of the current
  // frame pointer - between the current frame pointer and the rsp of the caller
  // are the return address (pushed by the x86_64 call instruction) and the
  // previous stack pointer as required by the x86_64 ABI.
  // The stack pointer is ahead because the stack grows down on x86_64.
  buf->rsp = (__UINTPTR_TYPE__)(__builtin_frame_address(0)) +
             sizeof(__UINTPTR_TYPE__) * 2;
  buf->rip = (__UINTPTR_TYPE__)(__builtin_return_address(0));
#else // LLVM_LIBC_ARCH_X86_64
  debug_printf("setjmp not implemented");
  __builtin_trap();
#endif

  return 0;
}
