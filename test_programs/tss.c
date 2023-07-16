#include <stdio.h>
#include <threads.h>

// Thread-specific data key
tss_t dataKey;

void destructor(void* data) {
    printf("Destructor called for thread-specific data: %p\n", data);
}

int thread_function(void* arg) {
    int threadData = *(int*)arg;

    // Set thread-specific data for each thread
    if (tss_set(dataKey, (void*)&threadData) != thrd_success) {
        fprintf(stderr, "Failed to set thread-specific data\n");
        return 1;
    }

    // Get thread-specific data for each thread
    void* retrievedData = tss_get(dataKey);
    if (retrievedData != NULL) {
        int* value = (int*)retrievedData;
        printf("Thread-specific data retrieved for thread %ld: %d\n",
               thrd_current(), *value);
    } else {
        fprintf(stderr, "Failed to get thread-specific data\n");
        return 1;
    }

    return 0;
}

int main() {
    // Create thread-specific data key
    if (tss_create(&dataKey, destructor) != thrd_success) {
        fprintf(stderr, "Failed to create thread-specific data key\n");
        return 1;
    }

    // Create multiple threads
    thrd_t thread1, thread2;
    int arg1 = 10, arg2 = 20;

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

    // Destroy the thread-specific data key
    tss_delete(dataKey);

    return 0;
}

