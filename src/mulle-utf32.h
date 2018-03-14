//
//  mulle_utf32.h
//  mulle-utf
//
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  Copyright (c) 2011 Codeon GmbH.
//  All rights reserved.
//
//  Coded by Nat!
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#ifndef mulle_utf32_h__
#define mulle_utf32_h__

#include "mulle-utf-type.h"

#include <stddef.h>


size_t   mulle_utf32_utf8length( mulle_utf32_t *src,
                                     size_t len);

int   mulle_utf32_information( mulle_utf32_t *src,
                               size_t len,
                               struct mulle_utf_information *info);

//
// these two are just here for completeness
//
static inline mulle_utf32_t   _mulle_utf32_next_utf32character( mulle_utf32_t **s_p)
{
   return( *(*s_p)++);
}


static inline mulle_utf32_t   _mulle_utf32_previous_utf32character( mulle_utf32_t **s_p)
{
   return( *--(*s_p));
}



void  mulle_utf32_bufferconvert_to_utf16_as_surrogatepair(
                       void *buffer,
                       void (*addbytes)( void *buffer, void *bytes, size_t size),
                       mulle_utf32_t x);

// these routines do not skip BOM characters
int  mulle_utf32_bufferconvert_to_utf8( mulle_utf32_t *src,
                                        size_t len,
                                        void *buffer,
                                        void (*addbytes)( void *buffer, void *bytes, size_t size));

int  mulle_utf32_bufferconvert_to_utf16( mulle_utf32_t *src,
                                         size_t len,
                                         void *buffer,
                                         void (*addbytes)( void *buffer, void *bytes, size_t size));
#endif

