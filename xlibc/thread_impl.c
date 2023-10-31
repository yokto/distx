// code from here
// http://locklessinc.com/articles/mutex_cv_futex/
// https://www.remlab.net/op/futex-misc.shtml
// try this for mac
// https://github.com/m-ou-se/atomic-wait/blob/main/src/macos.rs
#include "thread_impl.h"
#include "common.h"
#include <errno.h>
#include "threads.h"
#include <stdlib.h>

static bool (*WaitOnAddress)(volatile void *Address, void* CompareAddress, size_t AddressSize, uint32_t dwMilliseconds);
static void (*WakeByAddressSingle)(void* Address);
static void (*WakeByAddressAll)(void* Address);

#define SYS_futex       202
#define FUTEX_WAIT_PRIVATE      128
#define FUTEX_WAKE_PRIVATE      129
static long (*syscall)(uint32_t syscall, uint32_t *uaddr, int futex_op, uint32_t val,
                    const struct timespec *timeout,   /* or: uint32_t val2 */
                    uint32_t *uaddr2, uint32_t val3);

static bool isWin = false;

void init_threads(bool iswin, void* lib) {
	isWin = iswin;
	if (isWin) {
		void* sync = __dlopen("api-ms-win-core-synch-l1-2-0.dll");
		WaitOnAddress = __dlsym(sync, "WaitOnAddress");
		WakeByAddressAll = __dlsym(sync, "WakeByAddressAll");
		WakeByAddressSingle = __dlsym(sync, "WakeByAddressSingle");
	} else {
		syscall = __dlsym(lib, "syscall");
	}
}

DLL_PUBLIC
int32_t futex_wait(_Atomic uint32_t *addr, uint32_t val, const struct timespec *to)
{
	__debug_printf("futex_wait\n");
	if (isWin) {
		if (to == NULL)
		{
			WaitOnAddress((volatile void *)addr, &val, sizeof (val),
					-1);
			__debug_printf("futex_wait done\n");
			return 0;
		}

		if (to->tv_nsec >= 1000000000)
		{
			errno = EINVAL;
			__debug_printf("futex_wait done\n");
			return -1;
		}

		if (to->tv_sec >= 2147)
		{
			WaitOnAddress((volatile void *)addr, &val, sizeof (val),
					2147000000);
			__debug_printf("futex_wait done\n");
			return 0; /* time-out out of range, claim spurious wake-up */
		}

		uint32_t ms = (to->tv_sec * 1000000)
			+ ((to->tv_nsec + 999) / 1000);

		if (!WaitOnAddress((volatile void *)addr, &val,
					sizeof (val), ms))
		{
			errno = ETIMEDOUT;
			__debug_printf("futex_wait done\n");
			return -1;
		}
		return 0;
	} else {
            int32_t ret = syscall(SYS_futex, addr, FUTEX_WAIT_PRIVATE, val, to, NULL, 0);
	    __debug_printf("futex_wait done\n");
	    return ret;
	}
}

DLL_PUBLIC
int futex_signal(_Atomic uint32_t *addr)
{
	__debug_printf("futex_signal\n");
	if (isWin) {
	__debug_printf("futex_signal %p %p \n", WakeByAddressSingle, addr);
		WakeByAddressSingle(addr);
	__debug_printf("futex_signal done\n");
		return 0;
	} else {
		return (syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0) >= 1) ? 0 : 1;
	}
}

DLL_PUBLIC
int futex_broadcast(_Atomic uint32_t *addr)
{
	__debug_printf("futex_broadcast\n");
	if (isWin) {
		WakeByAddressAll(addr);
		return 0;
	} else {
		return (syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, (uint32_t)(-1), NULL, NULL, 0) >= 1) ? 0 : 1;
	}
}

#define MUTEX_INITIALIZER {0}

DLL_PUBLIC
int mtx_init(mtx_t *m, int type)
{
	__debug_printf("mtx_init\n");
	*m = 0;
	return 0;
}

DLL_PUBLIC
void mtx_destroy(mtx_t *m)
{
	__debug_printf("mtx_destroy\n");
	/* Do nothing */
	(void) m;
}

DLL_PUBLIC
int mtx_lock(mtx_t *m)
{
	//__debug_printf("mtx_lock\n");
	int i;
	uint32_t c;
	
	/* Spin and try to take lock */
	for (i = 0; i < 100; i++)
	{
		c = 0;
		__c11_atomic_compare_exchange_strong(m, &c, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
		if (!c) return 0;
		
		//cpu_relax();
	}

	/* The lock is now contended */
	if (c == 1) c = __c11_atomic_exchange(m, 2, __ATOMIC_SEQ_CST);

	while (c)
	{
		/* Wait in the kernel */
		futex_wait(m, 2, NULL);
		c = __c11_atomic_exchange(m, 2, __ATOMIC_SEQ_CST);
	}
	
	return 0;
}

DLL_PUBLIC
int mtx_unlock(mtx_t *m)
{
	//__debug_printf("mtx_unlock %d\n" , *m);
	int i;
	
	/* Unlock, and if not contended then exit. */
	if (*m == 2)
	{
		*m = 0;
	}
	else if (__c11_atomic_exchange(m, 0, __ATOMIC_SEQ_CST) == 1) {
		return 0;
	}

	/* Spin and hope someone takes the lock */
	for (i = 0; i < 200; i++)
	{
		if (*m)
		{
			/* Need to set to state 2 because there may be waiters */
			uint32_t expected = 1;
			__c11_atomic_compare_exchange_strong(m, &expected, 2,  __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
			if (expected) return 0;
		}
		//cpu_relax();
	}
	
	/* We need to wake someone up */
	futex_signal(m);
	
	return 0;
}

DLL_PUBLIC
int mtx_trylock(mtx_t *m)
{
	__debug_printf("mtx_trylock\n");
	/* Try to take the lock, if is currently unlocked */
	uint32_t c = 0;
	__c11_atomic_compare_exchange_strong(m, &c, 1, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	if (!c) return 0;
	return EBUSY;
}


#define PTHREAD_COND_INITIALIZER {NULL, 0, 0}

DLL_PUBLIC
int cnd_init(cnd_t *c)
{
	__debug_printf("cnd_init\n");
	c->m = (void*)NULL;

	/* Sequence variable doesn't actually matter, but keep valgrind happy */
	c->seq = 0;
	
	return 0;
}

DLL_PUBLIC
void cnd_destroy(cnd_t *c)
{
	__debug_printf("cnd_destroy\n");
	/* No need to do anything */
	(void) c;
}

DLL_PUBLIC
int cnd_signal(cnd_t *c)
{
	__debug_printf("cnd_signal\n");
	/* We are waking someone up */
	__c11_atomic_fetch_add(&c->seq, 1, __ATOMIC_SEQ_CST);
	
	/* Wake up a thread */
	futex_signal(&c->seq);
	
	return 0;
}

DLL_PUBLIC
int cnd_broadcast(cnd_t *c)
{
	__debug_printf("cnd_broadcast\n");
	mtx_t *m = c->m;
	
	/* No mutex means that there are no waiters */
	if (!m) return 0;
	
	/* We are waking everyone up */
	__c11_atomic_fetch_add(&c->seq, 1, __ATOMIC_SEQ_CST);
	
	/* Wake one thread, and requeue the rest on the mutex */
	futex_broadcast(&c->seq);
	
	return 0;
}

DLL_PUBLIC
int cnd_wait(cnd_t *c, mtx_t *m)
{
	__debug_printf("cnd_wait\n");
	int seq = c->seq;

	if (c->m != m)
	{
		/* Atomically set mutex inside cnd_t */
		mtx_t *expected = NULL;
		__c11_atomic_compare_exchange_strong(&c->m, &expected, m, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
		if (c->m != m) return EINVAL;
	}
	
	mtx_unlock(m);
	
	futex_wait(&c->seq, seq, NULL);
	
	while (__c11_atomic_exchange(m, 2, __ATOMIC_SEQ_CST))
	{
		futex_wait(m, 2, NULL);
	}
		
	return 0;
}

DLL_PUBLIC
int cnd_timedwait(cnd_t* c, mtx_t* m, const struct timespec* time_point )
{
	__debug_printf("cnd_timedwait\n");
	int seq = c->seq;

	if (c->m != m)
	{
		/* Atomically set mutex inside cv */
		mtx_t *expected = NULL;
		__c11_atomic_compare_exchange_strong(&c->m, &expected, m, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
		if (c->m != m) return EINVAL;
	}
	
	mtx_unlock(m);
	
	futex_wait(&c->seq, seq, time_point);
	
	if (__c11_atomic_exchange(m, 2, __ATOMIC_SEQ_CST)) {
		return thrd_timedout;
	}
		
	return 0;
}
