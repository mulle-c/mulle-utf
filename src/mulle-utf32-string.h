/*
 *  mulle_utf32-string.h
 *  MulleRegularExpressions
 *
 *  Created by Nat! on 09.11.11.
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  Copyright (c) 2011 Codeon GmbH.
//  All rights reserved.
 *
 */
#ifndef mulle_utf32_string_h__
#define mulle_utf32_string_h__

#include "mulle-utf-type.h"

#include <stddef.h>
#include <assert.h>


static inline size_t   mulle_utf32_strlen( mulle_utf32_t *s)
{
   mulle_utf32_t   *p;

   /* this produced the nicest looking i386 code :P */
   for( p = s - 1; *++p;);

   return( p - s);
}

size_t  mulle_utf32_strnlen( mulle_utf32_t *src, size_t len);


mulle_utf32_t  *mulle_utf32_strncpy( mulle_utf32_t *dst, mulle_utf32_t *src, size_t len);
mulle_utf32_t  *mulle_utf32_strchr( mulle_utf32_t *s, int c);
int            _mulle_utf32_atoi( mulle_utf32_t **s);
mulle_utf32_t  *mulle_utf32_strstr( mulle_utf32_t *s1, mulle_utf32_t *s2);
int      mulle_utf32_strncmp( mulle_utf32_t *s1, mulle_utf32_t *s2, size_t len);
size_t   mulle_utf32_strspn( mulle_utf32_t *s1, mulle_utf32_t *s2);
size_t   mulle_utf32_strcspn( mulle_utf32_t *s1, mulle_utf32_t *s2);


static inline int      mulle_utf32_strcmp( mulle_utf32_t *s1, mulle_utf32_t *s2)
{
   return( mulle_utf32_strncmp( s1, s2, -1));
}

static inline int   mulle_utf32_atoi( mulle_utf32_t *s)
{
   return( _mulle_utf32_atoi( &s));
}

#endif
