//
//  mulle_utf_is_validurlquery.h
//  mulle-utf
//
//  Created by Nat! on 2016-05-16 16:34:06 +0200.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_is_validurlquery_h__
#define mulle_utf_is_validurlquery_h__

#include "mulle-utf-type.h"


int   mulle_utf16_is_validurlquery( mulle_utf16_t c);
int   mulle_utf32_is_validurlquery( mulle_utf32_t c);
int   mulle_utf_is_validurlqueryplane( unsigned int plane);

#endif /* mulle_utf_is_validurlquery_h */
