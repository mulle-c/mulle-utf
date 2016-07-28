//
//  mulle_char5.h
//  mulle-utf
//
//  Created by Nat! on 02.05.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_char5_h__
#define mulle_char5_h__

#include "mulle_utf_type.h"

#include <stddef.h>
#include <assert.h>

//
// char5 is a compression scheme that places small strings
// into 32 bit or 64 bit words
// not all small strings are compatible!
//
// 32 bit can hold up to 6 chars  with 2 bits left over
// 64 bit can hold up to 12 chars with 4 bits left over
//
int   mulle_char5_encode( int c);

enum
{
   mulle_char5_max_length32 = 6,
   mulle_char5_max_length64 = 12
};


static inline int   mulle_char5_decode( int c)
{
   assert( c >= 0 && c < 32);
   
   static char  table[] =
   {
      0,
      '.', '0', '1', '2', 'A', 'C', 'E', 'I',
      'L', 'M', 'P', 'R', 'S', 'T', '_', 'a',
      'b', 'c', 'd', 'e', 'g', 'i', 'l', 'm',
      'n', 'o', 'p', 'r', 's', 't', 'u'
   };
   
   return( table[ c]);
}

int   mulle_char5_is32bit( char *src, size_t len);
int   mulle_char5_is64bit( char *src, size_t len);

uint32_t   mulle_char5_encode32_ascii( char *src, size_t len);
uint64_t   mulle_char5_encode64_ascii( char *src, size_t len);

size_t   mulle_char5_decode32_ascii( uint32_t value, char *dst, size_t len);
size_t   mulle_char5_decode64_ascii( uint64_t value, char *src, size_t len);

char  mulle_char5_at64( uint64_t value, unsigned int index);
char  mulle_char5_at32( uint32_t value, unsigned int index);

char  _mulle_char5_at64( uint64_t value, unsigned int index);
char  _mulle_char5_at32( uint32_t value, unsigned int index);


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

// the naming... the naming...
// i will rename all this eventually
//
static inline int   mulle_char5_is_uintptr( char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return(  mulle_char5_is32bit( src, len));
   return(  mulle_char5_is64bit( src, len));
}


static inline uintptr_t   mulle_char5_encode_ascii( char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char5_encode32_ascii( src, len));
   return( mulle_char5_encode64_ascii( src, len));
}


static inline size_t   mulle_char5_decode_ascii( uintptr_t value, char *src, size_t len)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char5_decode32_ascii( (uint32_t) value, src, len));
   return( mulle_char5_decode64_ascii( value, src, len));
}


static inline char   mulle_char5_at_uintptr( uintptr_t value, unsigned int index)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char5_at32( (uint32_t) value, index));
   return( mulle_char5_at64( value, index));
}


static inline size_t  mulle_char5_strlen_uintptr( uintptr_t value)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char5_strlen32( (uint32_t) value));
   return( mulle_char5_strlen64( value));
}


static inline size_t  mulle_char5_uintptr_max_length( void)
{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char5_max_length32);
   return( mulle_char5_max_length64);
}


static inline uintptr_t  mulle_char5_uintptr_substring( uintptr_t value, unsigned int location, unsigned int length)

{
   if( sizeof( uintptr_t) == sizeof( uint32_t))
      return( mulle_char5_substring32( (uint32_t) value, location, length));
   return( mulle_char5_substring64( value, location, length));
}

#endif
