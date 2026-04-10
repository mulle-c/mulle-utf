## 5.2.0







feature: expose lookup table and add UTF‑32 → UTF‑16 conversion helpers

* expose `mulle_char5_lookup_table` as a public symbol so callers can use direct table lookups for encoding/decoding
* add `mulle_utf32_as_utf16` (inline) and `_mulle_utf32_as_utf16_not_ascii` helpers to convert UTF‑32 codepoints to UTF‑16 (including surrogate pair handling)
