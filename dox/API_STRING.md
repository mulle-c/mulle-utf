# String manipulations

`mulle_utf` provides some low-level string manipulations for UTF16
and UTF32 strings similiar to those that are available for C strings
in `<string.h>`

The actual functions either have a `mulle_utf16_` prefix for UTF16 or `mulle_utf32_`for UTF32.

> BOM markers are just part of the UTF string and not specially treated.


## Function Suffixes

* atoi        // only useful for digits compatible to ASCII encoding
* strchr
* strcmp
* strcspn
* strlen
* strncpy
* strstr
