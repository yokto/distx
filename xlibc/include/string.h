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
void *memchr(const void *s, int c, size_t n);
char* strerror_r(int errnum, char* buf, size_t buflen);
int strcoll(const char *s1, const char *s2);
#define strcoll_l(s1, s2, locale) (strcoll(s1, s2))
size_t strxfrm(char * dst, const char * src, size_t n);
#define strxfrm_l(d, s, n, l) strxfrm(d, s, n)

typedef int mbstate_t;

#ifdef __cplusplus
}
#endif
#endif // STRING_H
