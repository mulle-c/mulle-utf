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

#include "mulle_utf_type.h"

#include <string.h>


struct mulle_bytebuffer;

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

int   mulle_utf8_are_valid_extracharacters( char *s, unsigned int len);

//
// if len is -1, assume that *s is '\0' terminated
// returns length in characters
// returns size in bytes
// returns pointer to character, that is invalid (first of compound chars)
// TODO: find encoding where nothing needs to be composed

int  mulle_utf8_information( mulle_utf8_t *s, size_t len, struct mulle_utf_information *info);
int  mulle_utf8_is_ascii( mulle_utf8_t *s, size_t len);


static inline size_t  mulle_utf8_strlen( mulle_utf8_t *s)
{
   return( strlen( (char *) s));
}


static inline size_t  mulle_utf8_strnlen( mulle_utf8_t *s, size_t len)
{
   return( strnlen( (char *) s, len));
}


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



// supply a "mulle_buffer" here as "buffer" and mulle_buffer_add_bytes as the
// callback.
// int == 0 : OK!
// these routines do not skip BOM characters
int  mulle_utf8_bufferconvert_to_utf16( mulle_utf8_t *src,
                                             size_t len,
                                             void *buffer,
                                             void (*add)( void *buffer, void *bytes, size_t size));

// as above, but for utf32
int  mulle_utf8_bufferconvert_to_utf32( mulle_utf8_t *src,
                                             size_t len,
                                             void *buffer,
                                             void (*add)( void *buffer, void *bytes, size_t size));

#endif
