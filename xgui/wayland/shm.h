#pragma once

int init_shm(void* libc);
int create_shm_file(void);
int allocate_shm_file(size_t size);
uint32_t* shm_map(size_t size, uintptr_t fd);
void shm_unmap(uint32_t* data, size_t size);