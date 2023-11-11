#ifndef UNICODE_H
#define UNICODE_H
#ifdef __cplusplus
extern "C" {
#endif

int32_t utf8decode(const uint8_t* ptr, uintptr_t length, uint8_t* codelength, uint32_t* codepoint);
int32_t utf16decode(const uint16_t* ptr, uintptr_t length, uint8_t* codelength, uint32_t* codepoint);
int32_t utf8encode(uint32_t codepoint, uint8_t* length, uint8_t* encoded);
int32_t utf16encode(uint32_t codepoint, uint8_t* length, uint16_t* encoded);

int32_t utf8to16len(const char *inputorig, uintptr_t* length);
int32_t utf16to8len(const uint16_t *inputorig, uintptr_t* length);
int32_t utf8to16(const char *inputorig, uint16_t* outputorig);
int32_t utf16to8(const uint16_t *inputorig, void* outputorig);
#ifdef __cplusplus
}
#endif
#endif // UNICODE_H
