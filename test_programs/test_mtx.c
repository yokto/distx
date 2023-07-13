#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

int main() {
    mtx_t mtx;
    int result;

    // Testing mtx_init
    if ((result = mtx_init(&mtx, mtx_plain)) != thrd_success) {
        printf("Mutex initialization failed with error code: %d\n", result);
        return -1;
    }
    printf("Mutex initialized successfully.\n");

    // Testing mtx_lock and mtx_unlock
    if ((result = mtx_lock(&mtx)) != thrd_success) {
        printf("Mutex lock failed with error code: %d\n", result);
        mtx_destroy(&mtx);
        return -1;
    }
    printf("Mutex locked.\n");

    if ((result = mtx_unlock(&mtx)) != thrd_success) {
        printf("Mutex unlock failed with error code: %d\n", result);
        mtx_destroy(&mtx);
        return -1;
    }

    // Testing mtx_trylock
    if ((result = mtx_trylock(&mtx)) == thrd_success) {
        printf("Mutex locked with trylock.\n");
        mtx_unlock(&mtx);
    } else {
        printf("trylock failed. Mutex is already locked.\n");
    }

    mtx_destroy(&mtx);
    return 0;
}
