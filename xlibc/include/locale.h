#ifndef LOCALE_H
#define LOCALE_H
#ifdef __cplusplus
extern "C" {
#endif

#define LC_CTYPE 1

#define LC_CTYPE_MASK   (1 << 0)
#define LC_COLLATE_MASK (1 << 1)
#define LC_NUMERIC_MASK (1 << 2)
#define LC_MONETARY_MASK (1 << 3)
#define LC_TIME_MASK    (1 << 4)
#define LC_MESSAGES_MASK (1 << 5)
#define LC_ALL_MASK     ((1 << 6) - 1)
#define LC_ALL          6

typedef int locale_t;

locale_t uselocale(locale_t locale);
locale_t newlocale(int category_mask, const char *locale, locale_t base);
void freelocale(locale_t locobj);
char *setlocale(int category, const char *locale);



struct lconv {
	char    *currency_symbol;
	char    *decimal_point;
	char     frac_digits;
	char    *grouping;
	char    *int_curr_symbol;
	char     int_frac_digits;
	char     int_n_cs_precedes;
	char     int_n_sep_by_space;
	char     int_n_sign_posn;
	char     int_p_cs_precedes;
	char     int_p_sep_by_space;
	char     int_p_sign_posn;
	char    *mon_decimal_point;
	char    *mon_grouping;
	char    *mon_thousands_sep;
	char    *negative_sign;
	char     n_cs_precedes;
	char     n_sep_by_space;
	char     n_sign_posn;
	char    *positive_sign;
	char     p_cs_precedes;
	char     p_sep_by_space;
	char     p_sign_posn;
	char    *thousands_sep;
};
struct lconv* localeconv();

#ifdef __cplusplus
}
#endif
#endif // LOCALE_H
