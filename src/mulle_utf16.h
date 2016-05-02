//
//  mulle_utf16.h 
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

#ifndef mulle_utf16_h__
#define mulle_utf16_h__

#include "mulle_utf_type.h"

#include <assert.h>
#include <stddef.h>

//
//
// These routines will strip off a leading BOM, but they will never add one.
//
// The mulle_utf16_t endianness is machine specific.
//

static inline void  mulle_utf16_encode_surrogatepair( mulle_utf32_t x, mulle_utf16_t *hi, mulle_utf16_t *lo)
{
   mulle_utf16_t  top;
   mulle_utf16_t  bottom;
   
   assert( x >= 0x10000 && x <= 0x10FFFF);
   
   x -= 0x10000;
   
   assert( (x >> 10) <= 0x3FF);
   
   top    = (mulle_utf16_t) (x >> 10);
   bottom = (mulle_utf16_t) (x & 0x3FF);
   
   *hi = 0xD800 + top;
   *lo = 0xDC00 + bottom;
   
   assert( *hi >= 0xD800 && *hi < 0xDC00);
   assert( *lo >= 0xDC00 && *lo < 0xE000);
}



static inline mulle_utf32_t  mulle_utf16_decode_surrogatepair( mulle_utf16_t hi, mulle_utf16_t lo)
{
   mulle_utf32_t   top;
   mulle_utf32_t   bottom;
   
   assert( hi >= 0xD800 && hi < 0xDC00);
   assert( lo >= 0xDC00 && lo < 0xE000);
   
   top    = (mulle_utf32_t) (hi - 0xD800);
   bottom = (mulle_utf32_t) (lo - 0xDC00);
   
   return( 0x10000 + (top << 10) + bottom);
}


static inline mulle_utf16_t  mulle_utf16_get_bom_char( void)
{
   return( 0xFEFF);  // only native encoding so far...
}


static inline int  mulle_utf16_is_bom_char( mulle_utf16_t c)
{
   return( c == mulle_utf16_get_bom_char());  // only native encoding so far...
}



struct mulle_utf16_information
{
   size_t          utf8len;
   size_t          utf16len;
   size_t          utf32len;
   mulle_utf16_t   *start;          // behind BOM if bommed, otherwise start
   mulle_utf16_t   *invalid_utf16;  // first fail char
   int             has_bom;
   int             is_ascii;
   int             is_char5;
   int             has_terminating_zero;
};

int     mulle_utf16_information( mulle_utf16_t *src, size_t len, struct mulle_utf16_information *info);

size_t  mulle_utf16_length_as_utf8( mulle_utf16_t *src, size_t len);
size_t  mulle_utf16_length( mulle_utf16_t *src, size_t len);
int     mulle_utf16_is_ascii( mulle_utf16_t *src, size_t len);
size_t  mulle_utf16_strlen( mulle_utf16_t *src);
size_t  mulle_utf16_strnlen( mulle_utf16_t *src, size_t len);

static inline size_t  mulle_utf16_max_length_as_utf8( size_t len)
{
   return( len * 4);
}

mulle_utf16_t  *mulle_utf16_validate( mulle_utf16_t *src, size_t len);

// hi and lo MUST be surrogates
int  mulle_utf16_is_valid_surrogatepair( mulle_utf16_t hi, mulle_utf16_t lo);

// supply a "mulle_buffer" here as "buffer" and mulle_buffer_guarantee as the
// callback.
// This will not stop on a zero. It will not by itself append a zero.
// int == 0 : OK!
// these routines do not skip BOM characters
int  mulle_utf16_convert_to_utf8_bytebuffer( void *buffer,
                                             void *(*addbytes)( void *buffer, void *bytes, size_t length),
                                             mulle_utf16_t *src,
                                             size_t len);

int  mulle_utf16_convert_to_utf32_bytebuffer( void *buffer,
                                              void *(*addbytes)( void *buffer, void *bytes, size_t length),
                                              mulle_utf16_t *src,
                                              size_t len);

#endif
