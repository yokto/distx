//#include <dirent.h>
#include <stdio.h>

int main() {
    const char* directory_path = ".";
    DIR* directory = opendir(directory_path);

    if (directory) {
        struct dirent* entry;
        while ((entry = readdir(directory)) != NULL) {
            //if (entry->d_type == DT_REG) {
                // Regular file
                printf("File: %s\n", entry->d_name);
            //} else if (entry->d_type == DT_DIR) {
                // Directory
             //   printf("Directory: %s\n", entry->d_name);
            //} else {
                // Other file types like symbolic links, etc.
            //    printf("Other: %s\n", entry->d_name);
            //}
        }
        closedir(directory);
    } else {
        fprintf(stdout, "Failed to open the directory");
    }

    return 0;
}

