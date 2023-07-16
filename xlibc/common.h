#define DLL_PUBLIC __attribute__ ((visibility ("default")))
#define UNUSED(x) (void)(x)
void __exit(int ret) __attribute__ ((weak));
int __write(char* str, size_t num) __attribute__ ((weak));
int __resolve(void* ptr, const char ** libfile, size_t* offset);

