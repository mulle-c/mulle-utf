# String

mulle-utf provides `<string.h>`-like operations for UTF-8, UTF-16 and UTF-32
strings. All `len` parameters are in code units (bytes for UTF-8, `mulle_utf16_t`
for UTF-16, `mulle_utf32_t` for UTF-32). Zero-terminated strings are expected
unless an explicit length is given.

> BOM markers are just part of the UTF string and not specially treated by these
> functions.


## UTF-8 string functions

``` c
size_t   mulle_utf8_strlen( char *s);
size_t   mulle_utf8_strnlen( char *s, size_t len);
char    *mulle_utf8_strncpy( char *dst, size_t len, char *src);
char    *mulle_utf8_strnstr( char *s, size_t len, char *search);
char    *mulle_utf8_strstr( char *s, char *search);
char    *mulle_utf8_strnchr( char *s, size_t len, mulle_utf32_t c);
char    *mulle_utf8_strchr( char *s, mulle_utf32_t c);
size_t   mulle_utf8_strspn( char *s, char *search);
size_t   mulle_utf8_strcspn( char *s, char *search);
char    *mulle_utf8_skiputf32( char *s, size_t *p_n);
```

Notes:

* `mulle_utf8_strchr` and `mulle_utf8_strnchr` accept a `mulle_utf32_t`
  character — they handle multi-byte matching internally.
* `mulle_utf8_strncpy` only writes the string and a terminator; it does **not**
  zero-fill the remaining buffer like `strncpy`.
* `mulle_utf8_skiputf32` advances `s` past `*p_n` UTF-32 characters and
  updates `*p_n` to the number actually skipped.


## UTF-16 string functions

``` c
size_t         mulle_utf16_strlen( mulle_utf16_t *s);
size_t         mulle_utf16_strnlen( mulle_utf16_t *src, size_t len);
mulle_utf16_t *mulle_utf16_strdup( mulle_utf16_t *s);
mulle_utf16_t *mulle_utf16_strncpy( mulle_utf16_t *dst, size_t len, mulle_utf16_t *src);
int            mulle_utf16_strncmp( mulle_utf16_t *s1, mulle_utf16_t *s2, size_t len);
int            mulle_utf16_strcmp( mulle_utf16_t *s1, mulle_utf16_t *s2);
mulle_utf16_t *mulle_utf16_strchr( mulle_utf16_t *s, mulle_utf32_t c);
mulle_utf16_t *mulle_utf16_strstr( mulle_utf16_t *s1, mulle_utf16_t *s2);
size_t         mulle_utf16_strspn( mulle_utf16_t *s1, mulle_utf16_t *s2);
size_t         mulle_utf16_strcspn( mulle_utf16_t *s1, mulle_utf16_t *s2);
int            mulle_utf16_atoi( mulle_utf16_t *s);
void           mulle_utf16_memcpy( mulle_utf16_t *dst, mulle_utf16_t *src, size_t len);
void           mulle_utf16_memmove( mulle_utf16_t *dst, mulle_utf16_t *src, size_t len);
```


## UTF-32 string functions

``` c
size_t         mulle_utf32_strlen( mulle_utf32_t *s);
size_t         mulle_utf32_strnlen( mulle_utf32_t *src, size_t len);
mulle_utf32_t *mulle_utf32_strdup( mulle_utf32_t *s);
mulle_utf32_t *mulle_utf32_strncpy( mulle_utf32_t *dst, size_t len, mulle_utf32_t *src);
int            mulle_utf32_strncmp( mulle_utf32_t *s1, mulle_utf32_t *s2, size_t len);
int            mulle_utf32_strcmp( mulle_utf32_t *s1, mulle_utf32_t *s2);
mulle_utf32_t *mulle_utf32_strchr( mulle_utf32_t *s, mulle_utf32_t c);
mulle_utf32_t *mulle_utf32_strstr( mulle_utf32_t *s1, mulle_utf32_t *s2);
size_t         mulle_utf32_strspn( mulle_utf32_t *s1, mulle_utf32_t *s2);
size_t         mulle_utf32_strcspn( mulle_utf32_t *s1, mulle_utf32_t *s2);
int            mulle_utf32_atoi( mulle_utf32_t *s);
void           mulle_utf32_memcpy( mulle_utf32_t *dst, mulle_utf32_t *src, size_t len);
void           mulle_utf32_memmove( mulle_utf32_t *dst, mulle_utf32_t *src, size_t len);
```

Notes:

* `strcmp` is a codepoint comparison, not locale-aware collation.
* `atoi` only works for ASCII-compatible digits.
* `memcpy` / `memmove` take element counts, not byte counts.
