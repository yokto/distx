#ifndef STRING_H
#define STRING_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>


size_t strlen(const char *s);
size_t strnlen(const char *str, size_t maxlen);
int strncmp(const char* str1, const char* str2, size_t n);
int strcmp(const char *s1, const char *s2);
char *strcpy(char * dest, const char *src);
char *stpcpy(char * dest, const char *src);
char *strncpy(char * dest, const char * src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *ptr, int value, size_t num);
void *memcpy(void * dest, const void * src, size_t n);
void *mempcpy(void * dest, const void * src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memchr(const void *s, int c, size_t n);
void *memrchr(const void *s, int c, size_t n);
char* strerror_r(int errnum, char* buf, size_t buflen);
char *strerror(int errnum);
int strcoll(const char *s1, const char *s2);
#define strcoll_l(s1, s2, locale) (strcoll(s1, s2))
size_t strxfrm(char * dst, const char * src, size_t n);
#define strxfrm_l(d, s, n, l) strxfrm(d, s, n)
char *strdup(const char *s);
char *strtok(char * str, const char * delim);
char *strtok_r(char * str, const char * delim, char ** saveptr);
size_t strspn(const char *s, const char *accept);
size_t strcspn(const char *str, const char *charset);
char* strcat(char* dest, const char* src);
char *strchr(const char *str, int ch);
char *strrchr(const char *str, int ch);
char *strstr(const char *haystack, const char *needle);

char *strsignal(int sig);

typedef int mbstate_t;

#ifdef __cplusplus
}
#endif
#endif // STRING_H
