#ifndef WCHAR_H
#define WCHAR_H

#include <stdarg.h>
#include <base/types.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WEOF (0xffffffffu)

typedef int mbstate_t;
typedef unsigned int wint_t;
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

wchar_t* wcschr(const wchar_t* str, wchar_t wc);
wchar_t* wcspbrk(const wchar_t* str, const wchar_t* chars);
wchar_t* wcsstr(const wchar_t* str, const wchar_t* substr);
wchar_t* wcsrchr(const wchar_t* str, wchar_t wc);
wchar_t* wmemchr(const wchar_t* ptr, wchar_t wc, size_t num);
wint_t btowc(int c);
int wctob(wint_t wc);
long wcstol(const wchar_t *nptr, wchar_t **endptr, int base);
unsigned long wcstoul(const wchar_t *nptr, wchar_t **endptr, int base);
long long wcstoll(const wchar_t *nptr, wchar_t **endptr, int base);
unsigned long long wcstoull(const wchar_t *nptr, wchar_t **endptr, int base);
size_t wcsnrtombs(char *dest, const wchar_t **src, size_t nwc, size_t len, mbstate_t *ps);
size_t wcstombs(char *dest, const wchar_t *src, size_t len);
int wcrtomb(char *s, wchar_t wc, mbstate_t *ps);
int wctomb(char *s, wchar_t wc);
int mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
size_t mbsnrtowcs(wchar_t *dst, const char **src, size_t nms, size_t len, mbstate_t *ps);
size_t mbsrtowcs(wchar_t *dest, const char **src, size_t len, mbstate_t *ps);
size_t mbrlen(const char *s, size_t n, mbstate_t *ps);
float wcstof(const wchar_t *nptr, wchar_t **endptr);
double wcstod(const wchar_t *nptr, wchar_t **endptr);
long double wcstold(const wchar_t *nptr, wchar_t **endptr);
int vswprintf (wchar_t * ws, size_t len, const wchar_t * format, va_list arg );
int swprintf (wchar_t* ws, size_t len, const wchar_t* format, ...);
int wcscoll(const wchar_t *ws1, const wchar_t *ws2);
#define wcscoll_l(w1, w2, l) wcscoll(w1, w2)
size_t wcsxfrm(wchar_t * ws1, const wchar_t * ws2, size_t n);
#define wcsxfrm_l(w1, w2, n, l) wcsxfrm(w1, w2, n)


#ifdef __cplusplus
}
#endif
#endif // WCHAR_H
