#ifndef THREAD_H
#define THREAD_H
#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include <stddef.h>


// Thread
typedef size_t thrd_t;
typedef int(*thrd_start_t)(void*);
int thrd_create( thrd_t *thr, thrd_start_t func, void *arg );
int thrd_equal( thrd_t lhs, thrd_t rhs );
thrd_t thrd_current(void);
int thrd_sleep( const struct timespec* duration,
                struct timespec* remaining );
void thrd_yield(void);
_Noreturn void thrd_exit( int res );
int thrd_detach( thrd_t thr );
int thrd_join( thrd_t thr, int *res );
enum {
    thrd_success = 0 /* unspecified */,
    thrd_nomem = 3 /* unspecified */,
    thrd_timedout = 4 /* unspecified */,
    thrd_busy = 1 /* unspecified */,
    thrd_error = 2 /* unspecified */
};

// Mutex
typedef _Atomic uint32_t mtx_t; // this is a hack because it has this size on linux. we should dynamically allocate it on mtx_init
int mtx_init( mtx_t* mutex, int type );
int mtx_lock( mtx_t* mutex );
int mtx_timedlock( mtx_t * mutex,
                   const struct timespec * time_point );
int mtx_trylock( mtx_t *mutex );
int mtx_unlock( mtx_t *mutex );
void mtx_destroy( mtx_t *mutex );
enum {
    mtx_plain = 0,
    mtx_recursive = 2,
    mtx_timed = 1
};

// call once
#define ONCE_FLAG_INIT 0
typedef uint8_t once_flag;
void call_once( once_flag* flag, void (*func)(void) );

// conditional variables
typedef struct cnd_t
{
	_Atomic (mtx_t *) m;
	_Atomic uint32_t seq;
	int pad;
} cnd_t;
int cnd_init( cnd_t* cond );
int cnd_signal( cnd_t *cond );
int cnd_broadcast( cnd_t *cond );
int cnd_wait( cnd_t* cond, mtx_t* mutex );
int cnd_timedwait( cnd_t*  cond, mtx_t*  mutex,
                   const struct timespec*  time_point );
void cnd_destroy( cnd_t* cond );

// thread local
typedef size_t tss_t;
typedef void(*tss_dtor_t)(void*);
//#define thread_local _Thread_local
#define TSS_DTOR_ITERATIONS 42
int tss_create( tss_t* tss_key, tss_dtor_t destructor );
void *tss_get( tss_t tss_key );
int tss_set( tss_t tss_id, void *val );
void tss_delete( tss_t tss_id );

#ifdef __cplusplus
}
#endif
#endif // THREAD_H
