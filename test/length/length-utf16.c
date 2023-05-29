#include <mulle-utf/mulle-utf.h>

#include <stdio.h>
#include <stdlib.h>


static mulle_utf16_t  text_with_emoji[] =
{
   0x0000003c, 0x0001f3b2, 0x0000003e, 0
};

static mulle_utf16_t  text_with_dollar[] =
{
   0x0041, 0x20ac, 0x0073, 0x0069, 0x0067, 0x006e, 0x0000
};


static void   test( mulle_utf16_t *s)
{
   printf( "utf8  length: %zu\n", mulle_utf16_utf8length( s, -1));
   printf( "utf16 length: %zu\n", mulle_utf16_strlen( s));
   printf( "utf32 length: %zu\n", mulle_utf16_utf32length( s, -1));
}


int  main()
{
   test( text_with_emoji);
   test( text_with_surrogate);
   return( 0);
}

