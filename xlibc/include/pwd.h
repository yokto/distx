#pragma once

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct passwd *getpwnam(const char *name);
 int getpwnam_r(const char * name, struct passwd * pwd,
                      char * buf, size_t buflen,
                      struct passwd ** result);

     int getpwuid_r(uid_t uid, struct passwd * pwd,
                      char * buf, size_t buflen,
                      struct passwd ** result);


#ifdef __cplusplus
}
#endif
