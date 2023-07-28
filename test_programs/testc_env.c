#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
    // Loop through each environment variable using 'environ' pointer
    char** env = environ;
    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }

    return 0;
}

