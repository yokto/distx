
#define WEOF (0xffffffffu)

typedef int mbstate_t;
typedef unsigned int wint_t;
//typedef __WCHAR_TYPE__ wchar_t;

wchar_t* wcschr(const wchar_t* str, wchar_t wc);
wchar_t* wcspbrk(const wchar_t* str, const wchar_t* chars);
wchar_t* wcsstr(const wchar_t* str, const wchar_t* substr);
wchar_t* wcsrchr(const wchar_t* str, wchar_t wc);
wchar_t* wmemchr(const wchar_t* ptr, wchar_t wc, size_t num);
