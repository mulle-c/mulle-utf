//
//  mulle_utf_is_whitespace.h
//  mulle-utf
//
//  Created by Nat! on 2016-04-15 17:24:36 +0200.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_whitespace_h__
#define mulle_utf_is_whitespace_h__

#include "mulle_utf_type.h"


int   mulle_utf16_is_whitespace( mulle_utf16_t c);
int   mulle_utf32_is_whitespace( mulle_utf32_t c);
int   mulle_utf_is_whitespaceplane( unsigned int plane);

#endif /* mulle_utf_is_whitespace_h */
