#ifndef BASE_TYPES_H
#define BASE_TYPES_H
#ifdef __cplusplus
extern "C" {
#endif

#define __ZWOLF__ 1

// these are all the types that should really be needed in our base interface
// int64_t for file positions even on 32-bit systems
// intptr_t for system resources. on most systems uint32_t would be enough. however these days we could imagine a system with more than 4'000'000'000 resources. However not with more resources than memory.
typedef __UINT8_TYPE__   uint8_t;
typedef  __INT8_TYPE__    int8_t;
typedef __UINT16_TYPE__  uint16_t;
typedef  __INT16_TYPE__   int16_t;
typedef __UINT32_TYPE__  uint32_t;
typedef  __INT32_TYPE__   int32_t;
typedef __UINT64_TYPE__  uint64_t;
typedef  __INT64_TYPE__   int64_t;
typedef __UINTPTR_TYPE__ uintptr_t;
typedef  __INTPTR_TYPE__  intptr_t;

#ifdef __cplusplus
}
#endif
#endif // BASE_TYPES_H
