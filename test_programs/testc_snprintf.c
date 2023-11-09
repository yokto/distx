#include <stdio.h>
#include <assert.h>

int test_snprintf_success() {
    char buffer[50];
    int result = snprintf(buffer, sizeof(buffer), "Hello, %s!", "World");
    return (result >= 0) ? 0 : -1;
}

int test_snprintf_failure() {
    char buffer[10];  // Small buffer intentionally set to trigger truncation
    int result = snprintf(buffer, sizeof(buffer), "Too long f");
    return (result < 0) ? 0 : -1;
}

int main() {
    // Test cases
    assert(test_snprintf_success() == 0 && "snprintf success test failed");
    assert(test_snprintf_failure() == 0 && "snprintf failure test failed");

    printf("All tests passed successfully!\n");
    return 0;
}
