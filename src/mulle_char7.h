//
//  mulle_char7.h
//  mulle-utf
//
//  Created by Nat! on 24.07.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_char7_h__
#define mulle_char7_h__

#include "mulle_utf_type.h"

#include <stddef.h>
#include <assert.h>

//
// char7 is a scheme that places small 7 bit ASCII strings
// into 32 bit or 64 bit words
//
// 32 bit can hold up to 4 chars with 4 bits left over
// 64 bit can hold up to 8 chars with 8 bits left over
//
int   mulle_char7_encode( int c);

enum
{
   mulle_char7_max_length32 = 4,
   mulle_char7_max_length64 = 8
};


int   mulle_char7_is32bit( char *src, size_t len);
int   mulle_char7_is64bit( char *src, size_t len);

uint32_t   mulle_char7_encode32_ascii( char *src, size_t len);
uint64_t   mulle_char7_encode64_ascii( char *src, size_t len);

size_t   mulle_char7_decode32_ascii( uint32_t value, char *dst, size_t len);
size_t   mulle_char7_decode64_ascii( uint64_t value, char *src, size_t len);

size_t  mulle_char7_decode32_utf32( uint32_t value, mulle_utf32_t *dst, size_t len);
size_t  mulle_char7_decode64_utf32( uint64_t value, mulle_utf32_t *dst, size_t len);

char  mulle_char7_at64( uint64_t value, unsigned int index);
char  mulle_char7_at32( uint32_t value, unsigned int index);

char  _mulle_char7_at64( uint64_t value, unsigned int index);
char  _mulle_char7_at32( uint32_t value, unsigned int index);


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
static inline int   mulle_char7_is_uintptr( char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return(  mulle_char7_is32bit( src, len));
   return(  mulle_char7_is64bit( src, len));
}


static inline uintptr_t   mulle_char7_encode_ascii( char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_encode32_ascii( src, len));
   return( mulle_char7_encode64_ascii( src, len));
}


static inline size_t   mulle_char7_decode_ascii( uintptr_t value, char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_decode32_ascii( (uint32_t) value, src, len));
   return( mulle_char7_decode64_ascii( value, src, len));
}


static inline char   mulle_char7_at_uintptr( uintptr_t value, unsigned int index)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_at32( (uint32_t) value, index));
   return( mulle_char7_at64( value, index));
}


static inline size_t  mulle_char7_strlen_uintptr( uintptr_t value)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_strlen32( (uint32_t) value));
   return( mulle_char7_strlen64( value));
}


static inline size_t  mulle_char7_uintptr_max_length( void)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_max_length32);
   return( mulle_char7_max_length64);
}


static inline uintptr_t  mulle_char7_uintptr_substring( uintptr_t value, unsigned int location, unsigned int length)

{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char7_substring32( (uint32_t) value, location, length));
   return( mulle_char7_substring64( value, location, length));
}

#endif

