#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size % 2 != 0) return 0;

    mulle_utf16_t *src = (mulle_utf16_t *)data;
    size_t len = size / 2;

    struct mulle_utf_information info;
    mulle_utf16_information(src, len, &info);
    
    return 0;
}
