#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* exit program
 * probably can replace this with lib exit
 */
void xload_exit(int ret)  __attribute__ ((noreturn));

/* writes to stdout. for debugging before library functions are available */
int xload_write(const char* str);

/* reads this threads errno */
extern int xload_errno();

#define XLOAD_OPEN_EXTERNAL 1

/* open a library */
void* xload_open(const char* name, uint32_t flags);

/* find a symbol in an open library */
void* xload_sym(void* handle, const char* name);

extern const char * const xload_root;
extern const char * const xload_exec;

#ifdef __cplusplus
}
#endif
