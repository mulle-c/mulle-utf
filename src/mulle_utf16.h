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
#include "mulle_bytebuffer.h"

//
//
// These routines will strip off a leading BOM, but they will never add one.
//
// The utf16char endianness is machine specific. For import/export there are
// conversion routines somewhere else
//

static inline void  mulle_utf16_encode_surrogatepair( utf32char x, utf16char *hi, utf16char *lo)
{
   utf16char  top;
   utf16char  bottom;
   
   assert( x >= 0x10000 && x <= 0x10FFFF);
   
   x -= 0x10000;
   
   assert( (x >> 10) <= 0x3FF);
   
   top    = (utf16char) (x >> 10);
   bottom = (utf16char) (x & 0x3FF);
   
   *hi = 0xD800 + top;
   *lo = 0xDC00 + bottom;
   
   assert( *hi >= 0xD800 && *hi < 0xDC00);
   assert( *lo >= 0xDC00 && *lo < 0xE000);
}


void  mulle_utf16_encode_surrogatepair_into_bytebuffer( struct mulle_bytebuffer *dst, utf32char x);


static inline utf32char  mulle_utf16_decode_surrogatepair( utf16char hi, utf16char lo)
{
   utf32char   top;
   utf32char   bottom;
   
   assert( hi >= 0xD800 && hi < 0xDC00);
   assert( lo >= 0xDC00 && lo < 0xE000);
   
   top    = (utf32char) (hi - 0xD800);
   bottom = (utf32char) (lo - 0xDC00);
   
   return( 0x10000 + (top << 10) + bottom);
}


static inline int  mulle_utf16_is_bom( utf16char c)
{
   return( c == 0xFEFF);  // only native encoding so far...
}

size_t  _mulle_utf16_convert_to_utf8( utf8char *dst, size_t dst_len, utf16char *src, size_t *p_len);
size_t  _mulle_utf16_convert_to_utf8_bytebuffer( struct mulle_bytebuffer *dst, utf16char *src, size_t *p_len);

static inline size_t   mulle_utf16_convert_to_utf8_bytebuffer( struct mulle_bytebuffer *dst,
                                                               utf16char *src,
                                                               size_t len)
{
   return( _mulle_utf16_convert_to_utf8_bytebuffer( dst, src, &len));
}

static inline size_t   mulle_utf16_convert_to_utf8( utf8char *dst, size_t dst_len, utf16char *src, size_t len)
{
   return( _mulle_utf16_convert_to_utf8( dst, dst_len, src, &len));
}


size_t  mulle_utf16_length_as_utf8( utf16char *src, size_t len);
size_t  mulle_utf16_length( utf16char *src, size_t len);
int     mulle_utf16_is_ascii( utf16char *src, size_t len);
size_t  mulle_utf16_strlen( utf16char *src);

static inline size_t  mulle_utf16_max_length_as_utf8( size_t len)
{
   return( len * 4);
}

utf16char  *mulle_utf16_validate( utf16char *src, size_t len);

// hi and lo MUST be surrogates
int  mulle_utf16_is_valid_surrogatepair( utf16char hi, utf16char lo);

#endif
