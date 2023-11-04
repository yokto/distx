#include <stdio.h>
#include <threads.h>

cnd_t condition;
mtx_t mutex;
int shared_variable = 0;

int thread_func(void *arg) {
    thrd_sleep(&(struct timespec){ .tv_sec = 2 }, NULL); // Introduce a delay

    mtx_lock(&mutex);
    shared_variable = 1;
    cnd_signal(&condition);
    mtx_unlock(&mutex);

    return 0;
}

int main() {
    mtx_init(&mutex, mtx_plain);
    cnd_init(&condition);

    thrd_t thread;
    thrd_create(&thread, thread_func, NULL);

    mtx_lock(&mutex);
    while (shared_variable == 0) {
        cnd_wait(&condition, &mutex);
    }
    mtx_unlock(&mutex);

    thrd_join(thread, NULL);

    cnd_destroy(&condition);
    mtx_destroy(&mutex);

    return 0;
}

