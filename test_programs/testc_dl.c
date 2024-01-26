#include <stdio.h>
#include <dlfcn.h>
#include <arch.h>

int main() {
    // Dynamic library handle
    void* handle = dlopen("/test_programs/" ARCH "/lib/libclib_dl.so", RTLD_NOW);

    // Check if the library was loaded successfully
    if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    // Attempt to get the function pointer for foobar_foo
    int (*foobar_foo)() = dlsym(handle, "foobar_foo");

    // Attempt to get the address of foobar_bar
    int* foobar_bar = dlsym(handle, "foobar_bar");

    // Check if symbols are found
    if (!foobar_foo || !foobar_bar) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Call the foobar_foo function and print the result
    printf("pointer foobar_foo result: %p\n", foobar_foo);

    // Print the value of foobar_bar
    printf("pointer foobar_bar value: %p\n", foobar_bar);

    // Call the foobar_foo function and print the result
    printf("foobar_foo result: %d\n", foobar_foo());

    // Print the value of foobar_bar
    printf("foobar_bar value: %d\n", *foobar_bar);

    // Close the library handle
    dlclose(handle);

    return 0;
}

