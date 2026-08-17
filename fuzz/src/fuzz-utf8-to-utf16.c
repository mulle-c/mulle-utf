#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    struct mulle_utf_information info;
    if (mulle_utf8_information((char *)data, size, &info) != 0) {
        return 0;
    }

    // Convert to UTF-16
    mulle_utf16_t *utf16 = malloc((info.utf16len + 1) * sizeof(mulle_utf16_t));
    if (!utf16) return 0;
    
    mulle_utf16_t *utf16_end = _mulle_utf8_convert_to_utf16((char *)info.start, info.utf8len, utf16);
    assert((size_t)(utf16_end - utf16) == info.utf16len);
    
    // Convert back to UTF-8
    size_t utf8_back_len = mulle_utf16_utf8length(utf16, info.utf16len);
    assert(utf8_back_len == info.utf8len);
    
    char *utf8_back = malloc(utf8_back_len + 1);
    if (!utf8_back) {
        free(utf16);
        return 0;
    }
    
    char *utf8_back_end = _mulle_utf16_convert_to_utf8(utf16, info.utf16len, utf8_back);
    assert((size_t)(utf8_back_end - utf8_back) == utf8_back_len);
    
    assert(memcmp(info.start, utf8_back, utf8_back_len) == 0);
    
    free(utf8_back);
    free(utf16);
    return 0;
}
