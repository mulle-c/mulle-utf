//
//  mulle_char7.c
//  mulle-utf
//
//  Created by Nat! on 24.07.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#include "mulle_char7.h"


int   mulle_char7_is32bit( char *src, size_t len)
{
   char   *sentinel;

   if( len > mulle_char7_max_length32)
      return( 0);

   sentinel = &src[ len];
   while( src < sentinel)
      if( *src++ & 0x80)
         return( 0);   // invalid char

   return( 1);
}


int   mulle_char7_is64bit( char *src, size_t len)
{
   char   *sentinel;

   if( len > mulle_char7_max_length64)
      return( 0);

   sentinel = &src[ len];
   while( src < sentinel)
      if( *src++ & 0x80)
         return( 0);   // invalid char

   return( 1);
}


uint32_t  mulle_char7_encode32_ascii( char *src, size_t len)
{
   char       *s;
   char       *sentinel;
   int        char7;
   uint32_t   value;

   value    = 0;
   sentinel = src;
   s        = &src[ len];
   while( s > sentinel)
   {
      char7 = *--s;
      if( ! char7)
         continue;

      assert( ! (char7 & 0x80));
      value <<= 7;
      value  |= char7;
   }
   return( value);
}


uint64_t  mulle_char7_encode64_ascii( char *src, size_t len)
{
   char       *s;
   char       *sentinel;
   int        char7;
   uint64_t   value;

   value    = 0;
   sentinel = src;
   s        = &src[ len];
   while( s > sentinel)
   {
      char7 = *--s;
      if( ! char7)
         continue;

      assert( ! (char7 & 0x80));
      value <<= 7;
      value  |= char7;
   }
   return( value);
}


size_t  mulle_char7_decode32_ascii( uint32_t value, char *dst, size_t len)
{
   char   *s;
   char   *sentinel;

   s        = dst;
   sentinel = &s[ len];
   while( s < sentinel)
   {
      if( ! value)
         break;

      *s++    = value & 0x7F;
      value >>= 7;
   }
   return( s - dst);
}


size_t  mulle_char7_decode64_ascii( uint64_t value, char *dst, size_t len)
{
   char   *s;
   char   *sentinel;

   s        = dst;
   sentinel = &s[ len];
   while( s < sentinel)
   {
      if( ! value)
         break;

      *s++    = value & 0x7F;
      value >>= 7;
   }
   return( s - dst);
}


char  mulle_char7_at64( uint64_t value, unsigned int index)
{
   char   char7;

   do
   {
      char7 = value & 0x7F;
      if( ! value)
         break;

      value >>= 7;
   }
   while( index--);

   return( char7);
}


char  mulle_char7_at32( uint32_t value, unsigned int index)
{
   char   char7;

   do
   {
      char7 = value & 0x7F;
      if( ! value)
         break;

      value >>= 7;
   }
   while( index--);

   return( char7);
}
