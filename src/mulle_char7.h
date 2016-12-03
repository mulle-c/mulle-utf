//
//  mulle_char7.h
//  mulle-utf
//
//  Created by Nat! on 24.07.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_char7_h__
#define mulle_char7_h__

#include "mulle_utf_type.h"

#include <assert.h>

//
// char7 is a scheme that places small 7 bit ASCII strings
// into 32 bit or 64 bit words
//
// 32 bit can hold up to 4 chars with 4 bits left over
// 64 bit can hold up to 8 chars with 8 bits left over
//
enum
{
   mulle_char7_maxlength32 = 4,
   mulle_char7_maxlength64 = 8
};


int   mulle_char7_is_char7string32( char *src, size_t len);
int   mulle_char7_is_char7string64( char *src, size_t len);

uint32_t   mulle_char7_encode32( char *src, size_t len);
uint64_t   mulle_char7_encode64( char *src, size_t len);

size_t   mulle_char7_decode32( uint32_t value, char *dst, size_t len);
size_t   mulle_char7_decode64( uint64_t value, char *src, size_t len);

int   mulle_char7_get64( uint64_t value, unsigned int index);
int   mulle_char7_get32( uint32_t value, unsigned int index);


static inline size_t   mulle_char7_strlen64( uint64_t value)
{
   size_t   len;

   len = 0;
   while( value)
   {
      value >>= 7;
      ++len;
   }
   return( len);
}


static inline size_t  mulle_char7_strlen32( uint32_t value)
{
   size_t   len;

   len = 0;
   while( value)
   {
      value >>= 7;
      ++len;
   }
   return( len);
}


static inline uint64_t   mulle_char7_substring64( uint64_t value, unsigned int location, unsigned int length)
{
   assert( location + length <= mulle_char7_strlen64( value));

   value >>= location * 7;
   value  &= ~((~(uint64_t) 0) << (length * 7));
   return( value);
}


static inline uint32_t   mulle_char7_substring32( uint32_t value, unsigned int location, unsigned int length)
{
   assert( location + length <= mulle_char7_strlen32( value));

   value >>= location * 7;
   value  &= ~((~(uint32_t) 0) << (length * 7));
   return( value);
}


# pragma mark -
# pragma mark uintptr_t interface

// the naming... the naming...
// i will rename all this eventually
//
static inline int   mulle_char7_is_char7string( char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_is_char7string32( src, len));
   return( mulle_char7_is_char7string64( src, len));
}


static inline mulle_char7_t   mulle_char7_encode( char *src, size_t len)
{
   if( sizeof( mulle_char7_t) == sizeof( uint32_t))
      return( (mulle_char7_t) mulle_char7_encode32( src, len));
   return( (mulle_char7_t) mulle_char7_encode64( src, len));
}


static inline size_t   mulle_char7_decode( mulle_char7_t value, char *src, size_t len)
{
   if( sizeof( mulle_char7_t) == sizeof( uint32_t))
      return( mulle_char7_decode32( (uint32_t) value, src, len));
   return( mulle_char7_decode64( value, src, len));
}


static inline int   mulle_char7_get( mulle_char7_t value, unsigned int index)
{
   if( sizeof( mulle_char7_t) == sizeof( uint32_t))
      return( mulle_char7_get32( (uint32_t) value, index));
   return( mulle_char7_get64( value, index));
}


static inline size_t  mulle_char7_strlen( mulle_char7_t value)
{
   if( sizeof( mulle_char7_t) == sizeof( uint32_t))
      return( mulle_char7_strlen32( (uint32_t) value));
   return( mulle_char7_strlen64( value));
}


static inline size_t  mulle_char7_get_maxlength( void)
{
   if( sizeof( mulle_char7_t) == sizeof( uint32_t))
      return( mulle_char7_maxlength32);
   return( mulle_char7_maxlength64);
}


static inline mulle_char7_t  mulle_char7_substring( mulle_char7_t value,
                                                    unsigned int location,
                                                    unsigned int length)
{
   if( sizeof( mulle_char7_t) == sizeof( uint32_t))
      return( (mulle_char7_t) mulle_char7_substring32( (uint32_t) value, location, length));
   return( (mulle_char7_t) mulle_char7_substring64( value, location, length));
}

#endif

