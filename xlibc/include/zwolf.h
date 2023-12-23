#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* exit program
 * probably can replace this with lib exit
 */
void zwolf_exit(int ret)  __attribute__ ((noreturn));

/* writes to stdout. for debugging before library functions are available */
int zwolf_write(const char* str);

/* reads this threads errno */
extern int zwolf_errno();

#define ZWOLF_OPEN_EXTERNAL 1

/* open a library */
void* zwolf_open(const char* name, uint32_t flags);

/* find a symbol in an open library */
void* zwolf_sym(void* handle, const char* name);

#ifdef __cplusplus
}
#endif
