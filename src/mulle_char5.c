//
//  mulle_char5.c
//  mulle-utf
//
//  Created by Nat! on 02.05.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#include "mulle_char5.h"

//
// mulle_char5 is a 5 bit encoding of the characters
// of the following set:
// \0 . 0 1 2 A C E   I L M P R S T _   a b c d e g i l   m n o p r s t u
// which cover a surprisingly large amounts of identifiers
// ~ 90%
//
//   218125	e
//   154048	t
//   126255	a
//   122543	i
//   116611	r
//   114143	n
//   111017	_
//   106029	s
//   104099	o
//   88064	l
//   66048	d
//   64974	c
//   56499	m
//   55261	u
//   54991	p
//   41769	g
//   40949	S
//   40281	C
//   36377	2
//   36191	T
//   33180	I
//   31631	E
//   31248	A
//   30212	1
//   28897	L
//   27508	b
//   27274	P
//   26910	M
//   26685	R
// ---
//   25881	f
//   25631	6
//   25007	D
//
int   mulle_char5_encode( int c)
{
   switch ( c)
   {
   case 0   : return( 0);

   case '.' : return( 1);
   case '0' : return( 2);
   case '1' : return( 3);
   case '2' : return( 4);
   case 'A' : return( 5);
   case 'C' : return( 6);
   case 'E' : return( 7);
   case 'I' : return( 8);

   case 'L' : return( 9);
   case 'M' : return( 10);
   case 'P' : return( 11);
   case 'R' : return( 12);
   case 'S' : return( 13);
   case 'T' : return( 14);
   case '_' : return( 15);
   case 'a' : return( 16);

   case 'b' : return( 17);
   case 'c' : return( 18);
   case 'd' : return( 19);
   case 'e' : return( 20);
   case 'g' : return( 21);
   case 'i' : return( 22);
   case 'l' : return( 23);
   case 'm' : return( 24);

   case 'n' : return( 25);
   case 'o' : return( 26);
   case 'p' : return( 27);
   case 'r' : return( 28);
   case 's' : return( 29);
   case 't' : return( 30);
   case 'u' : return( 31);
   }
   return( -1);
}


int   mulle_char5_is32bit( char *src, size_t len)
{
   char   *sentinel;

   if( len > mulle_char5_max_length32)
      return( 0);
   
   sentinel = &src[ len];
   while( src < sentinel)
      switch( mulle_char5_encode( *src++))
      {
      case 0  : return( 1);   // zero byte, ok fine!
      case -1 : return( 0);   // invalid char
      }

   return( 1);
}


int   mulle_char5_is64bit( char *src, size_t len)
{
   char   *sentinel;
   
   if( len > mulle_char5_max_length64)
      return( 0);
   
   sentinel = &src[ len];
   while( src < sentinel)
      switch( mulle_char5_encode( *src++))
      {
      case 0  : return( 1);
      case -1 : return( 0);
      }
   
   return( 1);
}


uint32_t  mulle_char5_encode32_ascii( char *src, size_t len)
{
   char       *s;
   char       *sentinel;
   char       c;
   int        char5;
   uint32_t   value;
   
   value    = 0;
   sentinel = src;
   s        = &src[ len];
   while( s > sentinel)
   {
      c = *--s;
      if( ! c)
         continue;
      
      char5   = mulle_char5_encode( c);
      assert( char5 > 0 && char5 < 0x20);
      assert( value << 5 >> 5 == value);  // hope the optimizer doesn't fck up
      value <<= 5;
      value  |= char5;
   }
   return( value);
}


uint64_t  mulle_char5_encode64_ascii( char *src, size_t len)
{
   char       *s;
   char       *sentinel;
   char       c;
   int        char5;
   uint64_t   value;
   
   value    = 0;
   sentinel = src;
   s        = &src[ len];
   while( s > sentinel)
   {
      c = *--s;
      if( ! c)
         continue;
      
      char5 = mulle_char5_encode( c);
      assert( char5 > 0 && char5 < 0x20);
      assert( value << 5 >> 5 == value);  // hope the optimizer doesn't fck up
      value <<= 5;
      value  |= char5;
   }
   return( value);
}


size_t  mulle_char5_decode32_ascii( uint32_t value, char *dst, size_t len)
{
   char   *s;
   char   *sentinel;
   int    char5;
   
   s        = dst;
   sentinel = &s[ len];
   while( s < sentinel)
   {
      if( ! value)
         break;
      
      char5  = value & 0x1F;
      *s++ = (char) mulle_char5_decode( char5);

      value >>= 5;
   }
   return( s - dst);
}


size_t  mulle_char5_decode64_ascii( uint64_t value, char *dst, size_t len)
{
   char   *s;
   char   *sentinel;
   int    char5;
   
   s        = dst;
   sentinel = &s[ len];
   while( s < sentinel)
   {
      if( ! value)
         break;

      char5 = value & 0x1F;
      *s++  = (char) mulle_char5_decode( char5);
      
      value >>= 5;
   }
   return( s - dst);
}


char  mulle_char5_at64( uint64_t value, unsigned int index)
{
   char   char5;
   
   do
   {
      char5 = value & 0x1F;
      if( ! value)
         break;
      
      value >>= 5;
   }
   while( index--);
   
   return( (char) mulle_char5_decode( char5));
}


char  mulle_char5_at32( uint32_t value, unsigned int index)
{
   char   char5;
   
   do
   {
      char5 = value & 0x1F;
      if( ! value)
         break;
      
      value >>= 5;
   }
   while( index--);
   
   return( (char) mulle_char5_decode( char5));
}
