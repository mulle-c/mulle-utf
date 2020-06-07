//
//  mulle_utf8.h
//  mulle-utf
//
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  Copyright (c) 2011 Codeon GmbH.
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
#ifndef mulle_utf8_h__
#define mulle_utf8_h__

#include "mulle-utf-type.h"

#include <string.h>


// Check: http://tools.ietf.org/html/rfc3629
// General considerations.
//    On the initial procurement of your UTF8 string, you must validate it once
//    then remember it as validated and don't change it (NSString :))
//    If this is the case, you can run all the other routines "quickly" without
//    having to validate again.

//
// if len is -1, assume that *s is '\0' terminated
// function is not ware of UTF8 BOM
// returned length does not include BOM
//
size_t  mulle_utf8_utf16length( mulle_utf8_t *src, size_t len);


static inline size_t  mulle_utf8_utf16maxlength( size_t len)
{
   return( len * 4);
}


static inline int  mulle_utf8_has_leading_bomcharacter( mulle_utf8_t *src, size_t len)
{
   if( len < 3)
      return( 0);

   return( src[ 0] == 0xEF && src[ 1] == 0xBB && src[ 2] == 0xBF);
}

// 0: no 1: yes
// if yes, p_c will contain char value
//
int   mulle_utf8_are_valid_extracharacters( mulle_utf8_t *s, unsigned int len, mulle_utf32_t *p_c);

//
// if len is -1, assume that *s is '\0' terminated
// returns length in characters
// returns size in bytes
// returns pointer to character, that is invalid (first of compound chars)
// TODO: find encoding where nothing needs to be composed

int  mulle_utf8_information( mulle_utf8_t *s, size_t len, struct mulle_utf_information *info);
int  mulle_utf8_is_ascii( mulle_utf8_t *s, size_t len);

// returns NULL if OK, otherwise the offending character address
mulle_utf8_t  *mulle_utf8_validate( mulle_utf8_t *src, size_t len);


static inline size_t  mulle_utf8_strlen( mulle_utf8_t *s)
{
   return( strlen( (char *) s));
}


//
// hand coded because linux doesn't have it by default, and I want to get rid
// of the warning without having to define __USE_XOPEN2K8
//
static inline size_t  mulle_utf8_strnlen( mulle_utf8_t *s, size_t len)
{
   mulle_utf8_t   *start;
   mulle_utf8_t   *sentinel;

   start    = s;
   sentinel = &s[ len];

   while( s < sentinel)
   {
      if( ! *s)
         break;
      ++s;
   }
   return( (size_t) (s - start));
}


// use the more canonical *, size_t oder
static inline int  mulle_utf8_strncmp( mulle_utf8_t *s, size_t len, mulle_utf8_t *other)
{
   return( strncmp( (char *) s, (char *) other, len));
}


mulle_utf8_t   *mulle_utf8_strnstr( mulle_utf8_t *s, size_t len, mulle_utf8_t *search);
mulle_utf8_t   *mulle_utf8_strnchr( mulle_utf8_t *s, size_t len, mulle_utf8_t c);
size_t         mulle_utf8_strnspn( mulle_utf8_t *s, size_t len, mulle_utf8_t *search);
size_t         mulle_utf8_strncspn( mulle_utf8_t *s, size_t len, mulle_utf8_t *search);


// extremely primitive!
// iterate back and forth over a  buffer. the utf8 must be valid, and
// this doesn't check for zero or buffer overflow
//
mulle_utf32_t   _mulle_utf8_next_utf32character( mulle_utf8_t **s_p);
mulle_utf32_t   _mulle_utf8_previous_utf32character( mulle_utf8_t **s_p);


// use this to walk through a utf8 string
static inline mulle_utf32_t   mulle_utf8_next_utf32character( mulle_utf8_t **s_p)
{
   if( **s_p <= 0x7F)
      return( *(*s_p)++);
   return( _mulle_utf8_next_utf32character( s_p));
}


// used in Foundation and maybe here in the future
struct mulle_utf8_data
{
   mulle_utf8_t   *characters;
   size_t         length;
};


static inline struct mulle_utf8_data   mulle_utf8_data_make( mulle_utf8_t *s, size_t length)
{
   struct mulle_utf8_data   data;

   data.characters = s;
   data.length     = length;
   return( data);
}

mulle_utf32_t   *_mulle_utf8_convert_to_utf32( mulle_utf8_t *src,
                                               size_t len,
                                               mulle_utf32_t *dst);


// low level conversion, no checks dst is assumed to be wide enough
// returns end of dst
mulle_utf16_t   *_mulle_utf8_convert_to_utf16( mulle_utf8_t *src,
                                               size_t len,
                                               mulle_utf16_t *dst);

mulle_utf32_t   *_mulle_utf8_convert_to_utf32( mulle_utf8_t *src,
                                               size_t len,
                                               mulle_utf32_t *dst);
//
// You can supply a "mulle-buffer" here as "buffer" and mulle_buffer_add_bytes
// as the callback.
// These routines do not skip BOM characters. And don't check for validity.
// The input must be correct!
//
void   mulle_utf8_bufferconvert_to_utf16( mulle_utf8_t *src,
                                          size_t len,
                                          void *buffer,
                                          mulle_utf_add_bytes_function_t addbytes);

// as above, but for utf32
void   mulle_utf8_bufferconvert_to_utf32( mulle_utf8_t *src,
                                          size_t len,
                                          void *buffer,
                                          mulle_utf_add_bytes_function_t addbytes);

#endif
