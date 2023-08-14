#ifndef MMAN_H
#define MMAN_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_PRIVATE 1
#define MAP_SHARED 2
#define MAP_FAILED ((void*)-1)

#define PROT_READ 11
#define PROT_WRITE 12
#define PROT_NONE 13
#define PROT_EXEC 14

void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
int munmap(void *addr, size_t length);

#define MADV_DONTNEED 1
int madvise(void *addr, size_t length, int advice);

int shm_open(const char *name, int oflag, int mode);
int shm_unlink(const char *name);

int mprotect(void *addr, size_t len, int prot);



#ifdef __cplusplus
}
#endif

#endif // MMAN_H
