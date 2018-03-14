//
//  mulle_utf_is_decomposable.h
//  mulle-utf
//
//  Created by Nat! on 2016-04-15 17:24:35 +0200.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_decomposable_h__
#define mulle_utf_is_decomposable_h__

#include "mulle-utf-type.h"


int   mulle_utf16_is_decomposable( mulle_utf16_t c);
int   mulle_utf32_is_decomposable( mulle_utf32_t c);
int   mulle_utf_is_decomposableplane( unsigned int plane);

#endif /* mulle_utf_is_decomposable_h */
