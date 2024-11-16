# 5.0.0

feat: improve UTF string handling and `size_t` compatibility

* Enhance UTF string conversion and buffer handling
  - Improve buffer conversion efficiency by batching character adds
  - Add `mulle_utf8data` introspection capabilities
  - Optimize UTF32 to UTF8 conversion paths

* Strengthen type safety and consistency
  - Replace unsigned int with `size_t` for string lengths
  - Change `mulle_utf8_t` to char for better C compatibility
  - Update function signatures to use `size_t` consistently

* Breaking changes
  - Rename `mulle_unicode_is_privatecharacterplane` to `mulle_utf_is_privatecharacterplane`
  - Change parameter order in `_strcnpy` functions
  - Update `mulle_utf_add_bytes_function_t` typedef
  - Move stdlib-like code to mulle-utf8-string.c

* Other improvements
  - Increase test coverage significantly
  - Fix 32-bit compatibility issues
  - Update CMake configuration for better Windows support


# 4.0.0

* upped the test coverage significantly
* **BREAKING CHANGE** the `mulle_utf_add_bytes_function_t` typedef is no longer a function pointer, to keep it more inline with other library callback typedefs
* **BREAKING CHANGE** changed the parameter order of `_strcnpy` functions, because progress demands it
* **BREAKING CHANGE** renamed `mulle_unicode_is_privatecharacterplane` to `mulle_utf_is_privatecharacterplane` (but MulleObjCUnicodeFoundation resupplies it as a static inline function)
* **BREAKING CHANGE** uses now ``size_t`` for char (byte) lengths and unsigned int for anything else
* `mulle_utf8_t` is no more, it is now just char
* improve `mulle_utf32_as_utf8` conversion, add some `mulle_utf8data` introspection
* improve buffer conversion by not calling addbytes for each character


### 3.1.3

* remove package.json as it conflicts with clib.json

### 3.1.2

* Various small improvements

### 3.1.1

* GLOBAL change for Windows
* fix GLOBAL for Windows

## 3.1.0

* added iso, nextstep, macroman converter


### 3.0.1

* added  function

# 3.0.0

* changed `struct mulle_utf8_data` to `struct mulle_utf8data` (and other related datas as well)
* add some `mulle_ascii` conversion routines
* add convenience `mulle_utfxx_data_make` functions
* added `mulle_utfxx_charinfo` functions
* coded an alternative strlen function for char5 and char7
* added `mulle_char5_encode32_utf16`, `mulle_char5_encode32_utf32`, `mulle_char7_encode32_utf16`, `mulle_char7_encode64_utf16`
* added `mulle_char5_next` and `mulle_char7_next` for faster isEqual:
* added `_mulle_utf16_character_mogrify_unsafe` and `_mulle_utf16_character_mogrify` for faster lowercaseString
* added `_mulle_utf16_word_mogrify` for completeness
* speed up various operations
* add routines to char5 and char7 to encode to UTF16 and UTF32
* rewrote conversion routines with changes in the API  and added lowleve routines
* tolower/toupper support code renamed from "convert" to "mogrify" to distinguis from UTF conversion
* added `mulle_utf32_data` and `mulle_utf16_data` structs


### 2.3.1

* get privatecharacter back from mulle-unicode

## 2.3.0

* split off ctype style code to mulle-unicode, because its pretty large and not that much used


## 2.2.0

* added ``mulle_utf8_strnstr`,` ``mulle_utf8_strnchr`,` ``mulle_utf8_strnspn`,` ``mulle_utf8_strncspn``
* add rover functionality to step through UTF strings
* added a longlong ascii conversion


## 2.1.0

* added `mulle_utf…_nop` functions for nop callbacks
* added `_mulle_utf32_as_utf8` for quick conversions w/o error checks
* struct `mulle_utf8data` added
* updated to Unicode 12.1
* added legal characterset, though its pretty large
* fixed a lot of bugs with respect to charactersets


### 2.0.11

* modernized mulle-sde
* Added a check for NULL pointer input

### 2.0.10

* modernized to mulle-sde with .mulle folder

### 2.0.9

* use memmove in a strategic place

### 2.0.8

* upgrade to newest mulle-sde

### 2.0.7

* fix a linux warning about strnlen the hard way

### 2.0.6

* remove obsolete file

### 2.0.5

* fix mingw, update sde

### 2.0.4

* Various small improvements

### 2.0.3

* modernized mulle-sde

### 2.0.2

* fix missing eval

### 2.0.1

* fix travis.yml

# 2.0.0

* migrated to mulle-sde
* made headernames hyphenated
* no longer distributed as a homebrew package


### 1.1.13

* migrate to mulle-project
* fix glaring regression in mulle_utf8_information

### 1.1.11

* don't use failing extrachar function in information routine

### 1.1.9

* support new mulle-tests

### 1.1.7

* fixed scion wrapper command

### 1.1.5

* follow mulle-configuration 3.1 changes and move .travis.yml to trusty

### 1.1.3

* fixes for _WIN32

### 1.0.13

* make it a cmake "C" project


### 1.0.11

* modernize project

1.0.9
===

* improve documentation
* fix homebrew formula generation

1.0.7
===

* improve documentation
* consistent errno return value for bufferconvert


1.0.5
===

* improve documentation
* reintroduce bom16 functions

1.0.3
===

* add travis.yml

1.0.1
===

* changed the char5 characterset to match typical Objective-C identifiers
better. Need to update the compiler too.
* redid the API to be more consistent with other projects

0.5.1
===

* improve the documentation somewhat

0.5
===

* incorporated some code from `mulle_regex`, namely those `_string` functions.
* add dependency to `mulle_allocator` for convenience methods
* changed parameter order in bytebuffer conversion functions!
* allow len to be passed as -1 to `mulle_utf<>_information` for convenience.


0.4
===

* Unify all utf8/16/32_info structs into one `mulle_utf_information`.
* added mulle_utf_is_validurlscheme.h


0.3
===

* add routines to iterate over utf8/utf16/utf32 forwards and (!) backwards
* fixed some bugs with respect to surrogate asserts in utf32


0.2
===

* removed BOM check from most routines. It is now expected that you strip
  the BOM away yourself
* added mulle_char5 encoding
* surrogate pairs are considered non-characters in utf32 encoding


0.1
===

* started versioning

* changed the callback scheme to a uniform `add`. This is nice,
  because by default the utf routines will use host order. If you want
  to have flipped output just change the `add`routine.

* renamed mulle_utf8char_t and friends to mulle_utf8_t
