//
//  mulle_utf_is_validurl.c
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#include "mulle_utf_is_validurl.h"


int   mulle_utf_is_validurlfragment( unichar c)
{
   if( c < 0x0021)
      return( 0);

   if( c > 0x007e)
      return( 0);

   switch( c)
   {
   case 0x0022 :
   case 0x0023 :
   case 0x0025 :
   case 0x003c :
   case 0x003e :
   case 0x005b :
   case 0x005c :
   case 0x005d :
   case 0x005e :
   case 0x0060 :
   case 0x007b :
   case 0x007c :
   case 0x007d :
      return( 0);

   default :
       return( 1);
   }
}


int   mulle_utf_is_validurlhost( unichar c)
{
   if( c < 0x0021)
      return( 0);

   if( c > 0x007e)
      return( 0);

   switch( c)
   {
   case 0x0022 :
   case 0x0023 :
   case 0x0025 :
   case 0x002f :
   case 0x003c :
   case 0x003e :
   case 0x003f :
   case 0x0040 :
   case 0x005c :
   case 0x005e :
   case 0x0060 :
   case 0x007b :
   case 0x007c :
   case 0x007d :
      return( 0);

   default :
       return( 1);
   }
}


int   mulle_utf_is_validurlpassword( unichar c)
{
   if( c < 0x0021)
      return( 0);

   if( c > 0x007e)
      return( 0);

   switch( c)
   {
   case 0x0022 :
   case 0x0023 :
   case 0x0025 :
   case 0x002f :
   case 0x003a :
   case 0x003c :
   case 0x003e :
   case 0x003f :
   case 0x0040 :
   case 0x005b :
   case 0x005c :
   case 0x005d :
   case 0x005e :
   case 0x0060 :
   case 0x007b :
   case 0x007c :
   case 0x007d :
      return( 0);

   default :
       return( 1);
   }
}


int   mulle_utf_is_validurlpath( unichar c)
{
   if( c < 0x0021)
      return( 0);

   if( c > 0x007e)
      return( 0);

   switch( c)
   {
   case 0x0022 :
   case 0x0023 :
   case 0x0025 :
   case 0x003b :
   case 0x003c :
   case 0x003e :
   case 0x003f :
   case 0x005b :
   case 0x005c :
   case 0x005d :
   case 0x005e :
   case 0x0060 :
   case 0x007b :
   case 0x007c :
   case 0x007d :
      return( 0);

   default :
       return( 1);
   }
}


int   mulle_utf_is_validurlquery( unichar c)
{
   if( c < 0x0021)
      return( 0);

   if( c > 0x007e)
      return( 0);

   switch( c)
   {
   case 0x0022 :
   case 0x0023 :
   case 0x0025 :
   case 0x003c :
   case 0x003e :
   case 0x005b :
   case 0x005c :
   case 0x005d :
   case 0x005e :
   case 0x0060 :
   case 0x007b :
   case 0x007c :
   case 0x007d :
      return( 0);

   default :
       return( 1);
   }
}


int   mulle_utf_is_validurluser( unichar c)
{
   if( c < 0x0021)
      return( 0);

   if( c > 0x007e)
      return( 0);

   switch( c)
   {
   case 0x0022 :
   case 0x0023 :
   case 0x0025 :
   case 0x002f :
   case 0x003a :
   case 0x003c :
   case 0x003e :
   case 0x003f :
   case 0x0040 :
   case 0x005b :
   case 0x005c :
   case 0x005d :
   case 0x005e :
   case 0x0060 :
   case 0x007b :
   case 0x007c :
   case 0x007d :
      return( 0);

   default :
       return( 1);
   }
}
