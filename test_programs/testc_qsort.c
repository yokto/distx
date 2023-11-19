#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Comparison function for qsort to sort strings in ascending order
int compareStrings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// Test function for qsort with strings
void testQSortStrings() {
    char *strings[] = {"banana", "apple", "orange", "grape", "cherry", "bannn", "baaa", "002"};

    size_t stringsCount = sizeof(strings) / sizeof(strings[0]);

    // Display the original strings
    printf("Original strings:\n");
    for (size_t i = 0; i < stringsCount; i++) {
        printf("%s\n", strings[i]);
    }
    printf("\n");

    // Sort the strings using qsort
    qsort(strings, stringsCount, sizeof(char *), compareStrings);

    // Display the sorted strings
    printf("Sorted strings:\n");
    for (size_t i = 0; i < stringsCount; i++) {
        printf("%s\n", strings[i]);
    }
}

int main() {
    printf("Testing qsort function with strings:\n");
    testQSortStrings();
    return 0;
}

