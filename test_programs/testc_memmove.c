#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv) {
    // Source and overlapping destination buffers
    char buffer[] = "Hello, Memmove!       ";

    // Print initial content
    printf("Initial Content: %s\n", buffer);

    // Perform memmove with overlapping regions
    buffer[20] = 0xee;
    memmove(buffer + 7, buffer, 14 + (int)(argc != 0));

    // Check the result
    printf("After Memmove:   %s\n", buffer);

    // Check if memmove worked correctly
    if (strcmp(buffer, "Hello, Hello, Memmove!") == 0) {
        printf("Memmove successful. Overlapping regions handled.\n");
        return 0;  // Success
    } else {
        printf("Memmove failed. Overlapping regions not handled correctly.\n");
        return 1;  // Failure
    }
}

