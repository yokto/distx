#ifndef STDLIB_H
#define STDLIB_H

#include <base/types.h>
#include <systypes.h>

#ifdef __cplusplus
extern "C" {
#endif

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
char *getenv(const char *name);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
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


typedef void* FILE; // not really supposed to be here
extern FILE* stderr; // not really supposed to be here
int fprintf(FILE* stream, const char * format, ...); // not really supposed to be here
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
_Noreturn void exit(int status);
void abort() __attribute__ ((__noreturn__));

#define MB_CUR_MAX 1

static inline long llabs(long long l) { return (l >= 0) ? l : -l; }

char* getenv(const char* name);
void qsort(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*));
int atoi(const char *nptr);
double atof(const char *nptr);

int rand(void);
int rand_r(unsigned int *seedp);
void srand(unsigned int seed);

int ftruncate(int fd, uint64_t length);
static inline void _Exit( int exit_code ) { exit(exit_code); }

#ifdef __cplusplus
}
#endif
#endif // STDLIB_H
