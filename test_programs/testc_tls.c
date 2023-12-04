#include <stdio.h>
#include <threads.h>

// Define a thread-local variable
_Thread_local int threadLocalVariable;


int thread_function(void* arg) {
    // Access and modify the thread-local variable
    threadLocalVariable = *(int*)arg;
    threadLocalVariable += 10;

    printf("Thread-local variable value: %d\n", threadLocalVariable);
    return 0;
}

int main() {
    thrd_t thread1, thread2;
    int arg1 = 5, arg2 = 10;

    // Create two threads
    if (thrd_create(&thread1, thread_function, &arg1) != thrd_success ||
        thrd_create(&thread2, thread_function, &arg2) != thrd_success) {
        fprintf(stderr, "Failed to create threads\n");
        return 1;
    }

    // Wait for the threads to complete
    if (thrd_join(thread1, NULL) != thrd_success ||
        thrd_join(thread2, NULL) != thrd_success) {
        fprintf(stderr, "Failed to join threads\n");
        return 1;
    }

    // Access the thread-local variable in the main thread
    printf("Thread-local variable value in the main thread: %d\n", threadLocalVariable);

    return 0;
}

