//
//  mulle_utf_ctype.h
//  mulle-utf
//
//  Created by Nat! on 15.04.16.
//  Copyright © 2016 Mulle kybernetiK. All rights reserved.
//

#ifndef mulle_utf_ctype_h__
#define mulle_utf_ctype_h__

#include "mulle_utf.h"

#include "mulle_utf_is_alphanumeric.h"
#include "mulle_utf_is_capitalized.h"
#include "mulle_utf_is_decimaldigit.h"
#include "mulle_utf_is_decomposable.h"
#include "mulle_utf_is_nonbase.h"
#include "mulle_utf_is_noncharacter.h"
#include "mulle_utf_is_letter.h"
#include "mulle_utf_is_lowercase.h"
#include "mulle_utf_is_uppercase.h"
#include "mulle_utf_is_symbol.h"
#include "mulle_utf_is_validurl.h"


int   mulle_utf_is_control( unichar c);
int   mulle_utf_is_newline( unichar c);
int   mulle_utf_is_punctuation( unichar c);
int   mulle_utf_is_whitespace( unichar c);
int   mulle_utf_is_whitespace_or_newline( unichar c);


#endif /* mulle_utf_ctype_h */
