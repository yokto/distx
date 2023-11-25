//#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {
    const char* directory_path = "/_zwolf/test_programs/x86_64";
    DIR* directory = opendir(directory_path);

    bool foundBin = false;

    if (directory) {
        struct dirent* entry;
        while ((entry = readdir(directory)) != NULL) {
		if (strcmp(entry->d_name, "bin") == 0) {
			foundBin = true;
		} else if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { // nothing
		} else {
			fprintf(stderr, "found unexpected file %s\n", entry->d_name);
			return -1;
		}
	}
        closedir(directory);
    } else {
        fprintf(stdout, "Failed to open the directory\n");
	return -1;
    }

    if (!foundBin) {
	    fprintf(stderr, "not found bin dir\n");
	    return -1;
    }

    return 0;
}

