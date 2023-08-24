#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    struct stat file_stat;

    if (stat(file_path, &file_stat) != 0) {
        perror("Error");
        return 1;
    }

    printf("File: %s\n", file_path);
    printf("Size: %ld bytes\n", (long long) file_stat.st_size);
    printf("Owner: %d\n", file_stat.st_uid);
    printf("Group: %d\n", file_stat.st_gid);
    printf("Permissions: %o\n", file_stat.st_mode & 0777);
    printf("Last access time: %ld\n", file_stat.st_atime);
    printf("Last modification time: %ld\n", file_stat.st_mtime);
    printf("Last status change time: %ld\n", file_stat.st_ctime);

    return 0;
}

