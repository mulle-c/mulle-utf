//
//  mulle_utf32.h
//  mulle-utf
//
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  All rights reserved.
//
//  Coded by Nat!
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//

#include "mulle_utf_type.h"


#include <stddef.h>


size_t  mulle_utf32_strlen( mulle_utf32_t *src);
size_t  mulle_utf32_strnlen( mulle_utf32_t *src, size_t len);


static inline mulle_utf32_t  mulle_utf32_get_bom_char( void)
{
   return( 0xFEFF);  // only native encoding so far...
}


static inline int  mulle_utf32_is_bom_char( mulle_utf32_t c)
{
   return( c == mulle_utf32_get_bom_char());  // only native encoding so far...
}


static inline int   mulle_utf32_is_non_char( mulle_utf32_t c)
{
   return( (c >= 0xFFFE && c <= 0xFFFF) || (c >= 0xFDD0 && c <= 0xFDEF));
}


size_t   mulle_utf32_length_as_utf8( mulle_utf32_t *src,
                                     size_t len);

struct mulle_utf32_information
{
   size_t          utf8len;
   size_t          utf16len;
   size_t          utf32len;
   mulle_utf32_t   *start;          // behind BOM if bommed, otherwise start
   mulle_utf32_t   *invalid_utf32;  // first fail char
   int             has_bom;
   int             is_ascii;
   int             has_terminating_zero;
};

int     mulle_utf32_information( mulle_utf32_t *src,
                                 size_t len,
                                 struct mulle_utf32_information *info);



void  _mulle_utf32_encode_as_surrogatepair_into_utf16_bytebuffer(
                       void *buffer,
                       void (*add)( void *, void *, size_t size),
                       mulle_utf32_t x);

int  _mulle_utf32_convert_to_utf8_bytebuffer( void *buffer,
                                              void (*add)( void *, void *, size_t size),
                                              mulle_utf32_t *src,
                                              size_t len);

int  _mulle_utf32_convert_to_utf16_bytebuffer( void *buffer,
                                               void (*add)( void *, void *, size_t size),
                                               mulle_utf32_t *src,
                                               size_t len);
