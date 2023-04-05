## Encode strings as integers

* **mulle_char5** is a compression scheme, that uses a set of 31 characters
  to encode strings into integers of varying sizes. This can be useful for small
  strings with typical Objective-C "keys" and small common mostly lowercase words.
* **mulle_char7** is a compression scheme placing 7 bit ASCII characters into
  integer numbers of varying sizes. A 64 bit integer can hold up to 8 characters.

> Naming: UTF is a transfer encoding for Unicode. So everything
> eventually maps to (32 bit) unicode characters. That operations are done on
> UTF directly is kinda questionable, but I believe common.

