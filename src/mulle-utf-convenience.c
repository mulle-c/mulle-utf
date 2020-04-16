//
//  mulle_utf_convenience.c
//  mulle-utf
//
//  Created by Nat! on 30.05.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#include "mulle-utf-convenience.h"

#include "mulle-utf8.h"
#include "mulle-utf16.h"
#include "mulle-utf32.h"

#include <errno.h>


struct buffer
{
   char     *s;
   size_t   n;
};


//
// len will be #bytes
//
static void  buffer_add( struct buffer *p, void *bytes, size_t len)
{
   memmove( &p->s[ p->n], bytes, len);
   p->n += len;
}


# pragma mark -
# pragma mark utf8

mulle_utf16_t  *mulle_utf8_convert_to_utf16( mulle_utf8_t *src,
                                             size_t len,
                                             struct mulle_allocator *allocator)
{
   struct mulle_utf_information   info;
   void                           *memo;
   struct buffer                  tmp;

   if( mulle_utf8_information( src, len, &info))
   {
      errno = EINVAL;
      return( NULL);
   }

   memo = mulle_allocator_malloc( allocator, sizeof( mulle_utf16_t) * (info.utf16len + 1));

   tmp.s = memo;
   tmp.n = 0;

   if( mulle_utf8_bufferconvert_to_utf16( info.start, info.utf8len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   // add trailing zero
   tmp.s[ tmp.n]     = 0;
   tmp.s[ tmp.n + 1] = 0;

   return( memo);
}


mulle_utf32_t  *mulle_utf8_convert_to_utf32( mulle_utf8_t *src,
                                             size_t len,
                                             struct mulle_allocator *allocator)
{
   struct mulle_utf_information   info;
   void                           *memo;
   struct buffer                  tmp;

   if( mulle_utf8_information( src, len, &info))
   {
      errno = EINVAL;
      return( NULL);
   }

   memo = mulle_allocator_malloc( allocator, sizeof( mulle_utf32_t) * (info.utf32len + 1));

   tmp.s = memo;
   tmp.n = 0;

   if( mulle_utf8_bufferconvert_to_utf32( info.start, info.utf8len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   // add trailing zero
   tmp.s[ tmp.n]     = 0;
   tmp.s[ tmp.n + 1] = 0;
   tmp.s[ tmp.n + 2] = 0;
   tmp.s[ tmp.n + 3] = 0;

   return( memo);
}


# pragma mark -
# pragma mark utf16

mulle_utf8_t  *mulle_utf16_convert_to_utf8( mulle_utf16_t *src,
                                            size_t len,
                                            struct mulle_allocator *allocator)
{
   struct mulle_utf_information   info;
   void                           *memo;
   struct buffer                  tmp;

   if( mulle_utf16_information( src, len, &info))
   {
      errno = EINVAL;
      return( NULL);
   }

   memo = mulle_allocator_malloc( allocator, sizeof( mulle_utf8_t) * (info.utf8len + 1));

   tmp.s = memo;
   tmp.n = 0;

   if( mulle_utf16_bufferconvert_to_utf8( info.start, info.utf16len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   // add trailing zero
   tmp.s[ tmp.n] = 0;

   return( memo);
}


mulle_utf32_t  *mulle_utf16_convert_to_utf32( mulle_utf16_t *src,
                                              size_t len,
                                              struct mulle_allocator *allocator)
{
   struct mulle_utf_information   info;
   void                           *memo;
   struct buffer                  tmp;

   if( mulle_utf16_information( src, len, &info))
   {
      errno = EINVAL;
      return( NULL);
   }

   memo = mulle_allocator_malloc( allocator, sizeof( mulle_utf32_t) * (info.utf32len + 1));

   tmp.s = memo;
   tmp.n = 0;

   if( mulle_utf16_bufferconvert_to_utf32( info.start, info.utf16len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   // add trailing zero
   tmp.s[ tmp.n]     = 0;
   tmp.s[ tmp.n + 1] = 0;
   tmp.s[ tmp.n + 2] = 0;
   tmp.s[ tmp.n + 3] = 0;

   return( memo);
}


# pragma mark -
# pragma mark utf32

mulle_utf8_t  *mulle_utf32_convert_to_utf8( mulle_utf32_t *src,
                                            size_t len,
                                            struct mulle_allocator *allocator)
{
   struct mulle_utf_information   info;
   void                           *memo;
   struct buffer                  tmp;

   if( mulle_utf32_information( src, len, &info))
   {
      errno = EINVAL;
      return( NULL);
   }

   memo  = mulle_allocator_malloc( allocator, sizeof( mulle_utf8_t) * (info.utf8len + 1));

   tmp.s = memo;
   tmp.n = 0;

   if( mulle_utf32_bufferconvert_to_utf8( info.start, info.utf32len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   tmp.s[ tmp.n] = 0;

   return( memo);
}



mulle_utf16_t  *mulle_utf32_convert_to_utf16( mulle_utf32_t *src,
                                              size_t len,
                                              struct mulle_allocator *allocator)
{
   struct mulle_utf_information   info;
   void                           *memo;
   struct buffer                  tmp;

   if( mulle_utf32_information( src, len, &info))
   {
      errno = EINVAL;
      return( NULL);
   }

   memo  = mulle_allocator_malloc( allocator, sizeof( mulle_utf16_t) * (info.utf16len + 1));

   tmp.s = memo;
   tmp.n = 0;

   if( mulle_utf32_bufferconvert_to_utf16( info.start, info.utf32len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   // add trailing zero
   tmp.s[ tmp.n]     = 0;
   tmp.s[ tmp.n + 1] = 0;

   return( memo);
}



#pragma mark -  support for -toLower

// ctype_convert will always add a zero (at buf[ len])
struct mulle_utf8_data
   mulle_utf8_character_convert( mulle_utf8_t *src,
                                 size_t len,
                                 mulle_utf32_t (*f_conversion)( mulle_utf32_t),
                                 struct mulle_allocator *allocator)
{
   mulle_utf8_t            *p;
   mulle_utf8_t            *sentinel;
   mulle_utf32_t           c;
   struct mulle_utf8_data  buf;

   // mulle_utf32_t can become 4 bytes max
   buf.length     = (len * 4) + 1;
   buf.characters = mulle_allocator_malloc( allocator, buf.length);
   p              = buf.characters;

   sentinel = &src[ len];
   while( src < sentinel)
   {
      c = mulle_utf8_next_utf32character( &src);
      c = (*f_conversion)( c);
      p = mulle_utf32_as_utf8( c, p);
   }

   *p++ = 0;
   assert( p <= &buf.characters[ buf.length]);

   buf.length     = p - buf.characters;
   buf.characters = mulle_allocator_realloc( allocator, buf.characters, buf.length);
   return( buf);
}


struct mulle_utf8_data
   mulle_utf8_word_convert( mulle_utf8_t *src,
                            size_t len,
                            mulle_utf32_t (*f1_conversion)( mulle_utf32_t),
                            mulle_utf32_t (*f2_conversion)( mulle_utf32_t),
                            int           (*is_white)( mulle_utf32_t),
                            struct mulle_allocator *allocator)
{
   int                     is_start;
   mulle_utf32_t           c;
   mulle_utf8_t            *p;
   mulle_utf8_t            *sentinel;
   struct mulle_utf8_data  buf;

   is_start = 1;

   // mulle_utf32_t can become 4 bytes max
   buf.length     = (len * 4) + 1;
   buf.characters = mulle_allocator_malloc( allocator, buf.length);
   p              = buf.characters;

   sentinel = &src[ len];
   while( src < sentinel)
   {
      c = mulle_utf8_next_utf32character( &src);
      if( (*is_white)( c))
         is_start = 1;
      else
      {
         if( is_start)
         {
            c = (*f1_conversion)( c);
            is_start = 0;
         }
         else
            c = (*f2_conversion)( c);
      }
      p = mulle_utf32_as_utf8( c, p);
   }
   *p++ = 0;
   assert( p <= &buf.characters[ buf.length]);

   buf.length     = p - buf.characters;
   buf.characters = mulle_allocator_realloc( allocator, buf.characters, buf.length);
   return( buf);
}
