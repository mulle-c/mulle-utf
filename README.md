# mulle-utf

#### 🔤 UTF8-16-32 analysis and manipulation library

It is a C (C99) library to analyze and convert unicode strings. It also
contains some limited `<string.h>` like functionality for UTF16 and UTF32.
It is the backbone of **NSString**.



| Release Version                                       | Release Notes  | AI Documentation
|-------------------------------------------------------|----------------|---------------
| ![Mulle kybernetiK tag](https://img.shields.io/github/tag/mulle-c/mulle-utf.svg) ![standard: C11](pix/c11.svg) ![apis: ansi](pix/apis-ansi.svg) ![memory: allocator](pix/memory-allocator.svg) ![threads: threadsafe](pix/thread-threadsafe.svg) [![Build Status](https://github.com/mulle-c/mulle-utf/workflows/CI/badge.svg)](//github.com/mulle-c/mulle-utf/actions) | [RELEASENOTES](RELEASENOTES.md) | [DeepWiki for mulle-utf](https://deepwiki.com/mulle-c/mulle-utf)


## API

* [API Overview](asset/dox/api/toc)

| Documentation                          | Description                                      |
|----------------------------------------|--------------------------------------------------|
| [`mulle_char7`](dox/API_CHAR7.md)      | Encode small ASCII strings into integers         |
| [`mulle_char5`](dox/API_CHAR5.md)      | Encode some more smallish ASCII strings integers |
| [conversion](dox/API_CONVERSION.md)    | Character and string conversions                 |
| [information](dox/API_INFORMATION.md)  | Analyze and classify string encodings            |
| [string](dox/API_STRING.md)            | Primitive UTF16 and UTF32 string handling        |

For character type classification (`is_alphanumeric`, `is_whitespace`,
`tolower`, `toupper`, etc.) see
[mulle-unicode](//github.com/mulle-c/mulle-unicode).

`size_t` is used for byte-sized (really `char`-sized) lengths. UTF16 and
UTF32 use `size_t`.





### You are here

![Overview](overview.dot.svg)





## Add

### Add mulle-utf via mulle-core

mulle-utf is a component of the
[mulle-core](//github.com/mulle-core/mulle-core) library.
In your code include its umbrella header:

``` c
#include <mulle-core/mulle-core.h>
```

Add mulle-core to your `CMakeLists.txt` with
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html):

``` cmake
include( FetchContent)

set( MULLE__CORE_TAG "master" CACHE STRING "branch or tag to fetch mulle-core from")

FetchContent_Declare( MULLE__CORE
   GIT_REPOSITORY https://github.com/mulle-core/mulle-core.git
   GIT_TAG        "${MULLE__CORE_TAG}")
FetchContent_MakeAvailable( MULLE__CORE)

target_link_libraries( ${PROJECT_NAME} PUBLIC mulle-core)
```

Or add to your mulle-sde project with:

``` sh
mulle-sde add mulle-core/mulle-core
```




### Add mulle-utf as a cmake subproject

In your code include its header:

``` c
#include <mulle-utf/mulle-utf.h>
```

Add mulle-utf and its dependencies to your `CMakeLists.txt` with
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html):

``` cmake
include( FetchContent)

include_directories( SYSTEM "${CMAKE_BINARY_DIR}/include")

set( MULLE__ALLOCATOR_TAG "master" CACHE STRING "branch or tag to fetch mulle-allocator from")

FetchContent_Declare( MULLE__ALLOCATOR
   GIT_REPOSITORY https://github.com/mulle-c/mulle-allocator.git
   GIT_TAG        "${MULLE__ALLOCATOR_TAG}")
FetchContent_MakeAvailable( MULLE__ALLOCATOR)

set( MULLE__DATA_TAG "master" CACHE STRING "branch or tag to fetch mulle-data from")

FetchContent_Declare( MULLE__DATA
   GIT_REPOSITORY https://github.com/mulle-c/mulle-data.git
   GIT_TAG        "${MULLE__DATA_TAG}")
FetchContent_MakeAvailable( MULLE__DATA)

set( MULLE__UTF_TAG "master" CACHE STRING "branch or tag to fetch mulle-utf from")

FetchContent_Declare( MULLE__UTF
   GIT_REPOSITORY https://github.com/mulle-c/mulle-utf.git
   GIT_TAG        "${MULLE__UTF_TAG}")
FetchContent_MakeAvailable( MULLE__UTF)

target_link_libraries( ${PROJECT_NAME} PUBLIC mulle-utf)
```

Or add to your mulle-sde project with:

``` sh
mulle-sde add github:mulle-c/mulle-utf
```

## Individual Install

Install all the prerequisites.

``` bash
PREFIX=/tmp/usr/local
BUILD_DIR="mulle-utf/build"

cmake -S "mulle-utf" -B "${BUILD_DIR}"    \
  -DCMAKE_BUILD_TYPE="Release"                  \
  -DCMAKE_INSTALL_PREFIX="${PREFIX}"            \
  -DCMAKE_PREFIX_PATH="${PREFIX}"               \
  -DCMAKE_C_FLAGS="-I"${PREFIX}"/include"       \
&& cmake --build "${BUILD_DIR}"  --parallel     \
&& cmake --install "${BUILD_DIR}"
```


## Author

[Nat!](https://mulle-kybernetik.com/weblog) for Mulle kybernetiK  



