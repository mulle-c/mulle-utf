# information

The mulle-utf information functions uniformly analyze a string of `len`
characters and fill a struct value:

```
struct mulle_utf_information
{
   size_t   utf8len;
   size_t   utf16len;
   size_t   utf32len;
   void     *start;          // behind BOM if bommed, otherwise start
   void     *invalid;        // first fail char
   int      has_bom;
   int      is_ascii;
   int      is_char5;
   int      is_utf15;
   int      has_terminating_zero;
};
```

Field        | Description
-------------|---------------------------------------------------------
utf8len      | Length of string if encoded as UTF8 without a BOM
utf16len     | Length of string if encoded as UTF16 without a BOM
utf32len     | Length of string if encoded as UTF32 without a BOM
start        | Start of string, past a possibly leading BOM
invalid      | The character, that makes the given string invalid UTF
has_bom      | Set to one, if the string has a leading BOM
is_ascii     | Set to one, if the string is compatble with ASCII (7 bit)
is_char5     | Set to one, if the string is compatble with CHAR5 (5 bit)
is_utf15     | Set to one, if the string is compatble with a 15 bit subset of UTF16
has_terminating_zero | If the string is zero terminated


# Functions

The information functions always skip the BOM.  If you use
`mulle_utf32_information` and friends to figure out the lengths of
strings in various decodings its easy to make this error:

```
   ...
   mulle_utf32_information( s, len, &info);
   ...
   mulle_utf32_bufferconvert_to_utf16( s,
                                       info.utf32len,
                                       &buffer,
                                       (void *) mulle_buffer_add_bytes);
```

It must be coded as:

```
   mulle_utf32_information( s, len, &info);
   ...
   mulle_utf32_bufferconvert_to_utf16( info.start, // !!!
                                       info.utf32len
                                       &buffer,
                                       (void *) mulle_buffer_add_bytes);
```

## `mulle_utf8_information` et al.

```
int  mulle_utf8_information( mulle_utf8_t *s, size_t len, struct mulle_utf_information *info);
int  mulle_utf16_information( mulle_utf8_t *s, size_t len, struct mulle_utf_information *info);
int  mulle_utf132_information( mulle_utf8_t *s, size_t len, struct mulle_utf_information *info);
```

Returns 0 if successful, otherwise info->invalid points to the offending
character.

* If `len` is zero, then `s` may be NULL.
* These functions always preempt on zero chars. "ha\0ha" will have a string length
of 2 in any encoding.
* UTF16 surrogate pair character codes in UTF8 or UTF32 are considered to be invalid



