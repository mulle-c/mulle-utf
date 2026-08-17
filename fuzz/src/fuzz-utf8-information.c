#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    struct mulle_utf_information info;
    int rval;

    rval = mulle_utf8_information((char *)data, size, &info);
    
    if (rval == 0) {
        assert(mulle_utf_information_is_valid(&info));
        
        // Cross-check with other functions that assume valid input
        size_t utf16len = mulle_utf8_utf16length((char *)info.start, info.utf8len);
        assert(utf16len == info.utf16len);
        
        size_t utf32len = mulle_utf8_utf32length((char *)info.start, info.utf8len);
        assert(utf32len == info.utf32len);

        // Walk through characters and verify count
        char *s = (char *)info.start;
        char *sentinel = s + info.utf8len;
        size_t count = 0;
        while (s < sentinel) {
            mulle_utf32_t c = mulle_utf8_next_utf32character(&s);
            assert(c >= 0);
            count++;
        }
        assert(count == info.utf32len);
    } else {
        assert(!mulle_utf_information_is_valid(&info));
        assert(info.invalid >= (void *)data && info.invalid < (void *)(data + size));
    }

    // Also test is_ascii separately as it has its own implementation
    int is_ascii = mulle_utf8_is_ascii((char *)data, size);
    if (is_ascii) {
        // If it's ascii, mulle_utf8_information should generally agree 
        // (but it might have a BOM which makes it NOT ascii in its view)
        if (size > 0 && !mulle_utf8_has_leading_bomcharacter((char *)data, size)) {
             // mulle_utf8_information should return 0 and set is_ascii=1
             struct mulle_utf_information info2;
             if (mulle_utf8_information((char *)data, size, &info2) == 0) {
                 assert(info2.is_ascii);
             }
        }
    }

    return 0;
}
