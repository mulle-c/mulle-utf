# 6.0.0







* `mulle_utf16_validate` no longer reads past the end of the buffer when a string ends in a high surrogate
* `mulle_utf32_is_surrogatecharacter` no longer misclassifies U+E000 as a surrogate
* `mulle_utf16_information` utf8len no longer overcounts astral characters by one byte
* `mulle_utf32_utf16length` counts U+FFFF as a single UTF-16 unit instead of two
* `_mulle_utf8_convert_to_iso1` now actually decodes two-byte sequences (U+0080..U+00FF)
* `mulle_utf8_strnstr` finds overlapping matches instead of skipping past them
* `mulle_utf8data_range_of_utf32_range` now handles multi-byte characters correctly
* `mulle_utf8_utf16length` signals truncation with -1 instead of silently returning 0
* embedded NUL is treated as a valid terminator by utf8/utf16/utf32 validate
* `mulle_char5_is_char5string32/64` reject strings containing zero bytes
* **BREAKING** remove `mulle_utf16_length`


### 5.2.1

Various small improvements
