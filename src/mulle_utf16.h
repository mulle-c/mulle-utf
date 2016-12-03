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


static inline int   mulle_utf_is_hi_surrogate( unsigned int x)
{
   return( x >= 0xD800 && x < 0xDC00);
}


static inline int   mulle_utf_is_lo_surrogate( unsigned int x)
{
   return( x >= 0xDC00 && x < 0xE000);
}


static inline int   mulle_utf_is_surrogate( unsigned int x)
{
   return( x >= 0xD800 && x < 0xE000);
}


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
   
   assert( mulle_utf_is_hi_surrogate( hi));
   assert( mulle_utf_is_lo_surrogate( lo));
   
   top    = (mulle_utf32_t) (hi - 0xD800);
   bottom = (mulle_utf32_t) (lo - 0xDC00);
   
   return( 0x10000 + (top << 10) + bottom);
}



static inline mulle_utf32_t   mulle_utf16_pull_surrogatepair( mulle_utf16_t c, mulle_utf16_t **s)
{
   mulle_utf16_t   d;
   
   if( c < 0xD800 || c >= 0xDC00)
      return( c);

   d = *++*s;
   if( d < 0xDC00 || d >= 0xE000)
   {
      --*s;
      return( c);
   }

   /* always HI followed by LO */
   return( mulle_utf16_decode_surrogatepair( c, d));
}


static inline mulle_utf16_t  mulle_utf16_get_bom_character( void)
{
   return( 0xFEFF);  // only native encoding so far...
}


static inline int  mulle_utf16_is_bom_character( mulle_utf16_t c)
{
   return( c == mulle_utf16_get_bom_character());  // only native encoding so far...
}


int     mulle_utf16_information( mulle_utf16_t *src, size_t len, struct mulle_utf_information *info);

size_t  mulle_utf16_utf8length( mulle_utf16_t *src, size_t len);
size_t  mulle_utf16_length( mulle_utf16_t *src, size_t len);
int     mulle_utf16_is_ascii( mulle_utf16_t *src, size_t len);

static inline size_t  mulle_utf16_max_length_as_utf8( size_t len)
{
   return( len * 4);
}

mulle_utf16_t  *mulle_utf16_validate( mulle_utf16_t *src, size_t len);

// hi and lo MUST be surrogates
int  mulle_utf16_is_valid_surrogatepair( mulle_utf16_t hi, mulle_utf16_t lo);

// step through string characters
mulle_utf32_t   _mulle_utf16_next_utf32character( mulle_utf16_t **s_p);
mulle_utf32_t   _mulle_utf16_previous_utf32character( mulle_utf16_t **s_p);

//
// This will not stop on a zero. It will not by itself append a zero.
// return value = 0  means OK!
// these routines do not skip BOM characters
//
int  mulle_utf16_bufferconvert_to_utf8( mulle_utf16_t *src,
                                             size_t len,
                                             void *buffer,
                                             void (*addbytes)( void *buffer, void *bytes, size_t length));

int  mulle_utf16_bufferconvert_to_utf32( mulle_utf16_t *src,
                                              size_t len,
                                              void *buffer,
                                              void (*addbytes)( void *buffer, void *bytes, size_t length));

#endif
