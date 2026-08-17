#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    // Fuzz char7 encode/decode
    if (size <= mulle_char7_maxlength32 && mulle_char7_is_char7string32((char *)data, size)) {
        uint32_t encoded = mulle_char7_encode32((char *)data, size);
        char decoded[mulle_char7_maxlength32 + 1];
        size_t len = mulle_char7_decode32(encoded, decoded, mulle_char7_maxlength32);
        assert(len == size);
        assert(memcmp(data, decoded, size) == 0);
    }

    if (size <= mulle_char7_maxlength64 && mulle_char7_is_char7string64((char *)data, size)) {
        uint64_t encoded = mulle_char7_encode64((char *)data, size);
        char decoded[mulle_char7_maxlength64 + 1];
        size_t len = mulle_char7_decode64(encoded, decoded, mulle_char7_maxlength64);
        assert(len == size);
        assert(memcmp(data, decoded, size) == 0);
    }

    // Fuzz char5 encode/decode
    if (size <= mulle_char5_maxlength32 && mulle_char5_is_char5string32((char *)data, size)) {
        uint32_t encoded = mulle_char5_encode32((char *)data, size);
        char decoded[mulle_char5_maxlength32 + 1];
        size_t len = mulle_char5_decode32(encoded, decoded, mulle_char5_maxlength32);
        assert(len == size);
        assert(memcmp(data, decoded, size) == 0);
    }

    if (size <= mulle_char5_maxlength64 && mulle_char5_is_char5string64((char *)data, size)) {
        uint64_t encoded = mulle_char5_encode64((char *)data, size);
        char decoded[mulle_char5_maxlength64 + 1];
        size_t len = mulle_char5_decode64(encoded, decoded, mulle_char5_maxlength64);
        assert(len == size);
        assert(memcmp(data, decoded, size) == 0);
    }

    return 0;
}
