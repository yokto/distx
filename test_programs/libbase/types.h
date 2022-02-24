#define B_64BIT

#ifdef B_64BIT
typedef unsigned long long size_t;
typedef long long ssize_t;
#else
typedef unsigned int size_t;
typedef int ssize_t;
#endif

typedef unsigned char uint8_t;
typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;

/* hmm lots of 32 bit systems do have uint64_t */
#ifdef B_64BIT
typedef unsigned long long uint64_t;
typedef long long int64_t;
#endif
