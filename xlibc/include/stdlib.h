#ifndef STDLIB_H
#define STDLIB_H

#include <systypes.h>

#ifdef __cplusplus
extern "C" {
#endif

//typedef __WCHAR_TYPE__ wchar_t;
typedef __SIZE_TYPE__ size_t;
typedef struct {
	int quot;
	int rem;
} div_t;
typedef struct {
	long int quot;
	long int rem;
} ldiv_t;
typedef struct {
	long long int quot;
	long long int rem;
} lldiv_t;
inline div_t     div( int x, int y ) {
  div_t result;
  result.quot = x / y;
  result.rem = x % y;
  if (x < 0 && result.rem != 0) {
    result.quot--;
    result.rem += y;
  }
  return result;
}
inline ldiv_t    ldiv( long x, long y ) {
  ldiv_t result;
  result.quot = x / y;
  result.rem = x % y;
  if (x < 0 && result.rem != 0) {
    result.quot--;
    result.rem += y;
  }
  return result;
}
inline lldiv_t   lldiv( long long x, long long y ) {
  lldiv_t result;
  result.quot = x / y;
  result.rem = x % y;
  if (x < 0 && result.rem != 0) {
    result.quot--;
    result.rem += y;
  }
  return result;
}
#define ldiv __builtin_ldiv
#define lldiv __builtin_lldiv

void free(void* ptr);
void* malloc(size_t new_size);
void *calloc(size_t nmemb, size_t size);
#define alloca __builtin_alloca
//void *alloca(size_t size);
void* aligned_alloc(size_t alignment, size_t new_size);
void aligned_free(void* ptr);
void *realloc(void *ptr, size_t size);
void *memset(void *ptr, int value, size_t num);
void *memcpy(void * dest, const void * src, size_t n);
char *getenv(const char *name);
long int strtol(const char* str, char** endptr, int base);
int mbtowc(wchar_t *pwc, const char *s, size_t n);
unsigned long int strtoul(const char* str, char** endptr, int base);
long long int strtoll(const char* str, char** endptr, int base);
#define strtoll_l(str, endptr, base, locale) (strtoll(str, endptr, base))
//#define strtold_l(str, endptr, base, locale) (strtold(str, endptr, base))
#define strtoull_l(str, endptr, base, locale) (strtoull(str, endptr, base))
#define strtof_l(str, endptr, locale) (strtof(str, endptr))
#define strtod_l(str, endptr, locale) (strtod(str, endptr))
#define strtold_l(str, endptr, locale) (strtold(str, endptr))
unsigned long long int strtoull(const char* str, char** endptr, int base);
float strtof(const char *nptr, char **endptr);
double strtod(const char *nptr, char **endptr);
long double strtold (const char* nptr, char** endptr);


#define dladdr(x, y) 0
typedef void* FILE; // not really supposed to be here
extern FILE* stderr; // not really supposed to be here
int fprintf(FILE* stream, const char * format, ...); // not really supposed to be here
void abort() __attribute__ ((__noreturn__));

#define MB_CUR_MAX 1

#ifdef __cplusplus
}
#endif
#endif // STDLIB_H
