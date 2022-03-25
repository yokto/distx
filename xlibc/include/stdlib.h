#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>

void free(void* ptr);
void* malloc(size_t new_size);

#endif // STDLIB_H
