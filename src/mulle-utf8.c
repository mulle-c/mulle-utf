//
//  mulle_utf8.c
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
#include "mulle-utf8.h"

#include "mulle-char5.h"
#include "mulle-utf16.h"
#include "mulle-utf32.h"
#include "mulle-utf-noncharacter.h"
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


// it's not compatible to Apple
#define FORBID_NON_CHARACTERS  1


enum
{
   mulle_utf8_ascii_start_character,
   mulle_utf8_multiple_start_character,
   mulle_utf8_invalid_start_character
};



static inline int   mulle_utf8_is_asciicharacter( mulle_utf8_t c)
{
   return( (char) c >= 0);
}


static inline int   mulle_utf8_is_char5character( mulle_utf8_t c)
{
   return( mulle_char5_encode_character( c) >= 0);
}


static inline int   mulle_utf8_is_invalidstartcharacter( mulle_utf8_t c)
{
   return( (c >= 0x80 && c < 0xC2) || c >= 0xF5);
}


//static inline int   mulle_utf8_is_start_character( mulle_utf8_t c)
//{
//   return( !  mulle_utf8_is_invalidstartcharacter( c));
//}



static inline int   mulle_utf8_get_startcharactertype( mulle_utf8_t c)
{
   if( mulle_utf8_is_asciicharacter( c))
      return( mulle_utf8_ascii_start_character);

   if( mulle_utf8_is_invalidstartcharacter( c))
      return( mulle_utf8_invalid_start_character);

   return( mulle_utf8_multiple_start_character);
}


static inline int   mulle_utf8_is_validcontinuationcharacter( mulle_utf8_t c)
{
   return( c >= 0x80 && c < 0xC0);
}


static inline unsigned int  mulle_utf8_get_extracharacterslength( mulle_utf8_t c)
{
   assert( mulle_utf8_get_startcharactertype( c) == mulle_utf8_multiple_start_character);

   if( c < 0xE0)
      return( 1);  // 11 bits

   if( c < 0xF0)
      return( 2); //  16 bits

   return( 3);  // 21 bits -> UTF32
}


//
// this also works with extra_len = 0
//
static mulle_utf32_t   mulle_utf8_extracharactersvalue( mulle_utf8_t *src, size_t extra_len)
{
   mulle_utf8_t    _c;
   mulle_utf32_t   x;

   assert( src);
   assert( extra_len >=0 && extra_len <= 3);

   _c = *src++;
   x  = _c;

   switch( extra_len)
   {
   case 1 : // 11 bits
      x   = (_c & 0x1F) << 6;

      _c = *src;
      assert( mulle_utf8_is_validcontinuationcharacter( _c));
      x  |= (_c & 0x3F);

      assert( x >= 128);
      break;

   case 2 :  // 16 bits
      x   = (_c & 0x0F) << 12;

      _c = *src++;
      assert( mulle_utf8_is_validcontinuationcharacter( _c));
      x  |= (_c & 0x3F) << 6;

      _c = *src;
      assert( mulle_utf8_is_validcontinuationcharacter( _c));
      x  |= (_c & 0x3F);

      assert( x >= 0x800 && (x < 0xD800 || x >= 0xE000));
      break;

   case 3 :   // 21 bits -> UTF32
      x   = (_c & 0x7) << 18;

      _c = *src++;
      assert( mulle_utf8_is_validcontinuationcharacter( _c));
      x  |= (_c & 0x3F) << 12;

      _c = *src++;
      assert( mulle_utf8_is_validcontinuationcharacter( _c));
      x  |= (_c & 0x3F) << 6;

      _c = *src;
      assert( mulle_utf8_is_validcontinuationcharacter( _c));
      x  |= (_c & 0x3F);

      assert( x >= 0x10000 && x <= 0x10FFFF);
   }
   return( x);
}


mulle_utf32_t   _mulle_utf8_next_utf32character( mulle_utf8_t **s_p)
{
   mulle_utf8_t    *s;
   mulle_utf8_t    c;
   mulle_utf32_t   value;
   unsigned int    len;

   s = *s_p;
   c = *s++;
   if( mulle_utf8_is_asciicharacter( c))
   {
      *s_p  = s;
      return( c);
   }

   len   = mulle_utf8_get_extracharacterslength( c);
   value = mulle_utf8_extracharactersvalue( &s[ -1], len);
   *s_p  = &s[ len];

   return( value);
}


// sorta undoes _mulle_utf8_next_utf32_value
mulle_utf32_t   _mulle_utf8_previous_utf32character( mulle_utf8_t **s_p)
{
   mulle_utf8_t    *s;
   mulle_utf8_t    c;
   mulle_utf32_t   value;
   unsigned int    extra_len;

   extra_len = 0;
   s         = *s_p;
   for(;;)
   {
      c = *--s;

      if( mulle_utf8_get_startcharactertype( c) != mulle_utf8_invalid_start_character)
         break;

      ++extra_len;
   }

   value = mulle_utf8_extracharactersvalue( s, extra_len);
   *s_p  = s;

   return( value);
}



//
// the slower non-crashing code ...
//
int   mulle_utf8_are_valid_extracharacters( mulle_utf8_t *src, unsigned int len, mulle_utf32_t *p_x)
{
   mulle_utf8_t    _c;
   mulle_utf32_t   x;

   assert( src);
   assert( len >= 1 && len <= 3);

   _c = *src++;

   switch( len)
   {
   case 1 : // 11 bits
      x  = (_c & 0x1F) << 6;

      _c = *src;
      if( ! mulle_utf8_is_validcontinuationcharacter( _c))
         return( 0);

      x  |= (_c & 0x3F);
      if( x < 0x80)   // can't be ASCII
         return( 0);
      break;

   case 2 :  // 16 bits
      x  = (_c & 0x0F) << 12;

      _c = *src++;
      if( ! mulle_utf8_is_validcontinuationcharacter( _c))
         return( 0);
      x  |= (_c & 0x3F) << 6;

      _c = *src;
      if( ! mulle_utf8_is_validcontinuationcharacter( _c))
         return( 0);
      x  |= (_c & 0x3F);

      if( x < 0x800)  // can't be in case 1 range
         return( 0);
      break;

   case 3 :   // 21 bits -> UTF32
      x   = (_c & 0x7) << 18;

      _c = *src++;
      if( ! mulle_utf8_is_validcontinuationcharacter( _c))
         return( 0);
      x  |= (_c & 0x3F) << 12;

      _c = *src++;
      if( ! mulle_utf8_is_validcontinuationcharacter( _c))
         return( 0);
      x  |= (_c & 0x3F) << 6;

      _c = *src;
      if( ! mulle_utf8_is_validcontinuationcharacter( _c))
         return( 0);
      x  |= (_c & 0x3F);

      if( x < 0x10000 || x > 0x0010FFFF)  // // can't be in case 2 range or totally too large
         return( 0);
   }

#if FORBID_NON_CHARACTERS
   if( mulle_utf32_is_invalidcharacter( x))
      return( 0);
#endif
   *p_x = x;
   return( 1);
}


//
// this also does not do any error checking, the UTF8 string must be perfect
//
// -1  check errno
//  0  OK!
//

int  mulle_utf8_bufferconvert_to_utf16( mulle_utf8_t *src,
                                        size_t len,
                                        void *buffer,
                                        void (*addbytes)( void *, void *, size_t size))
{
   mulle_utf16_t   _w;
   mulle_utf8_t    *next;
   mulle_utf8_t    *sentinel;
   mulle_utf8_t    _c;
   size_t          extra_len;
   uint32_t        x;

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)

   sentinel = &src[ len];

   while( src < sentinel)
   {
      _c = *src++;
      assert( mulle_utf8_get_startcharactertype( _c) != mulle_utf8_invalid_start_character);

      if( (char) _c >= 0)
      {
         _w = (uint16_t) _c;
         (*addbytes)( buffer, &_w, sizeof( _w));
         continue;
      }

      extra_len = mulle_utf8_get_extracharacterslength( _c);
      next      = &src[ extra_len];
      if( next > sentinel)
      {
         errno = EINVAL;
         return( -1);
      }

      x   = mulle_utf8_extracharactersvalue( src - 1, extra_len);
      src = next;
      if( x < 0x10000)
      {
         _w = (uint16_t) x;
         (*addbytes)( buffer, &_w, sizeof( _w));
         continue;
      }

      mulle_utf32_bufferconvert_to_utf16_as_surrogatepair( buffer, addbytes, x);
   }

   return( 0);
}


//
// this also does not do any error checking, the UTF8 string must be perfect
//
// -1  check errno
//  0  OK!
//

int   mulle_utf8_bufferconvert_to_utf32( mulle_utf8_t *src,
                                         size_t len,
                                         void *buffer,
                                         void (*addbytes)( void *, void *, size_t size))
{
   mulle_utf8_t   *next;
   mulle_utf8_t   *sentinel;
   mulle_utf8_t   _c;
   size_t         extra_len;
   mulle_utf32_t   x;

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)

   sentinel = &src[ len];

   while( src < sentinel)
   {
      _c = *src++;
      assert( mulle_utf8_get_startcharactertype( _c) != mulle_utf8_invalid_start_character);

      if( (char) _c >= 0)
      {
         x = _c;
         (*addbytes)( buffer, &x, sizeof( x));
         continue;
      }

      extra_len = mulle_utf8_get_extracharacterslength( _c);
      next      = &src[ extra_len];
      if( next > sentinel)
      {
         errno = EINVAL;
         return( -1);
      }

      x   = mulle_utf8_extracharactersvalue( src - 1, extra_len);
      src = next;
      (*addbytes)( buffer, &x, sizeof( x));
   }

   return( 0);
}


//
// make it more optimal, by (a) checking that pointer can be accessed with
// a long or long long
// (b) masking value with 0x80808080 to figure out if all are "ASCII"
//
int  mulle_utf8_information( mulle_utf8_t *src, size_t len, struct mulle_utf_information *info)
{
   mulle_utf8_t                   *start;
   mulle_utf8_t                   *end;
   mulle_utf8_t                   *sentinel;
   mulle_utf8_t                   _c;
   size_t                         dst_len;
   size_t                         extra_len;
   struct mulle_utf_information   dummy;
   mulle_utf32_t                  _x;

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
   if( ! src)
      goto fail;

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);

   //
   // remove leading BOM
   //

   info->has_bom = mulle_utf8_has_leading_bomcharacter( src, len);
   if( info->has_bom)
   {
      src += 3;
      len -= 3;
   }

   info->start = src;
   start       = src;
   sentinel    = &src[ len];
   dst_len     = len;

   for( ; src < sentinel; src++)
   {
      if( ! (_c = *src))
      {
         info->has_terminating_zero = 1;
         break;
      }

      if( mulle_utf8_is_asciicharacter( _c))
      {
         if( info->is_char5 && ! mulle_utf8_is_char5character( _c))
            info->is_char5 = 0;
         continue;
      }

      info->is_ascii = 0;
      if( mulle_utf8_is_invalidstartcharacter( _c))
         goto fail;

      extra_len = mulle_utf8_get_extracharacterslength( _c);
      dst_len  -= extra_len;  // reduce character count

      end = &src[ extra_len];
      if( end >= sentinel)
         goto fail;

      if( ! mulle_utf8_are_valid_extracharacters( src, extra_len, &_x))
         goto fail;
      if( _x >= 0x08000)
         info->is_utf15 = 0;
      if( _x >= 0x10000)
         info->utf16len += 1;

//
//      if( ! mulle_utf8_are_valid_extra_chars( src, extra_len))
//      {
//         info->invalid_utf8 = src;
//         return( -1);
//      }
      src = end;
   }

   info->utf8len   = src - start;                      // actual UTF8 strlen
   info->utf32len  = dst_len - (len - info->utf8len);  // number of characters
   info->utf16len += info->utf32len;                   // size in utf16 with escapes
   info->is_char5 &= info->is_ascii;

   return( 0);

fail:
   memset( info, 0, sizeof( *info));
   info->invalid = src;
   return( -1);
}


int   mulle_utf8_is_ascii( mulle_utf8_t *src, size_t len)
{
   mulle_utf8_t   *sentinel;

   if( ! src)
      return( 0);

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);

   sentinel = &src[ len];

   while( src < sentinel)
      if( ! mulle_utf8_is_asciicharacter( *src++))
         return( 0);

   return( 1);
}

//
// this routine does not validate...
//
size_t  mulle_utf8_utf16length( mulle_utf8_t *src, size_t len)
{
   mulle_utf8_t   _c;
   mulle_utf8_t   *sentinel;
   mulle_utf8_t   *end;
   size_t          extra_len;
   size_t          dst_len;

   if( ! src)
      return( 0);

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);

   sentinel = &src[ len];
   dst_len  = len;

   for( ; src < sentinel; src++)
   {
      _c = *src;
      assert( mulle_utf8_get_startcharactertype( _c) != mulle_utf8_invalid_start_character);

      if( (char) _c >= 0)
         continue;

      // 32 bit ?
      if( _c >= 0xF0)
         dst_len++;

      extra_len = mulle_utf8_get_extracharacterslength( _c);
      dst_len  -= extra_len;

      end       = &src[ extra_len];
      if( end >= sentinel)
         return( -1);
#ifndef NDEBUG
      do
      {
         _c = *++src;
         assert( mulle_utf8_is_validcontinuationcharacter( _c));
      }
      while( src < end);
#else
      src = end;
#endif
   }
   return( dst_len);
}




//
// different API to strnstr, and on linux strnstr is only available
// with BSD...
// You can't search for '\0' with this function.
//
mulle_utf8_t   *mulle_utf8_strnstr( mulle_utf8_t *s, size_t len, mulle_utf8_t *search)
{
   mulle_utf8_t      *sentinel;
   mulle_utf8_t      *p;
   ssize_t           offset;

   if( ! s || ! search)
      return( NULL);

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( s);

   offset   = mulle_utf8_strlen( search);
   if( ! offset)
      return( NULL);

   sentinel = &s[ len];
   p        = search;

   // fprintf( stderr, "# s=\"%s\" search=\"%s\" len=%ld, offset=%ld, sentinel=\"%s\"\n",
   //            s, search, (long) len, (long) offset, sentinel);

   for(;;)
   {
      if( s >= sentinel)
         return( NULL);

      if( *s++ != *p)
      {
         p = search;
         continue;
      }

      if( *++p)
         continue;

      return( &s[ -offset]);
   }
}


// 0 is no terminator in this case
mulle_utf8_t  *mulle_utf8_strnchr( mulle_utf8_t *s, size_t len, mulle_utf8_t c)
{
   mulle_utf8_t   *start;
   mulle_utf8_t   *sentinel;

   if( ! s)
      return( NULL);

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( s);

   start    = s;
   sentinel = &s[ len];

   while( s < sentinel)
   {
      if( *s == c)
         return( s);
      ++s;
   }
   return( NULL);
}


size_t   mulle_utf8_strnspn( mulle_utf8_t *s, size_t len, mulle_utf8_t *search)
{
   mulle_utf8_t   *start;
   mulle_utf8_t   *sentinel;
   size_t         search_len;

   if( ! s)
      return( 0);

   if( ! search)
      return( 0);
   search_len = mulle_utf8_strlen( search);

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( s);

   start    = s;
   sentinel = &s[ len];

   while( s < sentinel)
   {
      if( ! mulle_utf8_strnchr( search, search_len, *s))
         break;
      ++s;
   }
   return( s - start);
}


size_t   mulle_utf8_strncspn( mulle_utf8_t *s, size_t len, mulle_utf8_t *search)
{
   mulle_utf8_t   *start;
   mulle_utf8_t   *sentinel;
   size_t         search_len;

   if( ! s)
      return( 0);

   if( len == (size_t) -1)
      len = mulle_utf8_strlen( s);

   if( search)
   {
      search_len = mulle_utf8_strlen( search);

      start    = s;
      sentinel = &s[ len];

      while( s < sentinel)
      {
         if( mulle_utf8_strnchr( search, search_len, *s))
            return( s - start);
         ++s;
      }
   }
   return( len);
}
