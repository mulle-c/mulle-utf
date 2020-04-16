//
//  mulle_utf_convenience.h
//  mulle-utf
//
//  Created by Nat! on 30.05.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_convenience_h__
#define mulle_utf_convenience_h__

// pollute mulle_utf with mulle_allocator ? but it's just so.. convenient!

#include "include.h"
#include "mulle-utf-type.h"
#include "mulle-utf8.h"

// these convenience routines always append a \0
// the returned string is allocated with the allocator, which may be NULL

mulle_utf16_t  *mulle_utf8_convert_to_utf16( mulle_utf8_t *src,
                                             size_t len,
                                             struct mulle_allocator *allocator);
mulle_utf32_t  *mulle_utf8_convert_to_utf32( mulle_utf8_t *src,
                                             size_t len,
                                             struct mulle_allocator *allocator);

mulle_utf8_t  *mulle_utf16_convert_to_utf8( mulle_utf16_t *src,
                                            size_t len,
                                            struct mulle_allocator *allocator);
mulle_utf32_t  *mulle_utf16_convert_to_utf32( mulle_utf16_t *src,
                                              size_t len,
                                              struct mulle_allocator *allocator);

mulle_utf8_t  *mulle_utf32_convert_to_utf8( mulle_utf32_t *src,
                                            size_t len,
                                            struct mulle_allocator *allocator);
mulle_utf16_t  *mulle_utf32_convert_to_utf16( mulle_utf32_t *src,
                                             size_t len,
                                             struct mulle_allocator *allocator);


struct mulle_utf8_data
   mulle_utf8_character_convert( mulle_utf8_t *src,
                                 size_t len,
                                 mulle_utf32_t (*f_conversion)( mulle_utf32_t),
                                 struct mulle_allocator *allocator);
struct mulle_utf8_data
   mulle_utf8_word_convert( mulle_utf8_t *src,
                            size_t len,
                            mulle_utf32_t (*f1_conversion)( mulle_utf32_t),
                            mulle_utf32_t (*f2_conversion)( mulle_utf32_t),
                            int           (*is_white)( mulle_utf32_t),
                            struct mulle_allocator *allocator);

#if MULLE_ALLOCATOR_VERSION  < ((1 << 20) | (5 << 8) | 0)
# error "mulle-allocator is too old"
#endif

#endif /* mulle_utf_convenience_h */
