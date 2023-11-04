#include <stdio.h>
#include <threads.h>
#include <time.h>
#include <stdatomic.h>

int futex_wait(_Atomic uint32_t *addr, uint32_t val, const struct timespec *to);
int futex_signal(_Atomic uint32_t *addr);
int futex_broadcast(_Atomic uint32_t *addr);

_Atomic uint32_t shared_variable = 0;
mtx_t mutex;
cnd_t condition;

int thread_func(void *arg) {
    int id = *(int *)arg;
    struct timespec timeout = { .tv_sec = 5, .tv_nsec = 0 };

    // Threads 1 and 2 wait for shared_variable to become 1
    if (id == 1 || id == 2) {
        futex_wait((_Atomic uint32_t *)&shared_variable, 0, &timeout);
        printf("Thread %d: Shared variable is now 1.\n", id);
    } else {
        thrd_sleep(&(struct timespec){ .tv_sec = 2 }, NULL); // Introduce a delay

        shared_variable = 1;
        futex_signal(&shared_variable);

        printf("Thread %d: Changed shared variable to 1 and signaled.\n", id);
    }

    return 0;
}

int main() {
    thrd_t threads[3];
    int thread_ids[3] = { 1, 2, 3 };

    for (int i = 0; i < 3; ++i) {
        thrd_create(&threads[i], thread_func, &thread_ids[i]);
    }

    for (int i = 0; i < 3; ++i) {
        thrd_join(threads[i], NULL);
    }

    return 0;
}
