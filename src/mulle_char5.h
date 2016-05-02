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


static inline int   mulle_char5_decode( int c)
{
   assert( c >= 0 && c < 32);
   
   static char  table[] =
   {
        0, '.', '0', '1', '2', 'A', 'C', 'E',
      'I', 'L', 'M', 'P', 'R', 'S', 'T', '_',
      'a', 'b', 'c', 'd', 'e', 'g', 'i', 'l',
      'm', 'n', 'o', 'p', 'r', 's', 't', 'u'
   };
   
   return( table[ c]);
}

int   mulle_char5_is32bit( char *src, size_t len);
int   mulle_char5_is64bit( char *src, size_t len);

uint32_t  mulle_char5_encode32_ascii( char *src, size_t len);
uint64_t  mulle_char5_encode64_ascii( char *src, size_t len);

size_t  mulle_char5_decode32_ascii( uint32_t value, char *dst, size_t len);
size_t  mulle_char5_decode64_ascii( uint64_t value, char *src, size_t len);


static inline size_t  mulle_char5_strlen64( uint64_t value)
{
   size_t  len;
   
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
   size_t  len;
   
   len = 0;
   while( value)
   {
      value >>= 5;
      ++len;
   }
   return( len);
}

#endif
