#include <base/types.h>
#include <errno.h>

int32_t utf8decode(const char* ptr, uintptr_t length, uint8_t* codelength, uint32_t* codepoint) {
    if (length == 0 || ptr == 0) {
        return EINVAL;
    }

    unsigned char first_byte = (unsigned char)*ptr;
    unsigned char mask;
    uintptr_t bytes_to_read;

    if ((first_byte & 0x80) == 0) {
        // Single-byte UTF-8 character
        *codepoint = first_byte;
        *codelength = 1;
        return SUCCESS;
    } else if ((first_byte & 0xE0) == 0xC0) {
        // Two-byte UTF-8 character
        mask = 0x1F;
        bytes_to_read = 2;
    } else if ((first_byte & 0xF0) == 0xE0) {
        // Three-byte UTF-8 character
        mask = 0x0F;
        bytes_to_read = 3;
    } else if ((first_byte & 0xF8) == 0xF0) {
        // Four-byte UTF-8 character
        mask = 0x07;
        bytes_to_read = 4;
    } else {
        // Invalid UTF-8 encoding
        return EINVAL;
    }

    // Check if we have enough characters to read the codepoint
    if (length < bytes_to_read) {
        return EINVAL;
    }

    // Decode the codepoint from the UTF-8 sequence
    *codepoint = first_byte & mask;
    for (uintptr_t i = 1; i < bytes_to_read; i++) {
        if ((ptr[i] & 0xC0) != 0x80) {
            // Invalid UTF-8 sequence
            return EINVAL;
        }
        *codepoint = (*codepoint << 6) | (ptr[i] & 0x3F);
    }

    // Set the codelength to the number of bytes read for the codepoint
    *codelength = bytes_to_read;

    return SUCCESS;
}

int32_t utf16decode(const uint16_t* ptr, uintptr_t length, uint8_t* codelength, uint32_t* codepoint) {
    if (length == 0 || ptr == 0) {
        // Invalid input: NULL pointer or length is 0
        return EINVAL;
    }

    uint16_t first_word = *ptr;

    if ((first_word & 0xFC00) == 0xD800) {
        // High surrogate detected (first word is in the range U+D800 to U+DBFF)
        if (length < 2) {
            // Not enough characters to decode a complete surrogate pair
            return EINVAL;
        }

        uint16_t second_word = *(ptr + 1);
        if ((second_word & 0xFC00) != 0xDC00) {
            // Invalid low surrogate (second word is not in the range U+DC00 to U+DFFF)
            return EINVAL;
        }

        // Calculate the codepoint from the surrogate pair
        *codepoint = 0x10000 + (((uint32_t)(first_word & 0x03FF) << 10) | (second_word & 0x03FF));
        *codelength = 2;
    } else {
        // Single-word UTF-16 character (not a surrogate pair)
        *codepoint = first_word;
        *codelength = 1;
    }

    return SUCCESS;
}

int32_t utf8encode(uint32_t codepoint, uint8_t* length, char* encoded) {
    if (codepoint <= 0x7F) {
        // Single-byte UTF-8 character (ASCII range)
        *length = 1;
        encoded[0] = codepoint & 0x7F;
    } else if (codepoint <= 0x7FF) {
        // Two-byte UTF-8 character
        *length = 2;
        encoded[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
        encoded[1] = 0x80 | (codepoint & 0x3F);
    } else if (codepoint <= 0xFFFF) {
        // Three-byte UTF-8 character
        *length = 3;
        encoded[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
        encoded[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        encoded[2] = 0x80 | (codepoint & 0x3F);
    } else if (codepoint <= 0x10FFFF) {
        // Four-byte UTF-8 character
        *length = 4;
        encoded[0] = 0xF0 | ((codepoint >> 18) & 0x07);
        encoded[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        encoded[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        encoded[3] = 0x80 | (codepoint & 0x3F);
    } else {
        *length = 0;
	return EINVAL;
    }
    return SUCCESS;
}

int32_t utf16encode(uint32_t codepoint, uint8_t* length, uint16_t* encoded) {
    if (codepoint <= 0xFFFF) {
        // Single-word UTF-16 character (BMP range)
        *length = 1;
        encoded[0] = codepoint & 0xFFFF;
    } else if (codepoint <= 0x10FFFF) {
        // Surrogate pair for codepoints beyond the BMP
        *length = 2;
        codepoint -= 0x10000;
        encoded[0] = 0xD800 | ((codepoint >> 10) & 0x3FF);
        encoded[1] = 0xDC00 | (codepoint & 0x3FF);
    } else {
        // Invalid Unicode codepoint
        *length = 0;
	return EINVAL;
    }
    return SUCCESS;
}

int32_t utf8to16(const char *inputOrig, uint16_t* outputOrig) {
	int32_t error;
	const char* input = inputOrig;
	uint16_t* output = outputOrig;
	uint32_t codepoint;
	do {
		uint8_t utf8len;
		error = utf8decode(input, 4, &utf8len, &codepoint);
		if (error != SUCCESS) { return error; }

		if (codepoint == '/') { codepoint = '\\'; }

		uint16_t utf16[2];
		uint8_t utf16len;
		error = utf16encode(codepoint, &utf16len, utf16);
		if (error != SUCCESS) { return error; }

		for (int i = 0; i < utf16len; i++) {
			output[i] = utf16[i];
		}
		output += utf16len;
		input += utf8len;
	} while (codepoint != 0); // utf8 '\0'
	return SUCCESS;
}

int32_t utf16to8(const uint16_t *inputOrig, void* outputOrig) {
	int32_t error;
	const uint16_t * input = inputOrig;
	char* output = outputOrig;
	uint32_t codepoint;
	do {
		uint8_t utf16len;
		error = utf16decode(input, 4, &utf16len, &codepoint);
		if (error != SUCCESS) { return error; }

		if (codepoint == '/') { codepoint = '\\'; }

		char utf8[4];
		uint8_t utf8len;
		error = utf8encode(codepoint, &utf8len, utf8);
		if (error != SUCCESS) { return error; }

		for (int i = 0; i < utf8len; i++) {
			output[i] = utf8[i];
		}
		output += utf16len;
		input += utf8len;
	} while (codepoint != 0); // utf8 '\0'
	return SUCCESS;
}

int32_t utf8to16len(const char *inputOrig, uintptr_t* length) {
	const char* input = inputOrig;
	uint32_t len = 0;
	uint32_t codepoint;
	do {
		uint8_t utf8len;
		int32_t error = utf8decode(input, 4, &utf8len, &codepoint);
		if (error != SUCCESS) { return error; }

		uint16_t utf16[2];
		uint8_t utf16len;
		error = utf16encode(codepoint, &utf16len, utf16);
		if (error != SUCCESS) { return error; }

		len += utf16len;
		input += utf8len;
	} while (codepoint != 0); // utf8 '\0'
	*length = len;
	return SUCCESS;
}

int32_t utf16to8len(const uint16_t *inputOrig, uintptr_t* length) {
	const uint16_t* input = inputOrig;
	uint32_t len = 0;
	uint32_t codepoint;
	do {
		uint8_t utf16len;
		int32_t error = utf16decode(input, 2, &utf16len, &codepoint);
		if (error != SUCCESS) { return error; }

		char utf8[4];
		uint8_t utf8len;
		error = utf8encode(codepoint, &utf8len, utf8);
		if (error != SUCCESS) { return error; }

		len += utf8len;
		input += utf16len;
	} while (codepoint != 0); // utf8 '\0'
	*length = len;
	return SUCCESS;
}

