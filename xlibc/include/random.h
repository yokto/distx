#ifndef RANDOM_H
#define RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

int getentropy(void *buffer, size_t length);

#ifdef __cplusplus
}
#endif
#endif // RANDOM_H
