#ifndef CTYPE_H
#define CTYPE_H
#ifdef __cplusplus
extern "C" {
#endif

int isdigit(int c);
int isxdigit(int c);
#define isdigit_l(c, locale) (isdigit(c))
#define isxdigit_l(c, locale) (isxdigit(c))
int isascii(int c);
int isspace(int c);
int isprint(int c);
int iscntrl(int c);
int isupper(int c);
int islower(int c);
#define isupper_l(c, locale) (isupper(c))
#define islower_l(c, locale) (islower(c))
int isalpha(int c);
int ispunct(int c);
int isalnum(int c);
int isgraph(int c);
int toupper(int c);
#define toupper_l(c, l) toupper(c)
int tolower(int c);
#define tolower_l(c, l) tolower(c)
inline static int isblank(int ch) { return (ch == ' ' || ch == '\t'); }

#ifdef __cplusplus
}
#endif
#endif // CTYPE_H
