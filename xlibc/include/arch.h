#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__
#define ARCH "x86_64"
#elif __aarch64__
#define ARCH "aarch64"
#endif

#ifdef __cplusplus
}
#endif
