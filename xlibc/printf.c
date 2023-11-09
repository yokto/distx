#include <stdarg.h>
#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <error.h>

#define SUCCESS 0
#define ENOIMPL (-1)

#define CHECKPUTC(c) \
	if (putc != 0 && 1 != putc(arg, c)) { \
		return -1; \
	} else { \
		(*count)++; \
	}

#define CHECK(f) \
	error = (f); \
	if (error != SUCCESS) {\
		return error;\
	}

bool DEBUG=false;
__attribute__((constructor)) void isDebug() {
	if (getenv("XLIB_DEBUG")) { DEBUG=true; }
}

#define MAX_NUM_WIDTH 32
int32_t print_number(int (*putc)(void* arg, int chr), void* arg, int64_t number, uint64_t unumber, int base, int width, bool leading_zeros, size_t * count, bool capitalization, bool sign)
{
    if (width < 1) { width = 1; }
    // Allocate a buffer for the number representation
    char buffer[MAX_NUM_WIDTH+2];  // Assuming a maximum width of 32 characters
    
    // Convert the number to a string representation with the specified base
    size_t index = sizeof(buffer) - 1;
    buffer[index] = '\0';
    
    const char * const nums = capitalization ? "0123456789ABCDEF" : "0123456789abcdef";
    if (sign) {
	    const bool negative = number < 0;
	    if (negative) {
	    	number = -number;
	    }
	    do {
		    buffer[--index] = nums[number % base];
		    number /= base;
	    } while (number != 0);
	    if (negative) {
		    buffer[--index] = '-';
	    }
    } else {
	    do {
		    buffer[--index] = nums[unumber % base];
		    unumber /= base;
	    } while (unumber != 0);
    }
    
    // Determine the actual width of the number representation
    int actualWidth = sizeof(buffer) - 1 - index;
    
    // Print padding if necessary
    if (width > actualWidth) {
        int paddingCount = width - actualWidth;
        
        for (int i = 0; i < paddingCount; i++) {
            CHECKPUTC(leading_zeros ? '0' : ' ');
        }
    }
    
    // Print the number representation
    for (; index < sizeof(buffer) - 1; index++) {
        CHECKPUTC(buffer[index]);
    }
    return SUCCESS;
}
int32_t sprint_number(int (*putc)(void* arg, int chr), void* arg, int64_t number, int base, int width, bool leading_zeros, size_t * count, bool capitalization) {
	return print_number(putc, arg, number, 0, base, width, leading_zeros, count, capitalization, true);
}
int32_t uprint_number(int (*putc)(void* arg, int chr), void* arg, uint64_t unumber, int base, int width, bool leading_zeros, size_t * count, bool capitalization) {
	return print_number(putc, arg, 0, unumber, base, width, leading_zeros, count, capitalization, false);
}

// returns the number of bytes written or ENOIMPL if something is not implemented
// if putc is null it will only count the chars
int32_t internal_printf(int (*putc)(void* arg, int chr), void* arg, const char* format, va_list args, size_t * count)
{
	int32_t error = 0;
	while (*format != '\0') {
		if (*format == '%') {
			format++;
			bool zero_padding = false;
			// https://cplusplus.com/reference/cstdio/printf/
			if (*format == '0') { // flags
				zero_padding = true;
				format++;
			}
			int min_width = 0;
			if (*format == '*') { // width
				min_width = va_arg(args, int);
				format++;
			} else {
				min_width = atoi(format);
			}
			while(*format <= '9' && *format >= '0') {
				format++;
			}
			bool hh = false;
			bool h = false;
			bool l = false;
			bool ll = false;
			bool z = false;
			if (format[0] == 'h' && format[1] == 'h') { // specifier
				hh = true;
				format += 2;
			} else if (format[0] == 'h') {
				h = true;
				format++;
			} else if (format[0] == 'l' && format[1] == 'l') {
				ll = true;
				format += 2;
			} else if (format[0] == 'l') {
				l = true;
				format++;
			} else if (format[0] == 'z') {
				z = true;
				format++;
			}

			int base = 0;
			bool is_signed = false;
			bool print_n = false;
			bool capitalization = false;
			// Handle different specifiers
			switch (*format) { // specifier
				case '%':
					CHECKPUTC('%')
					break;

				case 'p':
					base = 16;
					is_signed = false;
					print_n = true;
					z = true;
					CHECKPUTC('0')
					CHECKPUTC('x')
					break;

				case 'o':
					base = 8;
					is_signed = false;
					print_n = true;
					break;
				case 'd':
					base = 10;
					is_signed = true;
					print_n = true;
					break;
				case 'u':
					base = 10;
					is_signed = false;
					print_n = true;
					break;
				case 'x':
					base = 16;
					is_signed = false;
					print_n = true;
					break;
				case 'X':
					base = 16;
					is_signed = false;
					print_n = true;
					capitalization = true;
					break;

				case 's': // Handle string
					{
						char* str = va_arg(args, char*);
						while (*str != '\0') {
							CHECKPUTC(*str)
							str++;
						}
					}
					break;

					// Add more cases for other specifiers as needed

				default:
					{
						debug_printf("not implemented \"%s\"", format);
						__builtin_trap();
						const char* str = "\nnot implemented in printf ";
						while (*str != '\0') {
							CHECKPUTC(*str)
							str++;
						}
						CHECKPUTC('%')
						CHECKPUTC(*format)
						return ENOIMPL;
					}
					// Invalid specifier
					// Handle error or unexpected input
					break;
			}
			if (print_n) {
				size_t res = 0; 
				if (hh && is_signed) {
					signed   char      ptr = va_arg(args, signed char       );
					CHECK(sprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (h && is_signed) {
					signed   short     ptr = va_arg(args, signed short      );
					CHECK(sprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (l && is_signed) {
					signed   long      ptr = va_arg(args, signed long       );
					CHECK(sprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (ll && is_signed) {
					signed   long long ptr = va_arg(args, signed long long  );
					CHECK(sprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (z && is_signed) {
					size_t             ptr = va_arg(args,        size_t     );
					CHECK(sprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (is_signed) {
					signed   int       ptr = va_arg(args, signed int        );
					CHECK(sprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));

				} else if (hh) {
					unsigned char      ptr = va_arg(args, unsigned char     );
					CHECK(uprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (h) {
					unsigned short     ptr = va_arg(args, unsigned short    );
					CHECK(uprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (l) {
					unsigned long      ptr = va_arg(args, unsigned long     );
					CHECK(uprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (ll) {
					unsigned long long ptr = va_arg(args, unsigned long long);
					CHECK(uprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else if (z) {
					size_t             ptr = va_arg(args, size_t            );
					CHECK(uprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				} else {
					unsigned int       ptr = va_arg(args, unsigned int      );
					CHECK(uprint_number(putc, arg, ptr, base, min_width, zero_padding, count, capitalization));
				}
			}
		}
		else {
			CHECKPUTC(*format);
		}

		format++;
	}

	return error;
}



struct print_state {
	char* buf;
	size_t buf_size;
	size_t to_write;
	FILE* file;
};

#define UNUSED(x) (void)(x)
#define debug_puts_buf 64
int debug_puts(void* arg, int c) {
	struct print_state * state = (struct print_state*)arg;
	state->buf[state->to_write] = c;
	state->to_write ++;
	if (state->to_write >= state->buf_size) {
		__write(state->buf);
		state->to_write = 0;
	}
	return 1;
}
int debug_printf(char* format, ...) {
	if (!DEBUG) { return 0; }
	va_list args;
	char buf[debug_puts_buf + 1];
	buf[debug_puts_buf] = '\0';
	struct print_state state = { buf, debug_puts_buf, 0, 0 };
	va_start(args, format);

	size_t ret = 0;
	int32_t err = internal_printf(debug_puts, &state, format, args, &ret); 
	if (err != SUCCESS) {
		errno = err;
		return -1;
	}
	state.buf[state.to_write] = '\0';
	__write(state.buf);
	if (ret < 0) { __exit(-1); }

	va_end(args);
	return ret;
}


int snprintf_puts(void* arg, int c) {
	struct print_state * state = (struct print_state*)arg;
	if (state->to_write >= state->buf_size - 1) {
		return 0;
	}
	state->buf[state->to_write] = c;
	state->to_write ++;
	return 1;
}
DLL_PUBLIC
int vsnprintf(char * s, size_t n, const char * format, va_list args) {
	struct print_state state = { s, n, 0, 0 };
	size_t ret = 0;
	const int32_t err = internal_printf(snprintf_puts, &state, format, args, &ret); 
	if (err != SUCCESS) {
		errno = err;
		return -1;
	}
	s[state.to_write] = '\0';
	return ret;
}

DLL_PUBLIC
int snprintf(char * s, size_t n, const char * format, ...) {
	va_list args;
	va_start(args, format);
	const int ret = vsnprintf(s, n, format, args);
	va_end(args);
	return ret;
}

DLL_PUBLIC
int sprintf(char * s, const char * format, ...) {
	va_list args;
	va_start(args, format);
	const int ret = vsnprintf(s, SIZE_MAX, format, args);
	va_end(args);
	return ret;
}

DLL_PUBLIC
int vasprintf(char ** strp, const char * format, va_list args) {
	size_t ret = 0;
	int32_t err = internal_printf(0, 0, format, args, &ret); 
	if (err != SUCCESS) {
		errno = err;
		return -1;
	}
	char* p = (char*)malloc(ret);
	*strp = p;
	return vsnprintf(p, ret, format, args);
}

DLL_PUBLIC
int printf(const char *restrict format, ...) {
	va_list args;
	va_start(args, format);
	int ret = vfprintf(stdout, format, args);
	va_end(args);
	return ret;
}

#define vfprintf_bufsz 64
int vfprintf_puts(void* arg, int c) {
	struct print_state * state = (struct print_state*)arg;
	state->buf[state->to_write] = c;
	state->to_write ++;
	if (state->to_write >= state->buf_size) {
		fwrite(state->buf, 1, state->buf_size, state->file);
		state->to_write = 0;
	}
	return 1;
}
DLL_PUBLIC
int vfprintf(FILE* file, const char *restrict format, va_list args) {
	char buf[vfprintf_bufsz];
	struct print_state state = { buf, vfprintf_bufsz, 0, file };

	size_t ret = 0;
	int32_t err = internal_printf(vfprintf_puts, &state, format, args, &ret); 
	if (err != SUCCESS) {
		errno = err;
		return -1;
	}
	fwrite(state.buf, 1, state.to_write, state.file);
	if (ret < 0) { __exit(-1); }

	return ret;
}

DLL_PUBLIC
int putchar(int c) {
	char out = c;
	fwrite(&c, 1, 1, stdout);
}
