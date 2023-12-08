//int main() { return 0; }
#include <stdio.h>
#include <setjmp.h>

// Global jump buffer
jmp_buf jump_buffer;

// Function that uses setjmp
int perform_operation(int value) {
    // Save the current context
    int result = setjmp(jump_buffer);

    if (result == 0) {
        // Attempt the operation that might fail
        if (value < 0) {
            // Simulate an error condition
            printf("Error: Negative value not allowed.\n");
            // Jump back to the saved context with a non-zero value
            longjmp(jump_buffer, 1);
        }

        // Successful operation
        printf("Success: Value is %d.\n", value);
        return 0;
    } else {
        // Handle the error condition
        printf("Error handling: Recovered from error with code %d.\n", result);
        return result;
    }
}

int main() {
    // Test with positive value
    int result1 = perform_operation(42);

    // Test with negative value (will trigger an error)
    int result2 = perform_operation(-10);

    // Print the overall result
    printf("Overall result: %d\n", result1 | result2);

    return (result1 | result2) == 1;
}

