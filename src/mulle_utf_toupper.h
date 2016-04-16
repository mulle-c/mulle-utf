//
//  mulle_utf_toupper.h
//  mulle-utf
//
//  Created by Nat! on 16.04.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_toupper_h__
#define mulle_utf_toupper_h__

#include "mulle_utf.h"


int   mulle_utf16_toupper( mulle_utf16char_t c);
int   mulle_utf32_toupper( mulle_utf32char_t c);

int   mulle_utf16_totitlecase( mulle_utf16char_t c);
int   mulle_utf32_totitlecase( mulle_utf32char_t c);

#endif /* mulle_utf_toupper_h */
