#include <base/loop.h>
#include <linux_base/aio_abi.h>
#include <base_private/loop.h>
#include <xload.h>
#include <stdlib.h>
#include <common.h>
#include <stdbool.h>
#include <stdio.h>
#include <base/fs.h>

struct io_event_closure {
    void (*func)(struct io_event*, void* specific_function, void* data);
    void* specific_function;
    void* data;
};

static bool isWin = false;

int (*epoll_create)(int size) = 0;

int32_t base_loop_create(base_loop *loop_ptr) {
    if (isWin) {
        return -1;
    } else {
        linux_base_loop *loop = calloc(1, sizeof(linux_base_loop));
        int32_t ret = io_setup(100, &loop->ctx_t);
        loop->epfd = epoll_create(1);
        if (ret == 0 && loop->epfd >= 0)
        {
            *loop_ptr = linux_to_loop(loop);
        }
        return ret;
    }
}

int32_t base_loop_run(base_loop base_loop) {
    linux_base_loop *loop = linux_from_loop(base_loop);
    struct io_event ev;
    while (true) {
        long read = io_getevents(loop->ctx_t, 1, 1, &ev, 0);
        if (read == 1)
        {
            struct io_event_closure* closure = (struct io_event_closure *)ev.data;
            closure->func(&ev, closure->specific_function, closure->data);
            free(closure);
            printf("got %ld %ld %ld %ld\n", ev.data, ev.obj, ev.res2, ev.res);
        }
    }
}

static void general_on_write(struct io_event* event, void* specific_function, void* data) {
    void (*specific_function2)(uint64_t written, void* data) = specific_function;
    specific_function2(event->res, data);
}

int32_t base_fs_write_async(
        base_loop loop, void (*on_write)(uint64_t written, void* data), void* data,
        uintptr_t fd, const void *buf, uintptr_t count, intptr_t offset,
        int32_t flags) {
    if (isWin) {
        return -1;
    } else {
        struct linux_base_loop* lloop = linux_from_loop(loop);
        struct io_event_closure * closure = calloc(1, sizeof(struct io_event_closure));
        closure->func = general_on_write;
        closure->specific_function = on_write;
        closure->data = data;
        struct iocb cmd =
            {
                .aio_data = (uintptr_t)closure,
                .aio_key = 0,      // carefull this can be swapped
                .aio_rw_flags = flags & BASE_FS_WRITE_END ? 0x10 : 0, // carefull this can be swapped
                .aio_lio_opcode = IOCB_CMD_PWRITE,
                .aio_reqprio = 0,
                .aio_fildes = fd,
                .aio_buf = (uintptr_t)buf,
                .aio_nbytes = count,
                .aio_offset = offset,
                .aio_reserved2 = 0,
                .aio_flags = 0,
                .aio_resfd = 0};
        struct iocb *cmdp = &cmd;
        io_submit(lloop->ctx_t, 1, &cmdp);
        return 0;
    }
}

#define SYS_io_setup 206
#define SYS_io_submit 209
#define SYS_io_cancel 210
#define SYS_io_destroy 207
#define SYS_io_getevents 208

static long (*syscall)(long syscall, ...);
long io_setup(unsigned int nr_events, aio_context_t *ctx_idp) { return syscall(SYS_io_setup, nr_events, ctx_idp); }
long io_submit(aio_context_t ctx_id, long nr, struct iocb **iocbpp) { return syscall(SYS_io_submit, ctx_id, nr, iocbpp); }
long io_getevents(aio_context_t ctx_id, long min_nr, long nr, struct io_event *events, struct timespec *timeout) {
    return syscall(SYS_io_getevents, ctx_id, min_nr, nr, events, timeout);
}

void base_loop_p_init(bool isWin2, void * lib) {
    
    isWin = isWin2;
    if (isWin) {
    } else {
        syscall = xload_sym(lib, "syscall");
        epoll_create = xload_sym(lib, "epoll_create");
    }
}
