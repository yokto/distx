#include <stdio.h>
#include <threads.h>

#define NUM_THREADS 8
#define ITERATIONS 1000000

mtx_t mutex;
int shared_counter = 0;

int worker(void *arg) {
    for (int i = 0; i < ITERATIONS; ++i) {
        mtx_lock(&mutex);
        ++shared_counter;
        mtx_unlock(&mutex);
    }
    return 0;
}

int main() {
    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        perror("Mutex initialization failed");
        return -1;
    }

    thrd_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (thrd_create(&threads[i], worker, NULL) != thrd_success) {
            perror("Thread creation failed");
            return -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        if (thrd_join(threads[i], NULL) != thrd_success) {
            perror("Thread join failed");
            return -1;
        }
    }

    mtx_destroy(&mutex);

    // Check if the shared counter matches the expected value
    if (shared_counter == NUM_THREADS * ITERATIONS) {
        printf("Test passed\n");
        return 0;
    } else {
        printf("Test failed\n");
        return -1;
    }
}

