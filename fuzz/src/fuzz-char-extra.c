#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    // Fuzz char5 encode with UTF-16 source
    if (size >= 2) {
        mulle_utf16_t *utf16 = (mulle_utf16_t *)data;
        size_t len16 = size / 2;
        if (len16 <= mulle_char5_maxlength32) {
             mulle_char5_encode32_utf16(utf16, len16);
        }
        if (len16 <= mulle_char5_maxlength64) {
             mulle_char5_encode64_utf16(utf16, len16);
        }
    }

    // Fuzz char5 encode with UTF-32 source
    if (size >= 4) {
        mulle_utf32_t *utf32 = (mulle_utf32_t *)data;
        size_t len32 = size / 4;
        if (len32 <= mulle_char5_maxlength32) {
             mulle_char5_encode32_utf32(utf32, len32);
        }
        if (len32 <= mulle_char5_maxlength64) {
             mulle_char5_encode64_utf32(utf32, len32);
        }
    }

    // Same for char7
    if (size >= 2) {
        mulle_utf16_t *utf16 = (mulle_utf16_t *)data;
        size_t len16 = size / 2;
        if (len16 <= mulle_char7_maxlength32) {
             mulle_char7_encode32_utf16(utf16, len16);
        }
        if (len16 <= mulle_char7_maxlength64) {
             mulle_char7_encode64_utf16(utf16, len16);
        }
    }

    if (size >= 4) {
        mulle_utf32_t *utf32 = (mulle_utf32_t *)data;
        size_t len32 = size / 4;
        if (len32 <= mulle_char7_maxlength32) {
             mulle_char7_encode32_utf32(utf32, len32);
        }
        if (len32 <= mulle_char7_maxlength64) {
             mulle_char7_encode64_utf32(utf32, len32);
        }
    }

    return 0;
}
