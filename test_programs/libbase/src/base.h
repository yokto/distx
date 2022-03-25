#ifndef BASE_H
#define BASE_H

#include "base-types.h"

#define BASE_O_READ 1
#define BASE_O_WRITE 2

int32_t base_open(const char *pathname, int32_t flags);
ssize_t base_write(int32_t fd, const void *buf, size_t count);
int32_t base_isatty(int32_t fd);

void* base_aligned_alloc(size_t alignment, size_t size);
void* base_realloc(void *ptr, size_t size);
void* base_calloc(size_t elem, size_t size);
void base_free(void* ptr);

size_t base_memory_granularity(void);

#endif // BASE_H
