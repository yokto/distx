#ifndef WCTYPE_H
#define WCTYPE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <wchar.h>

int iswspace(wint_t wc);
#define iswspace_l(wc, l) iswspace(wc)
int iswdigit(wint_t wc);
#define iswdigit_l(wc, l) iswdigit(wc)
int iswxdigit(wint_t wc);
#define iswxdigit_l(wc, l) iswxdigit(wc)
int iswupper(wint_t wc);
int iswlower(wint_t wc);
int iswpunct(wint_t wc);
#define iswpunct_l(wc, l) iswpunct(wc)
int iswblank(wint_t wc);
#define iswblank_l(wc, l) iswblank(wc)
int iswalpha(wint_t wc);
#define iswalpha_l(wc, l) iswalpha(wc)
int iswprint(wint_t wc);
#define iswprint_l(wc, l) iswprint(wc)
int iswcntrl(wint_t wc);
#define iswcntrl_l(wc, l) iswcntrl(wc)
#define iswupper_l(wc, l) iswupper(wc)
#define iswlower_l(wc, l) iswlower(wc)
wint_t towupper(wint_t wc);
#define towupper_l(wc, l) towupper(wc)
wint_t towlower(wint_t wc);
#define towlower_l(wc, l) towlower(wc)

#ifdef __cplusplus
}
#endif
#endif // WCTYPE_H
