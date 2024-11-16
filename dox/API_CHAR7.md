# mulle_char7_t

This is a scheme that encodes strings contsisting only of 7 bit ASCII
characters into integers. The string "VfL 1848" will be transformed into a
64 bit integer like this:

``` c
   ('8' << (7 * 7)) |
   ('4' << (6 * 7)) |
   ('8' << (5 * 7)) |
   ('1' << (4 * 7)) |
   (' ' << (3 * 7)) |
   ('L' << (2 * 7)) |
   ('f' << (1 * 7)) |
   ('V' << (0 * 7))
```

On 32 bit systems the maximum encodable string length is 4 on 64 bit systems
it is 8.


## Functions

## mulle_char7_get

``` c
char  mulle_char7_get( mulle_char7_t value, size_t index);
```

Get character at `index`. If the index is out of range, the returned value is
0.


### `mulle_char7_strlen`

``` c
size_t  mulle_char7_strlen( uintptr_t value)
```

Retrieve the string length of `value`.


### `mulle_char7_get_maxlength`

``` c
size_t  mulle_char7_get_maxlength( void)
```

Retrieve the maximum string length for `mulle_char7_t` on your system.


### `mulle_char7_substring`

``` c
mulle_char7_t  mulle_char7_substring( mulle_char7_t value, size_t location, size_t length)
```

Retrieve substring of `length` charactes of `value` starting at `location`.
Range errors are ignored unless compiling with `asserts` enabled.


### `mulle_char7_is_char7string`

``` c
int   mulle_char7_is_char7string( char *src, size_t len)
```

Check if string `src` of length `len` can be encoded as `mulle_char7_t`.


### `mulle_char7_encode`

``` c
mulle_char7_t   mulle_char7_encode( char *src, size_t len)
```

Encode ASCII string `src` into `len`. '\0' will
be ignored. When asserts are enabled, this function does some sanity checks,
but it is the callers responsibility that character are 7 bit ASCII, that `len`
is correct and that the string does not contain \0.


### `mulle_char7_decode`

``` c
size_t  mulle_char7_decode( mulle_char7_t value, char *dst, size_t len)
```

Decode up to `len` characters of `value` into `dst`. The destination buffer must
be large enough to hold `len` characters. Returns the actual number of
characters. `dst` is not zero terminated!
