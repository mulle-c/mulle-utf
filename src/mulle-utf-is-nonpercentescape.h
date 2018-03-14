//
//  mulle_utf_isnonpercentescape.h
//  mulle-utf
//
//  Created by Nat! on 24.04.16.
//  Copyright © 2016 Mulle kybernetiK.
//  Copyright (c) 2016 Codeon GmbH.
//  All rights reserved.
//

#ifndef mulle_utf_isnonpercentescape_h__
#define mulle_utf_isnonpercentescape_h__

#include "mulle-utf-type.h"


int   mulle_utf16_is_nonpercentescape( mulle_utf16_t c);
int   mulle_utf32_is_nonpercentescape( mulle_utf32_t c);
int   mulle_utf_is_nonpercentescapeplane( unsigned int plane);


#endif /* mulle_utf_isnonpercentescape_h */
