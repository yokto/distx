#ifndef TIME_H
#define TIME_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#define CLOCKS_PER_SEC 1

typedef int64_t time_t;

#define CLOCK_REALTIME 1
#define CLOCK_MONOTONIC 2
typedef struct timespec {
    time_t  tv_sec;  /* Seconds */
    int64_t    tv_nsec; /* Nanoseconds */
} timespec;
typedef int clockid_t;
int clock_gettime(clockid_t clockid, struct timespec *tp);



struct tm {
	int tm_sec;         /* seconds,  range 0 to 59          */
	int tm_min;         /* minutes, range 0 to 59           */
	int tm_hour;        /* hours, range 0 to 23             */
	int tm_mday;        /* day of the month, range 1 to 31  */
	int tm_mon;         /* month, range 0 to 11             */
	int tm_year;        /* The number of years since 1900   */
	int tm_wday;        /* day of the week, range 0 to 6    */
	int tm_yday;        /* day in the year, range 0 to 365  */
	int tm_isdst;       /* daylight saving time             */
};
typedef uint64_t clock_t;

char *asctime(const struct tm *timeptr);
clock_t clock(void);
char *ctime(const time_t *timer);
double difftime(time_t time1, time_t time2);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
struct tm *localtime_r(const time_t *timer, struct tm* result);
time_t mktime(struct tm *timeptr);
size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr);
#define strftime_l(s, m, f, t, l) strftime(s, m ,f ,t)
time_t time(time_t *timer);
int nanosleep(const struct timespec *req, struct timespec *rem);


struct timeval {
                  time_t      tv_sec;  /* Seconds */
                  uint32_t tv_usec; /* Microseconds */
              };



#ifdef __cplusplus
}
#endif
#endif // TIME_H
