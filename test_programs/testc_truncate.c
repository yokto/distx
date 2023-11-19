#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char* filename = "./testcpp_fs_files2";
    const char* content = "This is a test file for POSIX seek and truncate functions.";
    const off_t truncateLength = 20;
    const off_t seekPosition = 10;

    // Open or create a file
    int fileDescriptor = open(filename, O_CREAT | O_RDWR);
    if (fileDescriptor == -1) {
        perror("File open error");
        return 1;
    }

    // Write content to the file
    if (write(fileDescriptor, content, strlen(content)) == -1) {
        perror("File write error");
        close(fileDescriptor);
        return 1;
    }

    // Truncate the file
    if (ftruncate(fileDescriptor, truncateLength) == -1) {
        perror("File truncate error");
        close(fileDescriptor);
        return 1;
    }

    // Seek to a specific position in the file
    if (lseek(fileDescriptor, seekPosition, SEEK_SET) == -1) {
        perror("File seek error");
        close(fileDescriptor);
        return 1;
    }

    // Read and print the file content
    char fileContent[100];
    ssize_t bytesRead = read(fileDescriptor, fileContent, sizeof(fileContent));
    if (bytesRead == -1) {
        perror("File read error");
        close(fileDescriptor);
        return 1;
    }

    fileContent[bytesRead] = '\0'; // Null-terminate the string
    printf("File content: %s\n", fileContent);

    // Close the file
    close(fileDescriptor);

    return 0;
}
