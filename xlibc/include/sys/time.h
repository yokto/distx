#pragma once

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif


struct timezone {
        int tz_minuteswest;     /* minutes west of Greenwich */
        int tz_dsttime;         /* type of DST correction */
};

int gettimeofday(struct timeval * tv, struct timezone * tz);


#ifdef __cplusplus
}
#endif
