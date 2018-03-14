//
//  mulle_utf8_is_validurlscheme.h
//  mulle-utf
//
//  Created by Nat! on 16.05.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_validurlscheme_h__
#define mulle_utf_is_validurlscheme_h__

#include "mulle-utf-type.h"


int   mulle_utf16_is_validurlscheme( mulle_utf16_t c);
int   mulle_utf32_is_validurlscheme( mulle_utf32_t c);
int   mulle_utf_is_validurlschemeplane( unsigned int plane);

#endif /* mulle_utf8_is_validurlscheme_h */
