#include <common.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

DLL_PUBLIC
size_t strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s) {}
    return (s - str);
}

DLL_PUBLIC
size_t strnlen(const char *str, size_t maxlen) {
    size_t length = 0;

    while (length < maxlen && str[length] != '\0') {
        length++;
    }

    return length;
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
char* stpcpy(char* dest, const char* src) {
    // Copy characters from source to destination until null terminator is reached
    while (*src != '\0') {
        *dest = *src; // Copy the character
        dest++;       // Move destination pointer to the next position
        src++;        // Move source pointer to the next position
    }

    *dest = '\0'; // Null-terminate the destination string

    return dest;
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
	memcpy(tmp, src, n);

	// this line does nothing. But if we don't have it the compiler will realize it's memmove and replace the inner part with a call to memmove causing an infinite recursion.
	if (n != 0) { dst_ptr[0] = 0xdd; }

        // Copy tmp buffer to destination
	memcpy(dest, tmp, n);

        free(tmp);
    } else {
        // No overlap, copy directly from source to destination
	memcpy(dest, src, n);
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
void* mempcpy(void* dest, const void* src, size_t n) {
    char* dest_ptr = (char*)dest;
    const char* src_ptr = (const char*)src;

    // Copy bytes from source to destination
    for (size_t i = 0; i < n; i++) {
        *dest_ptr++ = *src_ptr++;
    }

    return dest_ptr + n;
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

DLL_PUBLIC
char* strdup(const char* str) {
    // Check if the input string is NULL
    if (str == NULL) {
        return NULL;
    }

    // Get the length of the input string
    size_t len = strlen(str);

    // Allocate memory for the new string (plus one for the null terminator)
    char* new_str = (char*)malloc(len + 1);

    // Check if memory allocation was successful
    if (new_str == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy the input string into the newly allocated memory
    strcpy(new_str, str);

    return new_str;
}

DLL_PUBLIC
char *strtok_r(char *restrict str, const char *restrict delim, char **restrict saveptr) {
    char *start, *end;

    // If str is NULL, use the saved pointer from the previous call (if available).
    if (str == NULL) {
        str = *saveptr;
    }

    // Skip leading delimiters.
    str += strspn(str, delim);

    // If the string is empty or contains only delimiters, return NULL.
    if (*str == '\0') {
        *saveptr = str;
        return NULL;
    }

    // Find the end of the token.
    end = str + strcspn(str, delim);

    // Save the end of the current token in saveptr for the next call.
    if (*end != '\0') {
        *end = '\0';
        *saveptr = end + 1;
    } else {
        *saveptr = end;
    }

    // Set start to the beginning of the current token.
    start = str;

    return start;
}

DLL_PUBLIC
size_t strspn(const char *str, const char *charset) {
    size_t count = 0;
    bool found = false;

    while (*str) {
        const char *c = charset;
        found = false;

        // Check if the current character is present in the charset.
        while (*c) {
            if (*c == *str) {
                found = true;
                break;
            }
            c++;
        }

        // If the character is found, increment the count and move to the next character.
        if (found) {
            count++;
            str++;
        } else {
            // If the character is not found, break out of the loop.
            break;
        }
    }

    return count;
}

DLL_PUBLIC
size_t strcspn(const char *str, const char *charset) {
    size_t count = 0;
    bool found = false;

    while (*str) {
        const char *c = charset;
        found = false;

        // Check if the current character is present in the charset.
        while (*c) {
            if (*c == *str) {
                found = true;
                break;
            }
            c++;
        }

        // If the character is found, break out of the loop.
        if (found) {
            break;
        } else {
            // If the character is not found, increment the count and move to the next character.
            count++;
            str++;
        }
    }

    return count;
}

DLL_PUBLIC char *strchr(const char *str, int ch) {
    while (*str != '\0') {
        if (*str == ch) {
            return (char *)str;
        }
        str++;
    }

    // If the character was not found, return NULL
    return NULL;
}

DLL_PUBLIC
int atoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // Skip leading white spaces
    while (str[i] == ' ') {
        i++;
    }

    // Check for sign
    if (str[i] == '+' || str[i] == '-') {
        sign = (str[i++] == '-') ? -1 : 1;
    }

    // Convert digits to integer
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i++] - '0');
    }

    return result * sign;
}

DLL_PUBLIC
double atof(const char *arr)
{
	double value = 0;
	int afterdot=0;
	double scale=1;
	int negative = 0; 

	if (*arr == '-') {
		arr++;
		negative = 1;
	}
	while (*arr) {
		if (afterdot) {
			scale = scale/10;
			value = value + (*arr-'0')*scale;
		} else {
			if (*arr == '.') {
				afterdot++;
			}
			else
				value = value * 10.0 + (*arr - '0');
		}
		arr++;
	}
	if(negative) return -value;
	else    return  value;
}


char* strrchr(const char* str, int ch) {
    const char* last_occurrence = NULL;

    while (*str != '\0') {
        if (*str == ch) {
            last_occurrence = str;
        }
        str++;
    }

    // Handle the case when ch is the null terminator.
    if (ch == '\0') {
        last_occurrence = str;
    }

    return (char*)last_occurrence;
}

DLL_PUBLIC
char* strpbrk(const char* str, const char* charset) {
    while (*str != '\0') {
        const char* ch = charset;
        while (*ch != '\0') {
            if (*str == *ch) {
                return (char*)str;
            }
            ch++;
        }
        str++;
    }

    return NULL;
}


DLL_PUBLIC
char* strtok(char* str, const char* delim) {
    static char* buffer = NULL;
    if (str != NULL) {
        buffer = str;
    }

    if (buffer == NULL || *buffer == '\0') {
        return NULL;
    }

    char* token = buffer;
    char* delimiter = strchr(buffer, *delim);

    if (delimiter != NULL) {
        *delimiter = '\0';
        buffer = delimiter + 1;
    } else {
        buffer = NULL;
    }

    return token;
}

DLL_PUBLIC
char* strcat(char* dest, const char* src) {
    char* original_dest = dest;

    // Move the dest pointer to the end of the destination string
    while (*dest) {
        dest++;
    }

    // Copy characters from the source to the destination
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }

    // Null-terminate the destination string
    *dest = '\0';

    return original_dest;
}

DLL_PUBLIC
char* strncat(char* dest, const char* src, size_t n) {
    // Find the end of the destination string
    char* destEnd = dest;
    while (*destEnd != '\0') {
        destEnd++;
    }

    // Copy at most n characters from src to dest
    while (*src != '\0' && n > 0) {
        *destEnd = *src;
        destEnd++;
        src++;
        n--;
    }

    // Null-terminate the resulting string
    *destEnd = '\0';

    return dest;
}

DLL_PUBLIC
char *strstr(const char *haystack, const char *needle) {
    if (*needle == '\0') {
        return (char *)haystack;  // Empty needle, return the whole haystack
    }

    while (*haystack != '\0') {
        const char *h = haystack;
        const char *n = needle;

        while (*n != '\0' && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char *)haystack;  // Found the substring
        }

        haystack++;
    }

    return NULL;  // Substring not found
}

DLL_PUBLIC
void *memrchr(const void *s, int c, size_t n) {
    const unsigned char *p = (const unsigned char *)s + n - 1;

    while (n > 0) {
        if (*p == (unsigned char)c) {
            return (void *)p;
        }
        p--;
        n--;
    }

    return NULL;
}
