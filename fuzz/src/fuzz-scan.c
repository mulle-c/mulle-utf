#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    // We use the input as a potential decimal string.
    // If it's valid ASCII, we can convert it to UTF-16 and UTF-32 and
    // ensure all three scanners return the same result.

    char *utf8_src = malloc(size + 1);
    if (!utf8_src) return 0;
    memcpy(utf8_src, data, size);
    utf8_src[size] = '\0';

    int is_ascii = 1;
    for (size_t i = 0; i < size; i++) {
        if (data[i] > 127) {
            is_ascii = 0;
            break;
        }
    }

    if (is_ascii) {
        long long val8 = 0, val16 = 0, val32 = 0;
        char *p8 = utf8_src;
        int r8 = _mulle_utf8_scan_longlong_decimal(&p8, size, &val8);

        mulle_utf16_t *utf16_src = malloc((size + 1) * sizeof(mulle_utf16_t));
        if (utf16_src) {
            for (size_t i = 0; i <= size; i++) utf16_src[i] = utf8_src[i];
            mulle_utf16_t *p16 = utf16_src;
            int r16 = _mulle_utf16_scan_longlong_decimal(&p16, size, &val16);
            assert(r8 == r16);
            if (r8 >= 0) assert(val8 == val16);
            free(utf16_src);
        }

        mulle_utf32_t *utf32_src = malloc((size + 1) * sizeof(mulle_utf32_t));
        if (utf32_src) {
            for (size_t i = 0; i <= size; i++) utf32_src[i] = utf8_src[i];
            mulle_utf32_t *p32 = utf32_src;
            int r32 = _mulle_utf32_scan_longlong_decimal(&p32, size, &val32);
            assert(r8 == r32);
            if (r8 >= 0) assert(val8 == val32);
            free(utf32_src);
        }
    } else {
        // Just fuzz the scanners individually to catch crashes
        long long val = 0;
        char *p8 = utf8_src;
        _mulle_utf8_scan_longlong_decimal(&p8, size, &val);
        
        if (size >= 2) {
            mulle_utf16_t *p16 = (mulle_utf16_t *)data;
            _mulle_utf16_scan_longlong_decimal(&p16, size / 2, &val);
        }
        if (size >= 4) {
            mulle_utf32_t *p32 = (mulle_utf32_t *)data;
            _mulle_utf32_scan_longlong_decimal(&p32, size / 4, &val);
        }
    }

    free(utf8_src);
    return 0;
}
