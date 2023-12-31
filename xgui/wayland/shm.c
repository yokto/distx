#include <stdlib.h>
#include <unistd.h>
#include <zwolf.h>

#define LINUX_O_RDWR 02
#define LINUX_O_CREAT 0100
#define LINUX_O_EXCL 0200
#define LINUX_EEXIST 17
#define LINUX_EINTR 4
#define LINUX_MAP_FAILED ((void*)-1)
#define LINUX_PROT_READ 0x1
#define LINUX_PROT_WRITE 0x2
#define LINUX_MAP_SHARED 0x01

typedef unsigned int LINUX_mode_t;
int (*shm_open)(const char *__name, int __oflag, LINUX_mode_t __mode);
int (*shm_unlink)(const char *__name);
void *(*mmap)(void *__addr, size_t __len, int __prot, int __flags, int __fd, uint64_t __offset);
int (*munmap)(void *__addr, size_t __len);



int init_shm(void* libc) {
    shm_open = zwolf_sym(libc, "shm_open");
    shm_unlink = zwolf_sym(libc, "shm_unlink");
    mmap = zwolf_sym(libc, "mmap");
    munmap = zwolf_sym(libc, "munmap");
    return 0;
}

uint32_t* shm_map(size_t size, uintptr_t fd) {
    uint32_t *data = mmap(NULL, size,
            LINUX_PROT_READ | LINUX_PROT_WRITE, LINUX_MAP_SHARED, fd, 0);
    if (data == LINUX_MAP_FAILED) {
        return NULL;
    }
    return data;
}

void shm_unmap(uint32_t* data, size_t size) {
    munmap(data, size);
}

int create_shm_file(void)
{
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        for (int i = 0; i<7; i++) {
            name[sizeof(name) - 7 + i] = rand() % 26 + 'a';
        }
        --retries;
        int fd = shm_open(name, LINUX_O_RDWR | LINUX_O_CREAT | LINUX_O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && zwolf_errno() == LINUX_EEXIST);
    return -1;
}

int allocate_shm_file(size_t size)
{
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && zwolf_errno() == LINUX_EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}