#pragma once

#include <stdbool.h>
#include <base/types.h>
#include <stdatomic.h>
#include <time.h>

void init_threads(bool iswin, void * lib);

int futex_wait(_Atomic uint32_t *addr, uint32_t val, const struct timespec *to);
int futex_signal(_Atomic uint32_t *addr);
int futex_broadcast(_Atomic uint32_t *addr);
