// code from here
// http://locklessinc.com/articles/mutex_cv_futex/
// https://www.remlab.net/op/futex-misc.shtml
// try this for mac
// https://github.com/m-ou-se/atomic-wait/blob/main/src/macos.rs
#include "base/futex.h"
#include "base/futex_p.h"
#include "common.h"
#include <zwolf.h>
#include <errno.h>
#include "threads.h"
#include <stdlib.h>
#include <stdbool.h>

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

void base_futex_p_init(bool iswin, void* lib) {
	isWin = iswin;
	if (isWin) {
		void* sync = zwolf_open("api-ms-win-core-synch-l1-2-0.dll");
		WaitOnAddress = zwolf_sym(sync, "WaitOnAddress");
		WakeByAddressAll = zwolf_sym(sync, "WakeByAddressAll");
		WakeByAddressSingle = zwolf_sym(sync, "WakeByAddressSingle");
	} else {
		syscall = zwolf_sym(lib, "syscall");
	}
}

DLL_PUBLIC
int32_t base_futex_wait(_Atomic uint32_t *addr, uint32_t val, const struct timespec *to)
{
	debug_printf("futex_wait\n");
	if (isWin) {
		if (to == NULL)
		{
			WaitOnAddress((volatile void *)addr, &val, sizeof (val),
					-1);
			debug_printf("futex_wait done\n");
			return 0;
		}

		if (to->tv_nsec >= 1000000000)
		{
			errno = EINVAL;
			debug_printf("futex_wait done\n");
			return -1;
		}

		if (to->tv_sec >= 2147)
		{
			WaitOnAddress((volatile void *)addr, &val, sizeof (val),
					2147000000);
			debug_printf("futex_wait done\n");
			return 0; /* time-out out of range, claim spurious wake-up */
		}

		uint32_t ms = (to->tv_sec * 1000000)
			+ ((to->tv_nsec + 999) / 1000);

		if (!WaitOnAddress((volatile void *)addr, &val,
					sizeof (val), ms))
		{
			errno = ETIMEDOUT;
			debug_printf("futex_wait done\n");
			return -1;
		}
		return 0;
	} else {
            int32_t ret = syscall(SYS_futex, (uint32_t*)addr, FUTEX_WAIT_PRIVATE, val, to, NULL, 0);
	    debug_printf("futex_wait done\n");
	    return ret;
	}
}

DLL_PUBLIC
int base_futex_signal(_Atomic uint32_t *addr)
{
	debug_printf("futex_signal\n");
	if (isWin) {
	debug_printf("futex_signal %p %p \n", WakeByAddressSingle, addr);
		WakeByAddressSingle(addr);
	debug_printf("futex_signal done\n");
		return 0;
	} else {
		return (syscall(SYS_futex, (uint32_t*)addr, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0) >= 1) ? 0 : 1;
	}
}

DLL_PUBLIC
int base_futex_broadcast(_Atomic uint32_t *addr)
{
	debug_printf("futex_broadcast\n");
	if (isWin) {
		WakeByAddressAll(addr);
		return 0;
	} else {
		return (syscall(SYS_futex, (uint32_t*)addr, FUTEX_WAKE_PRIVATE, (uint32_t)(-1), NULL, NULL, 0) >= 1) ? 0 : 1;
	}
}
