#include <common.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

DLL_PUBLIC
size_t strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s) {}
    return (s - str);
}

DLL_PUBLIC
char* strncpy(char* dest, const char* src, size_t n) {
    char* d = dest;
    const char* s = src;
    size_t i = 0;

    // Copy at most n characters from src to dest
    while (*s && i < n) {
        *d++ = *s++;
        i++;
    }

    // If n is greater than the length of src, pad dest with null characters
    while (i < n) {
        *d++ = '\0';
        i++;
    }

    return dest;
}

DLL_PUBLIC
char* strcpy(char* dest, const char* src) {
    char* original_dest = dest; // Store the original destination pointer

    // Copy characters from source to destination until null terminator is reached
    while (*src != '\0') {
        *dest = *src; // Copy the character
        dest++;       // Move destination pointer to the next position
        src++;        // Move source pointer to the next position
    }

    *dest = '\0'; // Null-terminate the destination string

    return original_dest; // Return the original destination pointer
}


DLL_PUBLIC
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

DLL_PUBLIC
int strncmp(const char* str1, const char* str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] == '\0' || str1[i] != str2[i]) {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }
    }
    return 0;
}

DLL_PUBLIC
int memcmp(const void* ptr1, const void* ptr2, size_t size) {
    const unsigned char* p1 = (const unsigned char*) ptr1;
    const unsigned char* p2 = (const unsigned char*) ptr2;
    for (size_t i = 0; i < size; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }
    return 0;
}

DLL_PUBLIC
void *memmove(void *dest, const void *src, size_t n) {
    char *dst_ptr = (char*) dest;
    const char *src_ptr = (const char*) src;

    // If source and destination regions overlap, use a temporary buffer
    if (src_ptr < dst_ptr && dst_ptr < src_ptr + n) {
        char *tmp = (char*) malloc(n);
        if (tmp == NULL) {
            return NULL; // Out of memory
        }

        // Copy source to tmp buffer
        for (size_t i = 0; i < n; i++) {
            *(tmp + i) = *(src_ptr + i);
        }

        // Copy tmp buffer to destination
        for (size_t i = 0; i < n; i++) {
            *(dst_ptr + i) = *(tmp + i);
        }

        free(tmp);
    } else {
        // No overlap, copy directly from source to destination
        for (size_t i = 0; i < n; i++) {
            *(dst_ptr + i) = *(src_ptr + i);
        }
    }

    return dest;
}

DLL_PUBLIC
void* memcpy(void* dest, const void* src, size_t n) {
    char* dest_ptr = (char*)dest;
    const char* src_ptr = (const char*)src;

    // Copy bytes from source to destination
    for (size_t i = 0; i < n; i++) {
        *dest_ptr++ = *src_ptr++;
    }

    return dest;
}

DLL_PUBLIC
void *memchr(const void *s, int c, size_t n) {
    const unsigned char *p = s;
    for (size_t i = 0; i < n; i++) {
        if (p[i] == (unsigned char)c) {
            return (void *)(p + i);
        }
    }
    return NULL;
}

DLL_PUBLIC
void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;

    // Set each byte in the memory block to the specified value
    for (size_t i = 0; i < num; i++) {
        *p++ = (unsigned char)value;
    }

    return ptr;
}

DLL_PUBLIC
int strcoll(const char *s1, const char *s2) {
    return strcmp(s1, s2);
}

DLL_PUBLIC
size_t strxfrm(char* dest, const char* src, size_t n) {
    strncpy(dest, src, n);
    return strlen(src);
}

