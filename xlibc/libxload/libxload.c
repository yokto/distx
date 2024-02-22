#include "xload.h"
void xload_exit(int ret) {}
int xload_write(const char* str) { return 0; }
int xload_errno() { return 0; }
void* xload_open(const char* name, uint32_t flags) { return 0; }
void* xload_sym(void* handle, const char* name) { return 0; }
const char * const xload_root = 0;
const char * const xload_exec = 0;
