#ifndef STDBOOL_H
#define STDBOOL_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef char bool;
#define true 1
#define false 0
#define __bool_true_false_are_defined 1
#endif

#ifdef __cplusplus
}
#endif
#endif // STDBOOL_H
