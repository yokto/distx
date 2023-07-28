#include <stdarg.h>
#include <common.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ENOIMPL (-1)

#define CHECKPUTC(c) \
	if (putc != 0 && 1 != putc(arg, c)) { \
		return count; \
	} else { \
		count++; \
	}

int print_number(int (*putc)(void* arg, int chr), void* arg, int64_t number, uint64_t unumber, int base, int width, bool sign)
{
    size_t count = 0;
    // Allocate a buffer for the number representation
    char buffer[32];  // Assuming a maximum width of 32 characters
    
    // Convert the number to a string representation with the specified base
    size_t index = sizeof(buffer) - 1;
    buffer[index] = '\0';
    
    if (sign) {
	    const bool negative = number < 0;
	    if (negative) {
	    	number = -number;
	    }
	    do {
		    buffer[--index] = "0123456789abcdef"[number % base];
		    number /= base;
	    } while (number != 0);
	    if (negative) {
		    buffer[--index] = '-';
	    }
    } else {
	    do {
		    buffer[--index] = "0123456789abcdef"[unumber % base];
		    unumber /= base;
	    } while (unumber != 0);
    }
    
    // Determine the actual width of the number representation
    int actualWidth = sizeof(buffer) - 1 - index;
    
    // Print padding if necessary
    if (width > actualWidth) {
        int paddingCount = width - actualWidth;
        
        for (int i = 0; i < paddingCount; i++) {
            CHECKPUTC('0');
        }
    }
    
    // Print the number representation
    for (; index < sizeof(buffer) - 1; index++) {
        CHECKPUTC(buffer[index]);
    }
    return count;
}
int sprint_number(int (*putc)(void* arg, int chr), void* arg, int64_t number, int base, int width) {
	return print_number(putc, arg, number, 0, base, width, true);
}
int uprint_number(int (*putc)(void* arg, int chr), void* arg, uint64_t unumber, int base, int width) {
	return print_number(putc, arg, 0, unumber, base, width, false);
}

// returns the number of bytes written or ENOIMPL if something is not implemented
// if putc is null it will only count the chars
int internal_printf(int (*putc)(void* arg, int chr), void* arg, const char* format, va_list args)
{
	size_t count = 0;
	while (*format != '\0') {
		if (*format == '%') {
			format++;

			// Handle different specifiers
			switch (*format) {
				case '%':
					CHECKPUTC('%')
					break;

				case 'p':
					{
						void* ptr = va_arg(args, void*);
						size_t res = uprint_number(putc, arg, (size_t)ptr, 16, 16);
						count += res;
					}
					break;

				case 'd':
					{
						int ptr = va_arg(args, int);
						size_t res = sprint_number(putc, arg, (int64_t)ptr, 10, 0);
						count += res;
					}
					break;

				case 'z':
					{
						bool success = false;
						switch (format[1]) {
							case 'u':
								{
									format++;
									size_t ptr = va_arg(args, size_t);
									size_t res = uprint_number(putc, arg, (uint64_t)ptr, 10, 0);
									count += res;
									success = true;
								}
								break;
						}
						if (success) {
							break;
						}
					}
					break;


				case 'l':
					{
						bool success = false;
						switch (format[1]) {
							case 'd':
								{
									format++;
									long ptr = va_arg(args, long);
									size_t res = sprint_number(putc, arg, (int64_t)ptr, 10, 0);
									count += res;
									success = true;
								}
								break;
							case 'x':
								{
									format++;
									long ptr = va_arg(args, long);
									size_t res = sprint_number(putc, arg, (int64_t)ptr, 16, 0);
									count += res;
									success = true;
								}
								break;
							case 'u':
								{
									format++;
									unsigned long ptr = va_arg(args, unsigned long);
									size_t res = uprint_number(putc, arg, (uint64_t)ptr, 10, 0);
									count += res;
									success = true;
								}
								break;
						}
						if (success) {
							break;
						}
					}
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
		}
		else {
			CHECKPUTC(*format);
		}

		format++;
	}

	return count;
}



struct print_state {
	char* buf;
	size_t buf_size;
	size_t to_write;
	FILE* file;
};

#define UNUSED(x) (void)(x)
#define __debug_puts_buf 64
int __debug_puts(void* arg, int c) {
	struct print_state * state = (struct print_state*)arg;
	state->buf[state->to_write] = c;
	state->to_write ++;
	if (state->to_write >= state->buf_size) {
		__write(state->buf);
		state->to_write = 0;
	}
	return 1;
}
int __debug_printf(char* format, ...) {
	va_list args;
	char buf[__debug_puts_buf + 1];
	buf[__debug_puts_buf] = '\0';
	struct print_state state = { buf, __debug_puts_buf, 0, 0 };
	va_start(args, format);

	const int ret = internal_printf(__debug_puts, &state, format, args); 
	state.buf[state.to_write] = '\0';
	__write(state.buf);
	if (ret < 0) { __exit(-1); }

	va_end(args);
	return ret;
}


int snprintf_puts(void* arg, int c) {
	struct print_state * state = (struct print_state*)arg;
	if (state->to_write >= state->buf_size) {
		return 0;
	}
	state->buf[state->to_write] = c;
	state->to_write ++;
	return 1;
}
DLL_PUBLIC
int vsnprintf(char * s, size_t n, const char * format, va_list args) {
	struct print_state state = { s, n, 0, 0 };
	const int ret = internal_printf(snprintf_puts, &state, format, args); 
	if (ret < 0) { __exit(-1); }
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
int vasprintf(char ** strp, const char * format, va_list args) {
	const int ret = internal_printf(0, 0, format, args); 
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

	const int ret = internal_printf(vfprintf_puts, &state, format, args); 
	fwrite(state.buf, 1, state.to_write, state.file);
	if (ret < 0) { __exit(-1); }

	return ret;
}

