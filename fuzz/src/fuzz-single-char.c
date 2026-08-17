#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (size == 0) return 0;

    // Fuzz UTF-8 single character functions
    {
        char *s = (char *)data;
        char *end = s + size;
        while (s < end) {
            char *prev_s = s;
            _mulle_utf8_next_utf32character(&s);
            if (s <= prev_s) break; 
        }

        s = (char *)data + size;
        char *start = (char *)data;
        while (s > start) {
            char *prev_s = s;
            _mulle_utf8_previous_utf32character(&s);
            if (s >= prev_s) break;
        }

        struct mulle_utf8data data8 = mulle_utf8data_make((char *)data, size);
        while (mulle_utf8data_next_utf32character(&data8) != (mulle_utf32_t)-1) ;
    }

    // Fuzz UTF-16 single character functions
    if (size >= 2) {
        mulle_utf16_t *s = (mulle_utf16_t *)data;
        mulle_utf16_t *end = s + (size / 2);
        while (s < end) {
            mulle_utf16_t *prev_s = s;
            _mulle_utf16_next_utf32character(&s);
            if (s <= prev_s) break;
        }

        s = (mulle_utf16_t *)data + (size / 2);
        mulle_utf16_t *start = (mulle_utf16_t *)data;
        while (s > start) {
            mulle_utf16_t *prev_s = s;
            _mulle_utf16_previous_utf32character(&s);
            if (s >= prev_s) break;
        }
    }

    // Fuzz UTF-32 single character functions
    if (size >= 4) {
        mulle_utf32_t *s = (mulle_utf32_t *)data;
        mulle_utf32_t *end = s + (size / 4);
        while (s < end) {
            mulle_utf32_t *prev_s = s;
            _mulle_utf32_next_utf32character(&s);
            if (s <= prev_s) break;
        }

        s = (mulle_utf32_t *)data + (size / 4);
        mulle_utf32_t *start = (mulle_utf32_t *)data;
        while (s > start) {
            mulle_utf32_t *prev_s = s;
            _mulle_utf32_previous_utf32character(&s);
            if (s >= prev_s) break;
        }
    }

    return 0;
}
