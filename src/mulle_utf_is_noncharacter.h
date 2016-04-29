//
//  mulle_utf_is_noncharacter.h
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_is_noncharacter_h__
#define mulle_utf_is_noncharacter_h__

#include "mulle_utf.h"

int   mulle_utf16_is_noncharacter( mulle_utf16_t c);
int   mulle_utf32_is_noncharacter( mulle_utf32_t c);
int   mulle_utf_is_noncharacterplane( unsigned int plane);


#endif /* mulle_utf_is_alphanumeric_h */
