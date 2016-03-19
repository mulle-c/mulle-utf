//
//  mulle_utf8.c
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
#include "mulle_utf8.h"

#include "mulle_utf16.h"
#include "mulle_utf32.h"
#include <errno.h>
#include <assert.h>


// it's not compatible to Apple
#define FORBID_NON_CHARACTERS  1


enum
{
   mulle_utf8_ascii_start_char,
   mulle_utf8_multiple_start_char,
   mulle_utf8_invalid_start_char
};


static inline int   mulle_utf8_is_ascii_char( utf8char c)
{
   return( (char) c >= 0);
}


static inline int   mulle_utf8_is_invalid_start_char( utf8char c)
{
   return( (c >= 0x80 && c < 0xC2) || c >= 0xF5);
}


static inline int   mulle_utf8_start_char_type( utf8char c)
{
   if( mulle_utf8_is_ascii_char( c))
      return( mulle_utf8_ascii_start_char);
   
   if( mulle_utf8_is_invalid_start_char( c))
      return( mulle_utf8_invalid_start_char);
   
   return( mulle_utf8_multiple_start_char);
}


static inline int   mulle_utf8_is_valid_continuation_char( utf8char c)
{
   return( c >= 0x80 && c < 0xC0);
}


static inline size_t  mulle_utf8_extra_characters_length( utf8char c)
{
   assert( mulle_utf8_start_char_type( c) == mulle_utf8_multiple_start_char);
   
   if( c < 0xE0)
      return( 1);  // 11 bits
   
   if( c < 0xF0)
      return( 2); //  16 bits
   
   return( 3);  // 21 bits -> UTF32
}



static utf32char   mulle_utf8_extra_chars_value( utf8char *src, size_t len)
{
   utf8char    _c;
   utf32char   x;
   
   assert( src);
   assert( len >=1 && len <= 3);
   
   _c = *src++;
   x  = 0;   

   switch( len)
   {
   case 1 : // 11 bits
      x   = (_c & 0x1F) << 6;
      
      _c = *src;
      assert( mulle_utf8_is_valid_continuation_char( _c));
      x  |= (_c & 0x3F);
         
      assert( x >= 128);
      break;
      
   case 2 :  // 16 bits
      x   = (_c & 0x0F) << 12;
      
      _c = *src++;
      assert( mulle_utf8_is_valid_continuation_char( _c));
      x  |= (_c & 0x3F) << 6;
      
      _c = *src;
      assert( mulle_utf8_is_valid_continuation_char( _c));
      x  |= (_c & 0x3F);
      
      assert( x >= 0x800 && (x < 0xD800 || x >= 0xE000));
      break;
      
   case 3 :   // 21 bits -> UTF32
      x   = (_c & 0x7) << 18;
      
      _c = *src++;
      assert( mulle_utf8_is_valid_continuation_char( _c));
      x  |= (_c & 0x3F) << 12;
      
      _c = *src++;
      assert( mulle_utf8_is_valid_continuation_char( _c));
      x  |= (_c & 0x3F) << 6;
      
      _c = *src;
      assert( mulle_utf8_is_valid_continuation_char( _c));
      x  |= (_c & 0x3F);
      
      assert( x >= 0x10000 && x <= 0x10FFFF);
   }        
   return( x);
}


//
// the slower non-crashing code ...
//
int   mulle_utf8_are_valid_extra_chars( char *src, unsigned int len)
{
   utf8char   _c;
   utf32char       x;
   
   assert( src);
   assert( len >= 1 && len <= 3);
   
   _c = *src++;
   
   switch( len)
   {
   case 1 : // 11 bits
      x  = (_c & 0x1F) << 6;
      
      _c = *src;
      if( ! mulle_utf8_is_valid_continuation_char( _c))
         return( 0);
         
      x  |= (_c & 0x3F);
      if( x < 0x80)
         return( 0);
      break;
      
   case 2 :  // 16 bits
      x  = (_c & 0x0F) << 12;
      
      _c = *src++;
      if( ! mulle_utf8_is_valid_continuation_char( _c))
         return( 0);
      x  |= (_c & 0x3F) << 6;
      
      _c = *src;
      if( ! mulle_utf8_is_valid_continuation_char( _c))
         return( 0);
      x  |= (_c & 0x3F);

      if( x < 0x800)
         return( 0);
      if( x >= 0xD800 && x < 0xE000)
         return( 0);
#if FORBID_NON_CHARACTERS         
      if( mulle_utf32_is_non_char( x))
         return( 0);
#endif         
      break;
      
   case 3 :   // 21 bits -> UTF32
      x   = (_c & 0x7) << 18;
      
      _c = *src++;
      if( ! mulle_utf8_is_valid_continuation_char( _c))
         return( 0);
      x  |= (_c & 0x3F) << 12;
      
      _c = *src++;
      if( ! mulle_utf8_is_valid_continuation_char( _c))
         return( 0);
      x  |= (_c & 0x3F) << 6;
      
      _c = *src;
      if( ! mulle_utf8_is_valid_continuation_char( _c))
         return( 0);
      x  |= (_c & 0x3F);

      if( x < 0x10000 || x > 0x0010FFFF)
         return( 0);
   }        
   return( 1);
}


//
// this also does not do any error checking, the UTF8 string must be perfect
//
// -1  check errno
//  0  OK!
//

int  mulle_utf8_convert_to_utf16_bytebuffer( void *buffer,
                                             void (*adduint16)( void *, uint16_t),
                                             utf8char *src,
                                             size_t len)
{
   utf8char   _c;
   uint32_t   x;
   utf8char   *next;
   utf8char   *sentinel;
   size_t     extra_len;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)
   
   sentinel = &src[ len];
   
   //
   // remove leading BOM
   //
   if( mulle_utf8_has_bom( src, len))
      src += 3;
   
   while( src < sentinel)
   {
      _c = *src++;
      assert( mulle_utf8_start_char_type( _c) != mulle_utf8_invalid_start_char);
      
      if( (char) _c >= 0)
      {
         (*adduint16)( buffer, _c);
         continue;
      }
      
      extra_len = mulle_utf8_extra_characters_length( _c);
      next      = &src[ extra_len];
      if( next > sentinel)
      {
         errno = EFAULT;
         return( -1);
      }   
            
      x   = mulle_utf8_extra_chars_value( src - 1, extra_len);
      src = next;
      if( x < 0x10000)
      {
         (*adduint16)( buffer, (uint16_t) x);
         continue;
      }
      
      mulle_utf32_encode_as_surrogatepair_into_utf16_bytebuffer( buffer, adduint16, x);
   }
   
   return( 0);
}


//
// make it more optimal, by (a) checking that pointer can be accessed with
// a long or long long
// (b) masking value with 0x80808080 to figure out if all are "ASCII"
//
size_t  mulle_utf8_information( utf8char *src, size_t len, struct mulle_utf8_information *info)
{
   utf8char                        _c;
   utf8char                        *sentinel;
   utf8char                        *end;
   size_t                          extra_len;
   size_t                          dst_len;
   struct mulle_utf8_information   dummy;
   
   assert( src);
   
   if( ! info)
      info = &dummy;

   info->has_terminating_zero = 0;
   info->invalid_utf8         = NULL;
   info->utf16len             = 0;
   info->utf32len             = 0;

//   if( len == (size_t) -1)
//   {
//      len = strlen( src);
//      info->has_terminating_zero = 1;
//   }
   
   //
   // remove leading BOM
   //
   info->has_bom = mulle_utf8_has_bom( src, len);
   if( info->has_bom)
   {
      src += 3;
      len -= 3;
   }
   info->start    = src;
   info->is_ascii = 1;

   sentinel = &src[ len];
   dst_len  = len;
   
   for( ; src < sentinel; src++)
   {
      if( ! (_c = *src))
      {  
         info->has_terminating_zero = 1;
         break;
      }
      
      if( mulle_utf8_is_ascii_char( _c))
         continue;

      info->is_ascii = 0;
      if( mulle_utf8_is_invalid_start_char( _c))
         goto fail;
      
      extra_len = mulle_utf8_extra_characters_length( _c);
      dst_len  -= extra_len;  // reduce character count
      
      end = &src[ extra_len];
      if( end >= sentinel)
         goto fail;

      if( mulle_utf8_extra_chars_value( src, extra_len) >= 0x10000)
         info->utf16len += 1;
         
//      
//      if( ! mulle_utf8_are_valid_extra_chars( src, extra_len))
//      {
//         info->invalid_utf8 = src;
//         return( -1);
//      }
      src = end;
   }

   info->utf8len   = src - info->start;                // actual UTF8 strlen
   info->utf32len  = dst_len - (len - info->utf8len);  // number of characters
   info->utf16len += info->utf32len;                   // size in utf16 with escapes

   return( info->utf8len);            // our "regular" -length

fail:
   memset( info, 0, sizeof( *info));
   info->invalid_utf8 = src;
   return( -1);
}


int   mulle_utf8_is_ascii( utf8char *src, size_t len)
{
   utf8char   *sentinel;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);

   if( mulle_utf8_has_bom( src, len))
   {
      src += 3;
      len -= 3;
   }
   
   sentinel = &src[ len];
   
   while( src < sentinel)
      if( ! mulle_utf8_is_ascii_char( *src++))
         return( 0);

   return( 1);
}

//
// this routine does not validate...
//
size_t  mulle_utf8_length_as_utf16( utf8char *src, size_t len)
{
   utf8char   _c;
   utf8char   *sentinel;
   utf8char   *end;
   size_t          extra_len;
   size_t          dst_len;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf8_strlen( src);
   
   sentinel = &src[ len];
   
   //
   // remove leading BOM
   //
   if( mulle_utf8_has_bom( src, len))
   {
      src += 3;
      len -= 3;
   }
   
   dst_len = len;
   
   for( ; src < sentinel; src++)
   {
      _c = *src;
      assert( mulle_utf8_start_char_type( _c) != mulle_utf8_invalid_start_char);
      
      if( (char) _c >= 0)
         continue;
      
      // 32 bit ?
      if( _c >= 0xF0)
         dst_len++;
      
      extra_len = mulle_utf8_extra_characters_length( _c);
      dst_len  -= extra_len;
      
      end       = &src[ extra_len];
      if( end >= sentinel)
         return( -1);
#ifndef NDEBUG      
      do 
      {
         _c = *++src;
         assert( mulle_utf8_is_valid_continuation_char( _c));
      }
      while( src < end);
#else
      src = end;
#endif      
   }
   return( dst_len);
}


// for debugging
utf32char   mulle_utf8_get_utf32_value( utf8char *s)
{
   switch( mulle_utf8_start_char_type( *s))
   {
   case mulle_utf8_ascii_start_char    : return( *s);
   case mulle_utf8_invalid_start_char  : return( -1);
   case mulle_utf8_multiple_start_char : return( mulle_utf8_extra_chars_value( s, mulle_utf8_extra_characters_length( *s)));
   }
   return( -1);
}
