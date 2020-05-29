//
//  mulle_char7.c
//  mulle-utf
//
//  Created by Nat! on 24.07.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#include "mulle-char7.h"


int   mulle_char7_is_char7string32( char *src, size_t len)
{
   char   *sentinel;

   if( len > mulle_char7_maxlength32)
      return( 0);

   sentinel = &src[ len];
   while( src < sentinel)
   {
      if( ! *src)
         return( 0);   // invalid char
      if( *src & 0x80)
         return( 0);   // invalid char
      ++src;
   }

   return( 1);
}


int   mulle_char7_is_char7string64( char *src, size_t len)
{
   char   *sentinel;

   if( len > mulle_char7_maxlength64)
      return( 0);

   sentinel = &src[ len];
   while( src < sentinel)
   {
      if( ! *src)
         return( 0);   // invalid char
      if( *src & 0x80)
         return( 0);   // invalid char
      ++src;
   }

   return( 1);
}


uint32_t  mulle_char7_encode32( char *src, size_t len)
{
   char       *s;
   char       *sentinel;
   int        char7;
   uint32_t   value;

   assert( len <= mulle_char7_maxlength32);

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


uint64_t  mulle_char7_encode64( char *src, size_t len)
{
   char       *s;
   char       *sentinel;
   int        char7;
   uint64_t   value;

   assert( len <= mulle_char7_maxlength64);

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


uint32_t  mulle_char7_encode32_utf32( mulle_utf32_t *src, size_t len)
{
   mulle_utf32_t   *s;
   mulle_utf32_t   *sentinel;
   int             char7;
   uint32_t        value;

   assert( len <= mulle_char7_maxlength32);

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


uint64_t  mulle_char7_encode64_utf32( mulle_utf32_t *src, size_t len)
{
   mulle_utf32_t   *s;
   mulle_utf32_t   *sentinel;
   int             char7;
   uint64_t        value;

   assert( len <= mulle_char7_maxlength64);

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


size_t  mulle_char7_decode32( uint32_t value, char *dst, size_t len)
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


size_t  mulle_char7_decode64( uint64_t value, char *dst, size_t len)
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


int  mulle_char7_get64( uint64_t value, unsigned int index)
{
   int   char7;

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


int  mulle_char7_get32( uint32_t value, unsigned int index)
{
   int   char7;

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
