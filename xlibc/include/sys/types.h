#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t blkcnt_t;
typedef uint64_t blksize_t;
typedef uint64_t clock_t;
typedef int clockid_t;
typedef uint64_t dev_t;
typedef uint64_t fsblkcnt_t;
typedef uint64_t fsfilcnt_t;
typedef uint64_t gid_t;
typedef uint64_t id_t;
typedef uint64_t ino_t;
typedef uint64_t key_t;
typedef uint64_t mode_t;
typedef uint64_t nlink_t;
typedef int64_t off_t;
typedef int32_t pid_t;
typedef uint64_t pthread_attr_t;
typedef uint64_t pthread_barrier_t;
typedef uint64_t pthread_barrierattr_t;
typedef uint64_t pthread_cond_t;
typedef uint64_t pthread_condattr_t;
typedef uint64_t pthread_key_t;
typedef uint64_t pthread_mutex_t;
typedef uint64_t pthread_mutexattr_t;
typedef uint64_t pthread_once_t;
typedef uint64_t pthread_rwlock_t;
typedef uint64_t pthread_rwlockattr_t;
typedef uint64_t pthread_spinlock_t;
typedef uint64_t pthread_t;
typedef uint64_t suseconds_t;
typedef int64_t time_t;
typedef uint64_t timer_t;
typedef uint64_t trace_attr_t;
typedef uint64_t trace_event_id_t;
typedef uint64_t trace_event_set_t;
typedef uint64_t trace_id_t;
typedef uint64_t uid_t;
typedef uint64_t useconds_t;

typedef uint64_t fsid_t;

struct passwd {
	char * pw_dir;
};

typedef size_t socklen_t;

#ifdef __cplusplus
}
#endif
#endif // SYS_TYPES_H
