#include <base/loop.h>
#include <stdlib.h>
#include <base/fs.h>
#include <stdio.h>

void on_write(uint64_t written, void* data) {
    printf("written %ld %p\n", written, data);
}

int main() {
    int32_t error = 0;

    base_loop loop;
    error = base_loop_create(&loop);
    if (error != 0) { exit(-1); }

    uintptr_t fd = -1;
    error = base_fs_open("test.txt", &fd, BASE_FS_OPEN_WRITE);
    if (error != 0) { exit(-1); }

    char * hello = "hello world\n";
    base_fs_write_async(loop, on_write, (void*)1, fd, hello, 12, 0, BASE_FS_WRITE_END);
    base_fs_write_async(loop, on_write, (void*)2, fd, hello, 12, 0, BASE_FS_WRITE_END);
    base_fs_write_async(loop, on_write, (void*)3, fd, hello, 12, 0, BASE_FS_WRITE_END);

    base_loop_run(loop);
}