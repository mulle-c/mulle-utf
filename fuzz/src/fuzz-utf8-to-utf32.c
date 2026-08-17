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

    // Convert to UTF-32
    mulle_utf32_t *utf32 = malloc((info.utf32len + 1) * sizeof(mulle_utf32_t));
    if (!utf32) return 0;
    
    mulle_utf32_t *utf32_end = _mulle_utf8_convert_to_utf32((char *)info.start, info.utf8len, utf32);
    assert((size_t)(utf32_end - utf32) == info.utf32len);
    
    // Convert back to UTF-8
    size_t utf8_back_len = mulle_utf32_utf8length(utf32, info.utf32len);
    assert(utf8_back_len == info.utf8len);
    
    char *utf8_back = malloc(utf8_back_len + 1);
    if (!utf8_back) {
        free(utf32);
        return 0;
    }
    
    char *utf8_back_end = _mulle_utf32_convert_to_utf8(utf32, info.utf32len, utf8_back);
    assert((size_t)(utf8_back_end - utf8_back) == utf8_back_len);
    
    assert(memcmp(info.start, utf8_back, utf8_back_len) == 0);
    
    free(utf8_back);
    free(utf32);
    return 0;
}
