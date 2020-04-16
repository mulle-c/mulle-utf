#
# cmake/reflect/_Headers.cmake is generated by `mulle-sde reflect`. Edits will be lost.
#
if( MULLE_TRACE_INCLUDE)
   MESSAGE( STATUS "# Include \"${CMAKE_CURRENT_LIST_FILE}\"" )
endif()

set( INCLUDE_DIRS
src
src/reflect
)

set( PRIVATE_GENERATED_HEADERS
src/reflect/_mulle-utf-include-private.h
)

set( PRIVATE_HEADERS
src/include-private.h
)

set( PUBLIC_GENERATED_HEADERS
src/reflect/_mulle-utf-include.h
)

set( PUBLIC_HEADERS
src/include.h
src/mulle-char5.h
src/mulle-char7.h
src/mulle-utf16.h
src/mulle-utf16-string.h
src/mulle-utf32.h
src/mulle-utf32-string.h
src/mulle-utf8.h
src/mulle-utf-convenience.h
src/mulle-utf.h
src/mulle-utf-noncharacter.h
src/mulle-utf-rover.h
src/mulle-utf-scan.h
src/mulle-utf-type.h
)
