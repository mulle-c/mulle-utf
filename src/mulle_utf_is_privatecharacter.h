//
//  mulle_utf_is_privatecharacter.h
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_privatecharacter_h__
#define mulle_utf_is_privatecharacter_h__

#include "mulle_utf_type.h"

int   mulle_utf16_is_privatecharacter( mulle_utf16_t c);
int   mulle_utf32_is_privatecharacter( mulle_utf32_t c);
int   mulle_utf_is_privatecharacterplane( unsigned int plane);

#endif /* mulle_utf_is_privatecharacter_h */
