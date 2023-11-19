#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void printFileMTime(const char *filename) {
    struct stat fileStat;

    // Retrieve file information
    if (stat(filename, &fileStat) == -1) {
        perror("Error getting file information");
        return;
    }

    // Extract and print the modification time
    time_t mtime = fileStat.st_mtime;
    printf("File: %s\n", filename);
    printf("Last modification time: %s", ctime(&mtime));
}

int main() {
    const char *filename = "./testc_mtime.c"; // Replace with the path to your file

    printFileMTime(filename);

    return 0;
}

