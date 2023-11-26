#pragma once

#include <base/types.h>
#include <time.h>

int base_futex_wait(_Atomic uint32_t *addr, uint32_t val, const struct timespec *to);
int base_futex_signal(_Atomic uint32_t *addr);
int base_futex_broadcast(_Atomic uint32_t *addr);
