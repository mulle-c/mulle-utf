//
//  mulle_utf_convenience.h
//  mulle-utf
//
//  Created by Nat! on 30.05.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_convenience_h__
#define mulle_utf_convenience_h__

// pollute mulle_utf with mulle_allocator ? but it's just so.. convenient!

#include <mulle_allocator/mulle_allocator.h>
#include "mulle_utf_type.h"



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

#if MULLE_ALLOCATOR_VERSION  < ((1 << 20) | (5 << 8) | 0)
# error "mulle_allocator is too old"
#endif

#endif /* mulle_utf_convenience_h */
