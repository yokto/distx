#ifndef BASE_TYPES_H
#define BASE_TYPES_H
#ifdef __cplusplus
extern "C" {
#endif

// these are all the types that should really be needed in our base interface
// int64_t for file positions even on 32-bit systems
// intptr_t for system resources. on most systems uint32_t would be enough. however these days we could imagine a system with more than 4'000'000'000 resources. However not with more resources than memory.
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef long intptr_t;
typedef unsigned long uintptr_t;

#ifdef __cplusplus
}
#endif
#endif // BASE_TYPES_H
