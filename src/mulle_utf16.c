//
//  mulle_utf16.c
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

#include "mulle_utf16.h"

#include <errno.h>


#define FORBID_NON_CHARACTERS  1


int  mulle_utf16_is_valid_surrogatepair( utf16char hi, utf16char lo)
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

void  mulle_utf16_encode_surrogatepair_into_bytebuffer( struct mulle_bytebuffer *dst, utf32char x)
{
   unichar  top;
   unichar  bottom;
   unichar  hi;
   unichar  lo;
   
   assert( x >= 0x10000 && x <= 0x10FFFF);
   
   x -= 0x10000;
   
   assert( (x >> 10) <= 0x3FF);
   
   top    = (unichar) (x >> 10);
   bottom = (unichar) (x & 0x3FF);
   
   hi = 0xD800 + top;
   lo = 0xDC00 + bottom;

   assert( hi >= 0xD800 && hi < 0xDC00);
   assert( lo >= 0xDC00 && lo < 0xE000);

#if __LITTLE_ENDIAN__      // should've been called __LITTLE_END_FIRST__
   _mulle_bytebuffer_add_uint16( dst, (uint16_t) lo);
   _mulle_bytebuffer_add_uint16( dst, (uint16_t) hi);
#else
   _mulle_bytebuffer_add_uint16( dst, (uint16_t) hi);
   _mulle_bytebuffer_add_uint16( dst, (uint16_t) lo);
#endif
}


// must be proper UTF16 code!
size_t   _mulle_utf16_convert_to_utf8_bytebuffer( struct mulle_bytebuffer *dst, utf16char *src, size_t *p_len)
{
   utf16char       *sentinel;
   unsigned char   *s;
   utf32char       x;
   size_t          len;
   utf16char       *src_start;
   
   assert( src);
   
   len = *p_len;
   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   if( ! len)
      return( 0);
             
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)
   
   src_start    = src;
   sentinel     = &src[ len];
   
   if( mulle_utf16_is_bom( *src))
      src += 1;
   
   while( src < sentinel)
   {
      x = *src++;
      if( x >= 0xD800 && x < 0xDC00)  // hi surrogate
      {
         // decode surrogate
         if( src >= sentinel)
         {
            errno = EFAULT;
            return( -1);
         }
         x = mulle_utf16_decode_surrogatepair( (utf16char) x, *src++);
      }
      
      // 4 cases
      // < 0x80, < 0x800 < 0x10000, > 0x10000
      if( x < 0x800)
      {
         if( x < 0x80)
         {
            _mulle_bytebuffer_add( dst, (utf8char) x);
            continue;
         }

         if( ! (s = _mulle_bytebuffer_guarantee( dst, 2)))
            goto exhausted;
         
         s[ 0] = 0xC0 | (utf8char) (x >> 6);
         s[ 1] = 0x80 | (x & 0x3F);
         continue;
      }
      else
      {
         if( x < 0x10000)
         {
            assert( x >= 0xD800 || x < 0xE000);

            if( ! (s = _mulle_bytebuffer_guarantee( dst, 3)))
               goto exhausted;
         
            s[ 0] = 0xE0 | (utf8char) (x >> 12);
            s[ 1] = 0x80 | ((x >> 6) & 0x3F);
            s[ 2] = 0x80 | (x & 0x3F);
            continue;
         }

         assert( x <= 0x10FFFF);
         
         if( ! (s = _mulle_bytebuffer_guarantee( dst, 4)))
            goto exhausted;
         
         s[ 0] = 0xF0 | (utf8char) (x >> 18);
         s[ 1] = 0x80 | ((x >> 12) & 0x3F);
         s[ 2] = 0x80 | ((x >> 6) & 0x3F);
         s[ 3] = 0x80 | (x & 0x3F);
      }
   }
   
   return( _mulle_bytebuffer_get_length( dst));
   
exhausted:
   _mulle_bytebuffer_zero_last_byte( dst);
   
   *p_len = &src[ -1] - src_start;
   errno = EACCES;
   return( -1);
}



size_t   _mulle_utf16_convert_to_utf8( utf8char *dst, size_t dst_len, utf16char *src, size_t *p_len)
{
   struct mulle_bytebuffer    tmp;
   size_t           rval;

   assert( dst);
   assert( src);
   
   _mulle_bytebuffer_inflexable_init_with_static_bytes( &tmp, dst, dst_len);
   rval = _mulle_utf16_convert_to_utf8_bytebuffer( &tmp, src, p_len);
   _mulle_bytebuffer_done( &tmp);
   return( rval);
}

//
// just checks that the surrogate pairs are ok
//
utf16char  *mulle_utf16_validate( utf16char *src, size_t len)
{
   utf16char   c;
   utf16char   d;
   utf16char   *sentinel;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   if( ! len)
      return( 0);
   
   sentinel = &src[ len];
   
   if( mulle_utf16_is_bom( *src))
      src += 1;
   
   for( ; src < sentinel; src++)
   {
      c = *src;

      // non characters, do not want
#if FORBID_NON_CHARACTERS
      if( c >= 0xFFFE || c >= 0xFDD0 && c <= 0xFDEF)
         return( src);
#endif      
      if( c < 0xD800 || c >= 0xE000)
         continue;
      
      if( c >= 0xDC00)
         return( src);
      if( src >= sentinel)
         return( src);
      d = *++src;
      if( d < 0xDC00 || d >= 0xE000)
         return( src);
      if( ! mulle_utf16_is_valid_surrogatepair( c, d))
         return( src - 1);
   }
   return( 0);
}

//
// this routine does not validate...
//
size_t  mulle_utf16_length_as_utf8( utf16char *src, size_t len)
{
   utf16char   c;
   utf16char   *sentinel;
   size_t    dst_len;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   if( ! len)
      return( 0);
   
   if( mulle_utf16_is_bom( *src))
   {
      src += 1;
      len -= 1;
   }
   
   sentinel = &src[ len];
   dst_len  = len;
   
   for( ; src < sentinel;)
   {
      c = *src++;
      
      if( c < 0x80)
         continue;
         
      if( c < 0x0800)
      {
         dst_len++;
         continue;
      }

      // not a surrogate pair ?
      if( c < 0xD800 || c > 0xE000)
      {
         dst_len += 2;
         continue;
      }
      
      if( ++src > sentinel)
         return( -1);
         
      dst_len += -1 + 3;      
   }
   return( dst_len);
}


size_t  mulle_utf16_strlen( utf16char *src)
{
   size_t   len;

   len = 0;
   if( src)
      for( len = 0; *src++; len++);
   return( len);
}


size_t  mulle_utf16_length( utf16char *src, size_t len)
{
   utf16char   c;
   utf16char   *sentinel;
   size_t    dst_len;
   
   assert( src);
   assert( sizeof( wchar_t) == sizeof( utf16char));
   
   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   if( ! len)
      return( 0);
   
   if( mulle_utf16_is_bom( *src))
   {
      src += 1;
      len -= 1;
   }
   
   sentinel = &src[ len];
   dst_len  = len;
   
   for( ; src < sentinel;)
   {
      c = *src++;
      
      if( c < 0x0800)
         continue;

      // not a surrogate pair ?
      if( c < 0xD800 || c > 0xE000)
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


int   mulle_utf16_is_ascii( utf16char *src, size_t len)
{
   utf16char   _c;
   utf16char   *sentinel;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf16_strlen( src);
   
   if( ! len)
      return( 1);
      
   if( mulle_utf16_is_bom( *src))
   {
      src += 1;
      len -= 1;
   }
   
   sentinel = &src[ len];
   
   while( src < sentinel)
   {
      _c = *src++;
      if( _c >= 0x80)
         return( 0);
   }

   return( 1);
}
