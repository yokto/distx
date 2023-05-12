#ifndef STRING_H
#define STRING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>


size_t strlen(const char *s);
int strncmp(const char* str1, const char* str2, size_t n);
int strcmp(const char *s1, const char *s2);
void *memmove(void *dest, const void *src, size_t n);
void *memcpy(void * dest, const void * src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
typedef int mbstate_t;

#ifdef __cplusplus
}
#endif
#endif // STRING_H
