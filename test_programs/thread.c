#include <stdio.h>
#include <threads.h>

int square(int n) {
    return n * n;
}

int thread_function(void* arg) {
    int* num = (int*)arg;
    printf("Thread function executing with argument: %d\n", *num);
    int result = square(*num);
    return result;
}

int main() {
    thrd_t thread;
    int arg = 5;
    int result;

    if (thrd_create(&thread, thread_function, &arg) != thrd_success) {
        fprintf(stderr, "Failed to create thread\n");
        return 1;
    }

    if (thrd_join(thread, &result) != thrd_success) {
        fprintf(stderr, "Failed to join thread\n");
        return 1;
    }

    printf("Thread returned: %d\n", result);

    return 0;
}

