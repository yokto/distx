#pragma once

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

int gettimeofday(struct timeval * tv, struct timezone * tz);


struct timezone {
        int tz_minuteswest;     /* minutes west of Greenwich */
        int tz_dsttime;         /* type of DST correction */
};


#ifdef __cplusplus
}
#endif
