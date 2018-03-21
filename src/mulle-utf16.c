//
//  mulle_utf16.c
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

#include "mulle-utf16.h"

#include "mulle-utf16-string.h"
#include "mulle-utf-ctype.h"
#include "mulle-char5.h"
#include "mulle-utf-ctype.h"
#include <errno.h>
#include <string.h>


#define FORBID_NON_CHARACTERS  1


static inline int   mulle_utf16_is_asciicharacter( mulle_utf16_t c)
{
   return( c < 0x80);
}


static inline int   mulle_utf16_is_char5character( mulle_utf16_t c)
{
   return( mulle_char5_encode_character( c) >= 0);
}


int  mulle_utf16_is_valid_surrogatepair( mulle_utf16_t hi, mulle_utf16_t lo)
{
// invalid surrogate pairs
//
//   D83F DFF*
//   D87F DFF*
//   D8BF DFF*
//   D8FF DFF*
//   D93F DFF*
//   D97F DFF*
//
//   ...
//   DBBF DFF*
//   DBFF DFF*
//   * = E or F

   if( lo == 0xDFFE || lo == 0xDFFF)
      switch( hi & 0xFF)
      {
      case 0x3F :
      case 0x7F :
      case 0xBF :
      case 0xFF :
      return( 0);
   }
   return( 1);
}


// bytebuffer code needs to be reintroduced sometime

//
// this also does not do any error checking, the UTF8 string must be perfect
//
// -1  dst buffer too small
//  0  OK!
//

// must be proper UTF16 code!
int  mulle_utf16_bufferconvert_to_utf8( mulle_utf16_t *src,
                                        size_t len,
                                        void *buffer,
                                        void (*addbytes)( void *buffer, void *bytes, size_t length))
{
   mulle_utf16_t   *sentinel;
   mulle_utf32_t   x;
   mulle_utf8_t    tmp[ 4];

   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);

   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)

   sentinel = &src[ len];

   while( src < sentinel)
   {
      x = *src++;
      if( mulle_utf32_is_highsurrogatecharacter( x))  // hi surrogate
      {
         // decode surrogate
         if( src >= sentinel)
         {
            errno = EINVAL;
            return( -1);
         }
         x = mulle_utf16_decode_surrogatepair( (mulle_utf16_t) x, *src++);
      }

      // 4 cases
      // < 0x80, < 0x800 < 0x10000, > 0x10000
      if( x < 0x800)
      {
         if( x < 0x80)
         {
            tmp[ 0] = (mulle_utf8_t) x;
            (*addbytes)( buffer, tmp, 1);
            continue;
         }

         tmp[ 0] = 0xC0 | (mulle_utf8_t) (x >> 6);
         tmp[ 1] = 0x80 | (x & 0x3F);
         (*addbytes)( buffer, tmp, 2);
      }
      else
      {
         if( x < 0x10000)
         {
            assert( ! mulle_utf32_is_lowsurrogatecharacter( x));

            tmp[ 0] = 0xE0 | (mulle_utf8_t) (x >> 12);
            tmp[ 1] = 0x80 | ((x >> 6) & 0x3F);
            tmp[ 2] = 0x80 | (x & 0x3F);
            (*addbytes)( buffer, tmp, 3);
            continue;
         }

         assert( x <= 0x10FFFF);

         tmp[ 0] = 0xF0 | (mulle_utf8_t) (x >> 18);
         tmp[ 1] = 0x80 | ((x >> 12) & 0x3F);
         tmp[ 2] = 0x80 | ((x >> 6) & 0x3F);
         tmp[ 3] = 0x80 | (x & 0x3F);
         (*addbytes)( buffer, tmp, 4);
      }
   }
   return( 0);
}


int  mulle_utf16_bufferconvert_to_utf32( mulle_utf16_t *src,
                                         size_t len,
                                         void *buffer,
                                         void (*addbytes)( void *buffer, void *bytes, size_t length))
{
   mulle_utf16_t   *sentinel;
   mulle_utf32_t   x;

   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);

   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)

   sentinel = &src[ len];

   while( src < sentinel)
   {
      x = *src++;
      if( mulle_utf32_is_highsurrogatecharacter( x))  // hi surrogate
      {
         // decode surrogate
         if( src >= sentinel)
         {
            errno = EINVAL;
            return( -1);
         }
         x = mulle_utf16_decode_surrogatepair( (mulle_utf16_t) x, *src++);
      }

      (*addbytes)( buffer, &x, sizeof( x));
   }
   return( 0);
}


static inline int  mulle_utf16_is_invalid_char( mulle_utf16_t c)
{
   return( c >= 0xFFFE || c >= 0xFDD0 && c <= 0xFDEF);
}


//
// just checks that the surrogate pairs are ok
//
mulle_utf16_t  *mulle_utf16_validate( mulle_utf16_t *src, size_t len)
{
   mulle_utf16_t   c;
   mulle_utf16_t   d;
   mulle_utf16_t   *sentinel;

   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);

   sentinel = &src[ len];

   for( ; src < sentinel; src++)
   {
      c = *src;

      // non characters, do not want
#if FORBID_NON_CHARACTERS
      if( mulle_utf16_is_invalid_char( c))
         return( src);
#endif
      if( ! mulle_utf32_is_surrogatecharacter( c))
         continue;

      if( mulle_utf32_is_lowsurrogatecharacter( c))
         return( src);

      if( src >= sentinel)
         return( src);

      d = *++src;
      if( ! mulle_utf32_is_lowsurrogatecharacter( d))
         return( src);

      if( ! mulle_utf16_is_valid_surrogatepair( c, d))
         return( src - 1);
   }
   return( 0);
}

//
// this routine does not validate...
//
size_t  mulle_utf16_utf8length( mulle_utf16_t *src, size_t len)
{
   mulle_utf16_t   c;
   mulle_utf16_t   *sentinel;

   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   if( ! len)
      return( 0);

   sentinel = &src[ len];

   for( ; src < sentinel;)
   {
      c = *src++;

      if( c < 0x80)
         continue;

      if( c < 0x0800)
      {
         len++;
         continue;
      }

      // not a surrogate pair ?
      if( ! mulle_utf32_is_surrogatecharacter( c))
      {
         len += 2;
         continue;
      }

      if( ++src > sentinel)
         return( -1);

      len += -1 + 3;
   }
   return( len);
}


size_t  mulle_utf16_length( mulle_utf16_t *src, size_t len)
{
   mulle_utf16_t   c;
   mulle_utf16_t   *sentinel;
   size_t    dst_len;

   assert( sizeof( wchar_t) == sizeof( mulle_utf16_t));

   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);

   sentinel = &src[ len];
   dst_len  = len;

   for( ; src < sentinel;)
   {
      c = *src++;

      if( c < 0x0800)
         continue;

      // not a surrogate pair ?
      if( ! mulle_utf32_is_surrogatecharacter( c))
      {
         dst_len--;
         continue;
      }

      if( ++src > sentinel)
         return( -1);

      dst_len -= 2;
   }

   return( dst_len);
}


mulle_utf32_t   _mulle_utf16_next_utf32character( mulle_utf16_t **s_p)
{
   mulle_utf16_t   *s;
   mulle_utf32_t   value;

   s     = *s_p;
   value = *s++;
   if( mulle_utf32_is_surrogatecharacter( value))
      value = mulle_utf16_decode_surrogatepair( (mulle_utf16_t) value, *s++);
   *s_p  = s;

   return( value);
}


// sorta undoes _mulle_utf8_next_utf32_value
mulle_utf32_t   _mulle_utf16_previous_utf32character( mulle_utf16_t **s_p)
{
   mulle_utf16_t   *s;
   mulle_utf32_t   value;

   s     = *s_p;
   value = *--s;

   if( mulle_utf32_is_surrogatecharacter( value))
      value = mulle_utf16_decode_surrogatepair( *--s, (mulle_utf16_t) value);

   *s_p  = s;

   return( value);
}



//
// make it more optimal, by (a) checking that pointer can be accessed with
// a long or long long
// (b) masking value with 0x80808080 to figure out if all are "ASCII"
//
int  mulle_utf16_information( mulle_utf16_t *src, size_t len, struct mulle_utf_information *info)
{
   mulle_utf16_t                  _c;
   mulle_utf16_t                  *start;
   mulle_utf16_t                  *sentinel;
   struct mulle_utf_information   dummy;

   if( ! info)
      info = &dummy;

   info->has_terminating_zero = 0;
   info->invalid              = NULL;
   info->start                = src;
   info->is_ascii             = 1;
   info->is_char5             = 1;
   info->is_utf15             = 1;
   info->utf8len              = 0;
   info->utf16len             = 0;
   info->utf32len             = 0;
   info->has_bom              = 0;

   if( ! len)
      return( 0);
   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);

   //
   // remove leading BOM
   //
   info->has_bom = mulle_utf32_is_bomcharacter( *src);
   if( info->has_bom)
   {
      src += 1;
      len -= 1;
   }

   info->start = src;
   start       = src;
   sentinel    = &src[ len];

   for( ; src < sentinel; src++)
   {
      if( ! (_c = *src))
      {
         info->has_terminating_zero = 1;
         break;
      }

      if( mulle_utf16_is_asciicharacter( _c))
      {
         if( info->is_char5 && ! mulle_utf16_is_char5character( _c))
            info->is_char5 = 0;
         continue;
      }

      info->is_ascii = 0;
      info->utf8len++;

      if( _c >= 0x0800)
         info->utf8len++;

      if( _c >= 0x8000)
         info->is_utf15 = 0;

      // surrogate pair
      if( _c >= 0xD800 && _c <= 0xE000)
      {
         if( ! mulle_utf32_is_highsurrogatecharacter( _c))
            goto fail;

         info->utf8len++;
         info->utf32len--;

         if( ++src >= sentinel)
            goto fail;

         _c = *src;
         if( ! mulle_utf32_is_lowsurrogatecharacter( _c))
            goto fail;
      }

#if FORBID_NON_CHARACTERS
      if( mulle_utf16_is_invalid_char( _c))
         goto fail;
#endif
   }

   info->utf8len  += src - start;
   info->utf16len += src - start;
   info->utf32len += src - start;
   info->is_char5 &= info->is_ascii;
   return( 0);

fail:
   memset( info, 0, sizeof( *info));
   info->invalid = src;
   return( -1);
}


int   mulle_utf16_is_ascii( mulle_utf16_t *src, size_t len)
{
   mulle_utf16_t   _c;
   mulle_utf16_t   *sentinel;

   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   if( ! len)
      return( 1);

   sentinel = &src[ len];

   while( src < sentinel)
   {
      _c = *src++;
      if( _c >= 0x80)
         return( 0);
   }

   return( 1);
}