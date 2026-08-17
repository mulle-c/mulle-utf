#include "mulle-utf.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    struct mulle_utf_rover rover;
    mulle_utf32_t c;

    // Fuzz UTF-8 rover
    _mulle_utf8_rover_init(&rover, (char *)data, size);
    while (_mulle_utf_rover_has_character(&rover)) {
        c = _mulle_utf_rover_next_character(&rover);
        // We could also test dialback occasionally
        if (size > 0 && data[0] % 10 == 0) {
            _mulle_utf_rover_dial_back(&rover);
            // Must not loop infinitely, so we only do it once or based on some state
            // But for fuzzing, just testing the call doesn't crash is good.
            // To be safe, we'd need to ensure progress or limit dialbacks.
            break; 
        }
    }

    // Fuzz UTF-16 rover
    if (size >= 2) {
        _mulle_utf16_rover_init(&rover, (mulle_utf16_t *)data, size / 2);
        while (_mulle_utf_rover_has_character(&rover)) {
            c = _mulle_utf_rover_next_character(&rover);
        }
    }

    // Fuzz UTF-32 rover
    if (size >= 4) {
        _mulle_utf32_rover_init(&rover, (mulle_utf32_t *)data, size / 4);
        while (_mulle_utf_rover_has_character(&rover)) {
            c = _mulle_utf_rover_next_character(&rover);
        }
    }

    return 0;
}
