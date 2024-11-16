# mulle_char5_t

This is a encoding scheme that stashes strings that only consist of a subset
of ASCII characters into integers.
The actual subset is:

```
.ACDEINOPST_abcdefghilmnoprstuy
```

The values have been determined by analyzing 54674 identifiers that
would not fit into 64 bit `mulle_char7_t` and tabulating the most
popular characters.

The string "a.reason" would be transformed into a 64 bit integer like this:

``` c
   ( mulle_char5_encode_character( 'n') << (7 * 5)) |
   ( mulle_char5_encode_character( 'o') << (6 * 5)) |
   ( mulle_char5_encode_character( 's') << (5 * 5)) |
   ( mulle_char5_encode_character( 'a') << (4 * 5)) |
   ( mulle_char5_encode_character( 'e') << (3 * 5)) |
   ( mulle_char5_encode_character( 'r') << (2 * 5)) |
   ( mulle_char5_encode_character( '.') << (1 * 5)) |
   ( mulle_char5_encode_character( 'a') << (0 * 5))
```

On 32 bit systems the maximum string length is 6, on 64 bit systems it is 12.


## Functions


## mulle_char5_get

``` c
char  mulle_char5_get( mulle_char5_t value, size_t index);
```

Get character at `index`. If the index is out of range, the
returned value is 0.


### `mulle_char5_strlen`

``` c
size_t  mulle_char5_strlen( uintptr_t value)
```

Retrieve the string length of `value`.


### `mulle_char5_get_maxlength`

``` c
size_t  mulle_char5_get_maxlength( void)
```

Retrieve the maximum string length for `mulle_char5_t` on your system.


### `mulle_char5_substring`

``` c
mulle_char5_t  mulle_char5_substring( mulle_char5_t value,
                                      size_t location,
                                      size_t length)
```

Retrieve substring of `length` characters of `value` starting at `location`.
Range errors are ignored unless compiling with `asserts` enabled.


### `mulle_char5_is_char5string`

``` c
int   mulle_char5_is_char5string( char *src, size_t len)
```

Check if string `src` of length `len` can be encoded as `mulle_char5_t`.


### `mulle_char5_encode`

``` c
mulle_char5_t   mulle_char5_encode( char *src, size_t len)
```

Encode ASCII string `src` into `len`. '\0' will
be ignored. When asserts are enabled, this function does some sanity checks,
but it is the callers responsibility that character are 7 bit ASCII, that `len`
is correct and that the string does not contain \0.


### `mulle_char5_decode`

``` c
size_t  mulle_char5_decode( mulle_char5_t value, char *dst, size_t len)
```

Decode up to `len` characters of `value` into `dst`. The destination buffer must
be large enough to hold `len` characters. Returns the actual number of
characters. `dst` is not zero terminated!
