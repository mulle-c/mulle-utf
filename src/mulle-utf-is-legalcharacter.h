//
//  mulle_utf_is_legalcharacter.h
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_legalcharacter_h__
#define mulle_utf_is_legalcharacter_h__

#include "mulle-utf-type.h"


// all characters defined in the unicode standard

int   mulle_utf16_is_legalcharacter( mulle_utf16_t c);
int   mulle_utf32_is_legalcharacter( mulle_utf32_t c);
int   mulle_utf_is_legalcharacterplane( unsigned int plane);

#endif /* mulle_utf_is_alphanumeric_h */
