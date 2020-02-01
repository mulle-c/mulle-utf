//
//  mulle_utf_is_decimaldigit.h
//  mulle-utf
//
//  Created by Nat! on 2016-04-15 17:24:35 +0200.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_decimaldigit_h__
#define mulle_utf_is_decimaldigit_h__

#include "mulle-utf-type.h"

#include <assert.h>


int   mulle_utf16_is_decimaldigit( mulle_utf16_t c);
int   mulle_utf32_is_decimaldigit( mulle_utf32_t c);


static inline int   mulle_utf_is_decimaldigitplane( unsigned int plane)
{
   switch( plane)
   {
   case 0 :
   case 1 :
      return( 1);
   }
   return( 0);
}

#endif
