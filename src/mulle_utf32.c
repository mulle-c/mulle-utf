//
//  mulle_utf32.c
//  mulle-utf
//
//  Created by Nat! on 19.03.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#include "mulle_utf32.h"

#include <assert.h>
#include <string.h>


static inline int   mulle_utf32_is_ascii_char( mulle_utf32char_t c)
{
   return( c < 0x80);
}


size_t  mulle_utf32_strlen( mulle_utf32char_t *src)
{
   size_t   len;

   len = 0;
   if( src)
      for( len = 0; *src++; len++);
   return( len);
}


size_t  mulle_utf32_strnlen( mulle_utf32char_t *src, size_t len)
{
   mulle_utf32char_t   *sentinel;
   
   sentinel = &src[ len];
   len = 0;
   while( src < sentinel)
   {
      if( ! *src)
         break;
      ++len;
   }
   return( len);
}


void  _mulle_utf32_encode_as_surrogatepair_into_utf16_bytebuffer( void *buffer,
                                                                  void (*adduint16)( void *, uint16_t),
                                                                  mulle_utf32char_t x)
{
   mulle_utf32char_t  top;
   mulle_utf32char_t  bottom;
   mulle_utf32char_t  hi;
   mulle_utf32char_t  lo;
   
   assert( x >= 0x10000 && x <= 0x10FFFF);
   
   x -= 0x10000;
   
   assert( (x >> 10) <= 0x3FF);
   
   top    = (mulle_utf32char_t) (x >> 10);
   bottom = (mulle_utf32char_t) (x & 0x3FF);
   
   hi = 0xD800 + top;
   lo = 0xDC00 + bottom;

   assert( hi >= 0xD800 && hi < 0xDC00);
   assert( lo >= 0xDC00 && lo < 0xE000);

#if __LITTLE_ENDIAN__      // should've been called __LITTLE_END_FIRST__
   (*adduint16)( buffer, (uint16_t) lo);
   (*adduint16)( buffer, (uint16_t) hi);
#else
   (*adduint16)( buffer, (uint16_t) hi);
   (*adduint16)( buffer, (uint16_t) lo);
#endif
}


// must be proper UTF32 code!
size_t   mulle_utf32_length_as_utf8( mulle_utf32char_t *src,
                                     size_t len)
{
   mulle_utf32char_t       *sentinel;
   mulle_utf32char_t       x;
   
   if( ! src)
      return( 0);
   
   if( len == (size_t) -1)
      len = mulle_utf32_strlen( src);
   if( ! len)
      return( 0);
             
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)
   
   sentinel = &src[ len];
   
   if( mulle_utf32_is_bom_char( *src))
   {
      src += 1;
      len -= 1;
   }
   
   while( src < sentinel)
   {
      x = *src++;
      
      if( x < 0x800)
      {
         if( x < 0x80)
            continue;

         ++len;
         continue;
      }

      if( x < 0x10000)
      {
         assert( x >= 0xD800 || x < 0xE000);
         len += 2;
         continue;
      }
      
      assert( x <= 0x10FFFF);
      len += 3;
   }
   return( len);
}



int   mulle_utf32_information( mulle_utf32char_t *src, size_t len, struct mulle_utf32_information *info)
{
   mulle_utf32char_t                        _c;
   mulle_utf32char_t                        *sentinel;
   struct mulle_utf32_information   dummy;
   
   if( ! info)
      info = &dummy;

   if( ! src)
      goto fail;

   //
   // remove leading BOM
   //
   info->has_bom = mulle_utf32_is_bom_char( *src);
   if( info->has_bom)
   {
      src += 1;
      len -= 1;
   }

   info->has_terminating_zero = 0;
   info->invalid_utf32        = NULL;
   info->start                = src;
   info->is_ascii             = 1;
   info->utf8len              =
   info->utf32len             =
   info->utf16len             = len;

   sentinel = &src[ len];
   
   for( ; src < sentinel; src++)
   {
      if( ! (_c = *src))
      {  
         info->has_terminating_zero = 1;
         break;
      }
      
      if( mulle_utf32_is_ascii_char( _c))
         continue;
      
      info->is_ascii = 0;
      info->utf8len++;

      if( _c >= 0x0800)
         info->utf8len++;

      
#if FORBID_NON_CHARACTERS
      if( c >= 0xD800 && c <= 0xE000)   // utf-16 surrogate pair
         goto fail;

      if( _c >= 0xFFFE || _c >= 0xFDD0 && _c <= 0xFDEF)
         goto fail;
#endif
     
      if( _c >= 0x10000)
      {
         info->utf8len++;
         info->utf16len++;
      }
   }

   return( 0);

fail:
   memset( info, 0, sizeof( *info));
   info->invalid_utf32 = src;
   return( -1);
}


// must be proper UTF32 code!
int  _mulle_utf32_convert_to_utf8_bytebuffer( void *buffer,
                                              void *(*advance)( void *, size_t),
                                              mulle_utf32char_t *src,
                                              size_t len)
{
   mulle_utf32char_t   *sentinel;
   mulle_utf8char_t    *s;
   mulle_utf32char_t   x;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf32_strlen( src);
   if( ! len)
      return( 0);
             
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)
   
   sentinel = &src[ len];
   
   if( mulle_utf32_is_bom_char( *src))
      src += 1;
   
   while( src < sentinel)
   {
      x = *src++;
      
      if( x < 0x800)
      {
         if( x < 0x80)
         {
            if( ! (s = (*advance)( buffer, 1)))
               return( -1);
            
            s[ 0] = (mulle_utf8char_t) x;
            continue;
         }

         if( ! (s = (*advance)( buffer, 2)))
            return( -1);
         
         s[ 0] = 0xC0 | (mulle_utf8char_t) (x >> 6);
         s[ 1] = 0x80 | (x & 0x3F);
         continue;
      }
      else
      {
         if( x < 0x10000)
         {
            assert( x >= 0xD800 || x < 0xE000);

            if( ! (s = (*advance)( buffer, 3)))
               return( -1);
         
            s[ 0] = 0xE0 | (mulle_utf8char_t) (x >> 12);
            s[ 1] = 0x80 | ((x >> 6) & 0x3F);
            s[ 2] = 0x80 | (x & 0x3F);
            continue;
         }

         assert( x <= 0x10FFFF);
         
         if( ! (s = (*advance)( buffer, 4)))
            return( -1);
         
         s[ 0] = 0xF0 | (mulle_utf8char_t) (x >> 18);
         s[ 1] = 0x80 | ((x >> 12) & 0x3F);
         s[ 2] = 0x80 | ((x >> 6) & 0x3F);
         s[ 3] = 0x80 | (x & 0x3F);
      }
   }
   return( 0);
}



int  _mulle_utf32_convert_to_utf16_bytebuffer( void *buffer,
                                               void (*adduint16)( void *, uint16_t),
                                               mulle_utf32char_t *src,
                                               size_t len)
{
   mulle_utf32char_t       *sentinel;
   mulle_utf32char_t       x;
   
   assert( src);
   
   if( len == (size_t) -1)
      len = mulle_utf32_strlen( src);
   if( ! len)
      return( 0);
   
   // if dst_len == -1, then sentinel - 1 = dst_sentinel (OK!)
   
   sentinel = &src[ len];
   
   if( mulle_utf32_is_bom_char( *src))
      src += 1;
   
   while( src < sentinel)
   {
      x = *src++;
      
      if( x < 0x10000)
      {
         assert( x >= 0xD800 || x < 0xE000);
         
         (*adduint16)( buffer, (uint16_t) x);
         continue;
      }
      _mulle_utf32_encode_as_surrogatepair_into_utf16_bytebuffer( buffer, adduint16, x);
   }
   return( 0);
}



