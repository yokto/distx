#pragma once

#include <stdint.h>

struct timespec;
typedef uintptr_t aio_context_t;

enum {
    IOCB_CMD_PREAD = 0,
    IOCB_CMD_PWRITE = 1,
    IOCB_CMD_FSYNC = 2,
    IOCB_CMD_FDSYNC = 3,
    IOCB_CMD_POLL = 5,
    IOCB_CMD_NOOP = 6,
    IOCB_CMD_PREADV = 7,
    IOCB_CMD_PWRITEV = 8,
} lio_opcode;

struct iocb
{
    uint64_t aio_data;
    uint32_t aio_key;
    uint32_t aio_rw_flags;
    uint16_t aio_lio_opcode;
    int16_t aio_reqprio;
    uint32_t aio_fildes;
    uint64_t aio_buf;
    uint64_t aio_nbytes;
    uint64_t aio_offset;
    uint64_t aio_reserved2;
    uint32_t aio_flags;
    uint32_t aio_resfd;
};

struct io_event {
    uint64_t data;
    uint64_t obj;
    int64_t res;
    int64_t res2;
};

long io_setup(unsigned int nr_events, aio_context_t *ctx_idp);
long io_submit(aio_context_t ctx_id, long nr, struct iocb **iocbpp);
long io_getevents(aio_context_t ctx_id, 
                   long min_nr, long nr, struct io_event *events,
                   struct timespec *timeout);
