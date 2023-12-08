#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
#define RTLD_NOW 0
#define RTLD_LAZY 0
#define RTLD_GLOBAL 0
#define RTLD_LOCAL 0
#define RTLD_DEFAULT 0

typedef struct Dl_info {
	char * dli_fname;
	void *dli_fbase;              /* Load address of that object.  */  
	char * dli_sname;
	void* dli_saddr;
} Dl_info;

int dladdr(const void *addr, Dl_info *info);
void *dlsym(void * handle, const char * symbol);
void *dlopen(const char *filename, int flags);
int dlclose(void *handle);
char *dlerror(void);

#ifdef __cplusplus
}
#endif
