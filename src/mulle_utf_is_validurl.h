//
//  mulle_utf_is_validurl.h
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_is_validurl_h__
#define mulle_utf_is_validurl_h__

#include "mulle_utf.h"


int   mulle_utf_is_validurlfragment( unichar c);
int   mulle_utf_is_validurlhost( unichar c);
int   mulle_utf_is_validurlpassword( unichar c);
int   mulle_utf_is_validurlpath( unichar c);
int   mulle_utf_is_validurlquery( unichar c);
int   mulle_utf_is_validurluser( unichar c);

#endif /* mulle_utf_is_validurl_h */
