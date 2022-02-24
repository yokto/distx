#include <types.h>

#define BASE_O_READ 1
#define BASE_O_WRITE 2

int32_t base_open(const char *pathname, int32_t flags);
ssize_t write(int32_t fd, const void *buf, size_t count);
