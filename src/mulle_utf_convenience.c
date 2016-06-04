//
//  mulle_utf_convenience.c
//  mulle-utf
//
//  Created by Nat! on 30.05.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#include "mulle_utf_convenience.h"

#include "mulle_utf8.h"
#include "mulle_utf16.h"
#include "mulle_utf32.h"

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
   memcpy( &p->s[ p->n], bytes, len);
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
   if( ! memo)
      return( NULL);
   
   tmp.s = memo;
   tmp.n = 0;
   
   if( mulle_utf8_convert_to_utf16_bytebuffer( info.start, info.utf8len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

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
   if( ! memo)
      return( NULL);
   
   tmp.s = memo;
   tmp.n = 0;
   
   if( mulle_utf8_convert_to_utf32_bytebuffer( info.start, info.utf8len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

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
   if( ! memo)
      return( NULL);
   
   tmp.s = memo;
   tmp.n = 0;
   
   if( mulle_utf16_convert_to_utf8_bytebuffer( info.start, info.utf16len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

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
   if( ! memo)
      return( NULL);
   
   tmp.s = memo;
   tmp.n = 0;
   
   if( mulle_utf16_convert_to_utf32_bytebuffer( info.start, info.utf16len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

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
   
   memo = mulle_allocator_malloc( allocator, sizeof( mulle_utf8_t) * (info.utf8len + 1));
   if( ! memo)
      return( NULL);
   
   tmp.s = memo;
   tmp.n = 0;
   
   if( mulle_utf32_convert_to_utf8_bytebuffer( info.start, info.utf32len, &tmp, (void *) buffer_add))
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
   
   memo = mulle_allocator_malloc( allocator, sizeof( mulle_utf16_t) * (info.utf16len + 1));
   if( ! memo)
      return( NULL);
   
   tmp.s = memo;
   tmp.n = 0;
   
   if( mulle_utf32_convert_to_utf16_bytebuffer( info.start, info.utf32len, &tmp, (void *) buffer_add))
   {
      mulle_allocator_free( allocator, memo);
      errno = EINVAL;
      return( NULL);
   }

   tmp.s[ tmp.n]     = 0;
   tmp.s[ tmp.n + 1] = 0;

   return( memo);
}

