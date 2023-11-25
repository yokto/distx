#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* exit program
 * probably can replace this with lib exit
 */
void zwolf_exit(int ret)  __attribute__ ((noreturn));

/* writes to stdout. for debugging before library functions are available */
int zwolf_write(char* str);

/* reads this threads errno */
extern int zwolf_errno();

/* open a library */
void* zwolf_open(char* name);

/* find a symbol in an open library */
void* zwolf_sym(void* handle, char* name);

#ifdef __cplusplus
}
#endif
