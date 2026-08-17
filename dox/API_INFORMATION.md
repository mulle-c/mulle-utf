# Information

The mulle-utf information functions analyze a string of `len` characters in a
single pass and fill a struct describing the string's properties and lengths in
all three encodings:

``` c
struct mulle_utf_information
{
   size_t   utf8len;
   size_t   utf16len;
   size_t   utf32len;
   void     *start;
   void     *invalid;
   int      has_bom;
   int      is_ascii;
   int      is_char5;
   int      is_utf15;
   int      has_terminating_zero;
};
```

| Field                  | Description                                                    |
|------------------------|----------------------------------------------------------------|
| `utf8len`              | Length in UTF-8 code units (bytes), excluding BOM              |
| `utf16len`             | Length in UTF-16 code units, excluding BOM                     |
| `utf32len`             | Length in UTF-32 code units (characters), excluding BOM        |
| `start`                | Start of string data, past a leading BOM if present            |
| `invalid`              | Pointer to the first invalid character, or NULL if valid       |
| `has_bom`              | 1 if the string has a leading BOM                              |
| `is_ascii`             | 1 if the string is pure ASCII (7-bit)                          |
| `is_char5`             | 1 if the string is compatible with mulle_char5 (5-bit subset)  |
| `is_utf15`             | 1 if the string fits in a 15-bit subset of UTF-16              |
| `has_terminating_zero` | 1 if a zero character was encountered (used as terminator)     |


## Functions

``` c
int  mulle_utf8_information( char *s, size_t len, struct mulle_utf_information *info);
int  mulle_utf16_information( mulle_utf16_t *s, size_t len, struct mulle_utf_information *info);
int  mulle_utf32_information( mulle_utf32_t *s, size_t len, struct mulle_utf_information *info);
```

Returns 0 if the string is valid, -1 otherwise. On failure, `info->invalid`
points to the offending character.

### Behaviour

* If `len` is 0, then `s` may be NULL.
* A zero character within the string acts as a terminator — `"ha\0ha"` with
  `len` 5 has a string length of 2 in any encoding and `has_terminating_zero`
  is set to 1.
* The BOM is skipped: `start` points past it and the lengths exclude it.
* UTF-16 surrogate code points in a UTF-8 or UTF-32 string are always invalid.
* Noncharacters (U+FDD0..U+FDEF, U+xFFFE, U+xFFFF) are currently rejected.


### Common pattern

Use `information` to validate and measure, then convert using `start` and
the appropriate length:

``` c
   struct mulle_utf_information  info;

   if( mulle_utf32_information( s, len, &info))
      handle_error( info.invalid);

   // CORRECT: use info.start (skips BOM) and info.utf32len
   mulle_utf32_bufferconvert_to_utf16( info.start,
                                       info.utf32len,
                                       &buffer,
                                       mulle_buffer_add_bytes);
```

Do **not** pass the original `s` and `len` to the converter after using
`information` — if the string had a BOM, you'd convert the BOM too.


## Validation

``` c
char          *mulle_utf8_validate( char *src, size_t len);
mulle_utf16_t *mulle_utf16_validate( mulle_utf16_t *src, size_t len);
mulle_utf32_t *mulle_utf32_validate( mulle_utf32_t *src, size_t len);
```

Returns NULL if the string is valid, otherwise a pointer to the first invalid
character. A zero character within `len` is treated as a terminator (valid).

`validate` and `information` agree on what constitutes valid input.


## Length functions

``` c
size_t  mulle_utf8_utf16length( char *src, size_t len);
size_t  mulle_utf8_utf32length( char *src, size_t len);
size_t  mulle_utf16_utf8length( mulle_utf16_t *src, size_t len);
size_t  mulle_utf16_utf32length( mulle_utf16_t *src, size_t len);
size_t  mulle_utf32_utf8length( mulle_utf32_t *src, size_t len);
size_t  mulle_utf32_utf16length( mulle_utf32_t *src, size_t len);
```

Return the number of code units the string would occupy in the target encoding.
On invalid input, returns `(size_t) -1`.
