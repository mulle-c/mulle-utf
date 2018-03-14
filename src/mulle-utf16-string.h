/*
 *  mulle_utf16-string.h
 *  MulleRegularExpressions
 *
 *  Created by Nat! on 09.11.11.
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  Copyright (c) 2011 Codeon GmbH.
//  All rights reserved.
 *
 */
#ifndef mulle_utf16_string_h__
#define mulle_utf16_string_h__

#include "mulle-utf-type.h"

#include <stddef.h>
#include <assert.h>


static inline size_t   mulle_utf16_strlen( mulle_utf16_t *s)
{
   mulle_utf16_t   *p;

   assert( s);
   /* this produced the nicest looking i386 code :P */
   for( p = s - 1; *++p;);

   return( p - s);
}

size_t  mulle_utf16_strnlen( mulle_utf16_t *src, size_t len);

mulle_utf16_t  *mulle_utf16_strncpy( mulle_utf16_t *dst, mulle_utf16_t *src, size_t len);
mulle_utf16_t  *mulle_utf16_strchr( mulle_utf16_t *s, int c);
int            _mulle_utf16_atoi( mulle_utf16_t **s);
mulle_utf16_t  *mulle_utf16_strstr( mulle_utf16_t *s1, mulle_utf16_t *s2);
int             mulle_utf16_strncmp( mulle_utf16_t *s1, mulle_utf16_t *s2, size_t len);
size_t          mulle_utf16_strspn( mulle_utf16_t *s1, mulle_utf16_t *s2);
size_t          mulle_utf16_strcspn( mulle_utf16_t *s1, mulle_utf16_t *s2);


static inline int   mulle_utf16_strcmp( mulle_utf16_t *s1, mulle_utf16_t *s2)

{
   return( mulle_utf16_strncmp( s1, s2, -1));
}


static inline int   mulle_utf16_atoi( mulle_utf16_t *s)
{
   return( _mulle_utf16_atoi( &s));
}

#endif
