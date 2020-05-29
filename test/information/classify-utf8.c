#include <mulle-utf/mulle-utf.h>

#include <stdio.h>
#include <stdlib.h>

static int   test( mulle_utf8_t *text, size_t len)
{
   struct mulle_utf_information   info;

   if( mulle_utf8_information( text, len, &info))
   {
      printf( "Offending character at position %ld (%u)\n",
                           (long) ((mulle_utf8_t *) info.invalid - text),
                           *(mulle_utf8_t *) info.invalid);
      return( 1);
   }
   return( 0);
}


int  main()
{
   int   fails;

   fails  = 0;
   fails += test( (mulle_utf8_t []) { 'I', 't', 226, 's' }, 4);
   return( 0);
}

