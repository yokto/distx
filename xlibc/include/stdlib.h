#ifndef STDLIB_H
#define STDLIB_H

typedef unsigned int w_char;
typedef unsigned long size_t;
typedef struct {
	int quot;
	int rem;
} div_t;
typedef struct {
	long int quot;
	long int rem;
} ldiv_t;
typedef struct {
	long long int quot;
	long long int rem;
} lldiv_t;

void free(void* ptr);
void* malloc(size_t new_size);

#endif // STDLIB_H
