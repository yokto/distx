#include <base/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <common.h>

// Swap two elements of size 'size' in bytes
void swap(void* a, void* b, size_t size) {
    char* p = a, *q = b, tmp;
    for (size_t i = 0; i < size; ++i) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

// Lomuto partition scheme for qsort
void* partition(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    char* p = base;
    size_t i = 0;

    for (size_t j = 0; j < nmemb - 1; ++j) {
        if (compar(p + j * size, p + (nmemb - 1) * size) <= 0) {
            swap(p + i * size, p + j * size, size);
            ++i;
        }
    }

    swap(p + i * size, p + (nmemb - 1) * size, size);
    return p + i * size;
}

// Quicksort function
DLL_PUBLIC
void qsort(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*)) {
    if (nmemb <= 1)
        return;

    void* pivot = partition(base, nmemb, size, compar);
    size_t pivot_idx = (pivot - base) / size;

    qsort(base, pivot_idx, size, compar);
    qsort(pivot + size, nmemb - pivot_idx - 1, size, compar);
}

// Compare function for integers (ascending order)
int compare_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}
