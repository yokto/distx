#include <base/fs.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    const char* filename = "/__zwolf_basedir__/test_programs/example.txt";
    int32_t error = 0;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;

    // Open the file for reading and writing
    uintptr_t fd;

    error = base_fs_open(filename, &fd, BASE_FS_OPEN_READ | BASE_FS_OPEN_WRITE | BASE_FS_OPEN_CREATE);
    if (error != SUCCESS) {
        fprintf(stderr, "Error opening the file %d %d", error, fd);
        return 1;
    }
    error = base_fs_truncate(fd, 0);
    if (error != SUCCESS) {
        fprintf(stderr, "Error truncating the file");
        base_fs_close(fd);
        return 1;
    }

    // Write some data to the file
    const char* data = "Hello, world!\n";
    error = base_fs_write(fd, data, strlen(data), &bytesWritten);
    if (error != SUCCESS) {
        fprintf(stderr, "Error writing to the file");
        base_fs_close(fd);
        return 1;
    }

    // Move the file pointer to the beginning
    int newpos = 0;
    base_fs_seek(fd, 0, SEEK_SET, &newpos);

    // Read data from the file
    error = base_fs_read(fd, buffer, BUFFER_SIZE - 1, &bytesRead);
    if (error != SUCCESS) {
        fprintf(stderr, "Error reading from the file");
        base_fs_close(fd);
        return 1;
    }

    // Null-terminate the buffer
    buffer[bytesRead] = '\0';

    printf("Contents of the file: %s", buffer);

    // Close the file
    base_fs_close(fd);

    return 0;
}

