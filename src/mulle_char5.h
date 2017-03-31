//
//  mulle_char5.h
//  mulle-utf
//
//  Created by Nat! on 02.05.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_char5_h__
#define mulle_char5_h__

#include "mulle_utf_type.h"

#include <assert.h>


//
// char5 is a compression scheme that places small strings
// into 32 bit or 64 bit words
// not all small strings are compatible!
//
// 32 bit can hold up to 6 chars  with 2 bits left over
// 64 bit can hold up to 12 chars with 4 bits left over
//
int   mulle_char5_encode_character( int c);

enum
{
   mulle_char5_maxlength32 = 6,
   mulle_char5_maxlength64 = 12
};


static inline char   *mulle_char5_get_charset( void)
{
   static const char  table[] =
   {
       0,  '.', 'A', 'C',  'D', 'E', 'I', 'N',
      'O', 'P', 'S', 'T',  '_', 'a', 'b', 'c',
      'd', 'e', 'f', 'g',  'h', 'i', 'l', 'm',
      'n', 'o', 'p', 'r',  's', 't', 'u', 'y',
      0  // bonus zero for tests :)
   };
   return( (char *) table);
}


static inline int   mulle_char5_decode_character( int c)
{
   assert( c >= 0 && c < 32);

   return( mulle_char5_get_charset()[ c]);
}


int   mulle_char5_is_char5string32( char *src, size_t len);
int   mulle_char5_is_char5string64( char *src, size_t len);

uint32_t   mulle_char5_encode32( char *src, size_t len);
uint64_t   mulle_char5_encode64( char *src, size_t len);

size_t   mulle_char5_decode32( uint32_t value, char *dst, size_t len);
size_t   mulle_char5_decode64( uint64_t value, char *src, size_t len);

int   mulle_char5_get64( uint64_t value, unsigned int index);
int   mulle_char5_get32( uint32_t value, unsigned int index);


static inline size_t   mulle_char5_strlen64( uint64_t value)
{
   size_t   len;

   len = 0;
   while( value)
   {
      value >>= 5;
      ++len;
   }
   return( len);
}


static inline size_t  mulle_char5_strlen32( uint32_t value)
{
   size_t   len;

   len = 0;
   while( value)
   {
      value >>= 5;
      ++len;
   }
   return( len);
}


static inline uint64_t   mulle_char5_substring64( uint64_t value, unsigned int location, unsigned int length)
{
   assert( location + length <= mulle_char5_strlen64( value));

   value >>= location * 5;
   value  &= ~((~(uint64_t) 0) << (length * 5));
   return( value);
}


static inline uint32_t   mulle_char5_substring32( uint32_t value, unsigned int location, unsigned int length)
{
   assert( location + length <= mulle_char5_strlen32( value));

   value >>= location * 5;
   value  &= ~((~(uint32_t) 0) << (length * 5));
   return( value);
}


# pragma mark -
# pragma mark uintptr_t interface

static inline int   mulle_char5_is_char5string( char *src, size_t len)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( mulle_char5_is_char5string32( src, len));
   return( mulle_char5_is_char5string64( src, len));
}


static inline mulle_char5_t   mulle_char5_encode( char *src, size_t len)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( (mulle_char5_t) mulle_char5_encode32( src, len));
   return( (mulle_char5_t) mulle_char5_encode64( src, len));
}


static inline size_t   mulle_char5_decode( mulle_char5_t value, char *src, size_t len)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( mulle_char5_decode32( (uint32_t) value, src, len));
   return( mulle_char5_decode64( value, src, len));
}


static inline int   mulle_char5_get( mulle_char5_t value, unsigned int index)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( mulle_char5_get32( (uint32_t) value, index));
   return( mulle_char5_get64( value, index));
}


static inline size_t   mulle_char5_strlen( mulle_char5_t value)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( mulle_char5_strlen32( (uint32_t) value));
   return( mulle_char5_strlen64( value));
}


static inline size_t  mulle_char5_get_maxlength( void)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( mulle_char5_maxlength32);
   return( mulle_char5_maxlength64);
}


static inline mulle_char5_t  mulle_char5_substring( mulle_char5_t value,
                                                    unsigned int location,
                                                    unsigned int length)
{
   if( sizeof( mulle_char5_t) == sizeof( uint32_t))
      return( (mulle_char5_t) mulle_char5_substring32( (uint32_t) value, location, length));
   return( (mulle_char5_t) mulle_char5_substring64( value, location, length));
}

#endif
