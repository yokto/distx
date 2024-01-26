#pragma once

#include <base/types.h>

typedef uintptr_t base_loop;

int32_t base_loop_create(base_loop *loop);
void base_loop_destroy(base_loop loop);

// this method blocks and calls the callbacks.
// it returns when it has no more running jobs.
int32_t base_loop_run(base_loop loop);