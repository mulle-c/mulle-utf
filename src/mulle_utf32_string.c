/*
 *  mulle_utf32_string.c
 *  MulleRegularExpressions
 *
 *  Created by Nat! on 09.11.11.
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  Copyright (c) 2011 Codeon GmbH.
//  All rights reserved.
 *
 */

#include "mulle_utf32_string.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


size_t  mulle_utf32_strnlen( mulle_utf32_t *src, size_t len)
{
   mulle_utf32_t   *sentinel;
   mulle_utf32_t   *p;
   
   p        = src;
   sentinel = &p[ len];

   while( p < sentinel)
   {
      if( ! *p)
         break;
      ++p;
   }
   return( p - src);
}


/* memo: "string" oriented routines view the utf32 string as an array 
         of int32_t, where the only special character is the 0, which
         is the terminator. 
         
         None of these routines know about a BOM. You need to
         strip it out yourself.
*/


mulle_utf32_t   *mulle_utf32_strncpy( mulle_utf32_t *dst, mulle_utf32_t *src, size_t len)
{
   mulle_utf32_t   *memo;
   mulle_utf32_t   *sentinel;
   mulle_utf32_t   c;
   
   assert( dst);
   assert( src);
   assert( src >= &dst[len] || &src[len] <= dst);
   
   memo     = dst;
   sentinel = &dst[ len];

   while( dst < sentinel)
   {
      c      = *src++;
      *dst++ = c;
      if( ! c)
         break;
   }
   return( memo);
}


mulle_utf32_t   *mulle_utf32_strchr( mulle_utf32_t *s, mulle_utf32_t c)
{
   mulle_utf32_t   d;

   --s;
   for( ;;)
   {
      d = *++s;
      if( d == c)
         return( s); /* compatible to strchr w/respect to c == 0 */
      if( ! d)
         break;
   }

   return( NULL);
}


/*
 * use Rabin–Karp for expected "usual" strings to match
 * this could "partial" match a surrogate, but is this 
 * my problem ?
 */
#define B    131

mulle_utf32_t  *mulle_utf32_strstr( mulle_utf32_t *s, mulle_utf32_t *pattern)
{
   unsigned int   hash_pattern;
   unsigned int   hash_text;
   unsigned int   Bm;
   unsigned int   i;
   unsigned int   n;
   
   if( pattern[ 0] == 0)
      return( s);
   
   Bm   = 1;
   hash_pattern = hash_text = 0;
   
   /* initial hash and count pattern length */
   for( n = 0; s[ n] && pattern[ n]; n++)
   {
      Bm           *= B;
      hash_pattern  = hash_pattern * B + pattern[ n];
      hash_text     = hash_text * B + s[ n];
   }
   
   if( ! s[ n] && pattern[ n])
      return( NULL);
   
   for( i = n;; i++)
   {
      if( (hash_pattern == hash_text) && ! mulle_utf32_strncmp( &s[ i - n], pattern, n))
         return( &s[ i - n]);
      
      if( ! s[ i])
         return( NULL);
      
      hash_text = hash_text * B - s[ i - n] * Bm + s[ i];
   }
}


int   mulle_utf32_strncmp( mulle_utf32_t *s1, mulle_utf32_t *s2, size_t len)
{
   mulle_utf32_t   *sentinel;
   mulle_utf32_t   c;
   mulle_utf32_t   d;
   
   sentinel = &s1[ len];

   while( s1 < sentinel)
   {
      c = *s1++;
      d = *s2++;

      if( c != d)
         return( c - d);

      if( ! c)
         break;
   }

   return( 0);
}



static int   _compare_mulle_utf32_t( mulle_utf32_t *a, mulle_utf32_t *b)
{
   return( *a - *b);
}

#define compare_mulle_utf32_t   ((int (*)( const void *, const void *)) _compare_mulle_utf32_t)

static size_t   _mulle_utf32_strspn( mulle_utf32_t *s1, mulle_utf32_t *s2, int flag)
{
   mulle_utf32_t   *start;
   mulle_utf32_t   *tmp;
   mulle_utf32_t   c;
   mulle_utf32_t   d;
   size_t          s2_len;
   unsigned int    i;
   mulle_utf32_t   *buf;

   start  = s1;
   s2_len = mulle_utf32_strlen( s2);
   if( ! s2_len)
      return( 0);
      
   if( s2_len == 1)
   {
      d = *s2;
         
      --s1;
      while( (c = *++s1))
         if( c != d)
            break;
      return( s1 - start);
   }

   i   = 0;
   buf = alloca( sizeof( mulle_utf32_t) * s2_len);

   --s2;
   while( d = *++s2)
      buf[ i++] = d;
   
   qsort( buf, i, sizeof( mulle_utf32_t), compare_mulle_utf32_t);
   
   --s1;
   while( tmp = s1, c = *++s1)
   {
      if( ! bsearch( &c, buf, i, sizeof( mulle_utf32_t), compare_mulle_utf32_t) == flag)
         break;
   }
   return( tmp + 1 - start);
}


size_t   mulle_utf32_strspn( mulle_utf32_t *s1, mulle_utf32_t *s2)
{
   return( _mulle_utf32_strspn( s1, s2, 1));
}


size_t   mulle_utf32_strcspn( mulle_utf32_t *s1, mulle_utf32_t *s2)
{
   return( _mulle_utf32_strspn( s1, s2, 0));
}


int   _mulle_utf32_atoi( mulle_utf32_t **s_p)
{
   char            buf[ 64];
   char            *sentinel;
   char            *p;
   mulle_utf32_t   *s;
   mulle_utf32_t         c;

   s        = *s_p;
   p        = buf;
   sentinel = &p[ sizeof( buf) - 1];
   while( p < sentinel)
   {
      c = *s;
      if( ! isdigit( c))
         break;
      *p++ = (char) c;
      s++;
   }
   *p   = 0;
   *s_p = s;
   
   return( (int) strtol( buf, NULL, 10));
}


