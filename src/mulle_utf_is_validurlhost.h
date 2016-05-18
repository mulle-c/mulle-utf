//
//  mulle_utf_is_validurlhost.h
//  mulle-utf
//
//  Created by Nat! on 2016-05-16 16:34:06 +0200.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_is_validurlhost_h__
#define mulle_utf_is_validurlhost_h__

#include "mulle_utf.h"


int   mulle_utf16_is_validurlhost( mulle_utf16_t c);
int   mulle_utf32_is_validurlhost( mulle_utf32_t c);
int   mulle_utf_is_validurlhostplane( unsigned int plane);

#endif /* mulle_utf_is_validurlhost_h */
