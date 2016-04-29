//
//  mulle_utf_is_punctuation.h
//  mulle-utf
//
//  Created by Nat! on 2016-04-15 17:24:35 +0200.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_is_punctuation_h__
#define mulle_utf_is_punctuation_h__

#include "mulle_utf.h"


int   mulle_utf16_is_punctuation( mulle_utf16_t c);
int   mulle_utf32_is_punctuation( mulle_utf32_t c);
int   mulle_utf_is_punctuationplane( unsigned int plane);

#endif /* mulle_utf_is_punctuation_h */
