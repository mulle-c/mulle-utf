# mulle-utf

#### 🔤 UTF8-16-32 analysis and manipulation library

a C (C99) library to analyze and convert unicode strings. It also contains some
limited `<string.h>` like functionality for UTF16 and UTF32.

It is the backbone of **NSString**.


| Release Version
|-----------------------------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-c/mulle-utf.svg?branch=release) [![Build Status](https://github.com/mulle-c/mulle-utf/workflows/CI/badge.svg?branch=release)](//github.com/mulle-c/mulle-utf/actions)


## Encode strings as integers

* **mulle_char5** is a compression scheme, that uses a set of 31 characters
to encode strings into integers of varying sizes. This can be useful for small
strings with typical Objective-C "keys" and small common mostly lowercase words.
* **mulle_char7** is a compression scheme placing 7 bit ASCII characters into
ints of varying sizes. A 64 bit integer can hold up to 8 characters.


> Naming: UTF is a transfer encoding for Unicode. So everything
eventually maps to (32 bit) unicode characters. That operations are done on
UTF directly is kinda questionable, but I believe common.


>> TODO (maybe): split this up into mulle_unicode, mulle_unitype, mulle_utf


## API

File                                    | Description
--------------------------------------- | ----------------------------------------
[`mulle_char7`](dox/API_CHAR7.md)       | Encode small ASCII strings into integers
[`mulle_char5`](dox/API_CHAR5.md)       | Encode some more smallish ASCII strings integers
[`Information`](dox/API_INFORMATION.md) | Analyze and classify string encodings
[`String`](dox/API_STRING.md)           | Primitive UTF16 and UTF32 string handling


### You are here

![Overview](overview.dot.svg)


## Add

### Either: link library

Use [mulle-sde](//github.com/mulle-sde) to add mulle-buffer to your project:

```
mulle-sde dependency add --c --github mulle-c mulle-buffer
```

### Or: add Sources

Alternatively you can read [STEAL.md](//github.com/mulle-c11/dox/STEAL.md) on
how to add mulle-c source code into your own projects.


## Install

### mulle-sde

Use [mulle-sde](//github.com/mulle-sde) to build and install mulle-utf and all dependencies:

```
mulle-sde install --linkorder --prefix /usr/local \
   //github.com/mulle-c/mulle-utf/archive/latest.tar.gz
```

### Manual Installation

Install the requirements:

Requirements                                             | Description
---------------------------------------------------------|-----------------------
[mulle-allocator](//github.com/mulle-c/mulle-allocator)  | Memory allocation wrapper


Install into `/usr/local`:

```
mkdir build 2> /dev/null
(
   cd build ;
   cmake -DCMAKE_INSTALL_PREFIX=/usr/local \
         -DCMAKE_PREFIX_PATH=/usr/local \
         -DCMAKE_BUILD_TYPE=Release .. ;
   make install
)
```

## Platforms and Compilers

All platforms and compilers supported by
[mulle-c11](//github.com/mulle-c/mulle-c11) and
[mulle-thread](//github.com/mulle-c/mulle-thread).


## Author

[Nat!](//www.mulle-kybernetik.com/weblog) for
[Mulle kybernetiK](//www.mulle-kybernetik.com) and
[Codeon GmbH](//www.codeon.de)
