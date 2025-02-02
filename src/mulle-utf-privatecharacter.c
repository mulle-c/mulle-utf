//
//  mulle_utf_privatecharacter.c
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#include "mulle-utf-privatecharacter.h"


int   mulle_utf16_is_privatecharacter( uint16_t c)
{
   // quick check for most
   if( c < 0xE000)
      return( 0);

   if( c <= 0xF8FF)
      return( 1);

   return( 0);
}

int   mulle_utf32_is_privatecharacter( int32_t c)
{
   if( c <= 0xFFFF)
      return( mulle_utf16_is_privatecharacter( (uint16_t) c));

   if( c >= 0xF0000 && c <= 0xFFFFD)
      return( 1);
   if( c >= 0x100000 && c <= 0x10FFFD)
      return( 1);

   return( 0);
}


int   mulle_utf_is_privatecharacterplane( size_t plane)
{
   switch( plane)
   {
   case 0x00 :
   case 0x0F :
   case 0x10 :
      return( 1);
   }
   return( 0);
}

