#include <stdio.h>
#include <string.h>
#include <zlib.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    const char* original_string = "Testing zlib compression and decompression!";
    char compressed_buffer[MAX_BUFFER_SIZE];
    char decompressed_buffer[MAX_BUFFER_SIZE];
    uLong compressed_size, decompressed_size;

    // Compress the original string
    compressed_size = sizeof(compressed_buffer);
    if (compress((Bytef*)compressed_buffer, &compressed_size, (const Bytef*)original_string, strlen(original_string) + 1) != Z_OK) {
        fprintf(stderr, "Compression failed!\n");
        return 1;
    }

    // Decompress the compressed data
    decompressed_size = sizeof(decompressed_buffer);
    if (uncompress((Bytef*)decompressed_buffer, &decompressed_size, (const Bytef*)compressed_buffer, compressed_size) != Z_OK) {
        fprintf(stderr, "Decompression failed!\n");
        return 1;
    }

    // Print the results
    printf("Original String: %s\n", original_string);
    // printf("Compressed String: %.*s\n", (int)compressed_size, compressed_buffer);
    printf("Decompressed String: %s\n", decompressed_buffer);

    return 0;
}
