#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct Dl_info {
	char * dli_fname;
	void *dli_fbase;              /* Load address of that object.  */  
	char * dli_sname;
	void* dli_saddr;
} Dl_info;

int dladdr(const void *addr, Dl_info *info);

#ifdef __cplusplus
}
#endif
