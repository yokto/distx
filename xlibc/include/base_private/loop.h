#pragma once

#include <base/types.h>

typedef uintptr_t base_loop;
typedef uintptr_t aio_context_t;

typedef struct linux_base_loop {
    aio_context_t ctx_t;
    uintptr_t io_cnt;
    uintptr_t epfd;
    uintptr_t ep_cnt;
} linux_base_loop;

inline linux_base_loop* linux_from_loop(base_loop loop) {
    return (linux_base_loop*)loop;
}

inline base_loop linux_to_loop(linux_base_loop *loop) {
    return (base_loop)loop;
}