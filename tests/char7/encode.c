#include "dependencies.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int  main()
{
   int         c;
   int         i;
   int         d;
   uint32_t    code;
   char        buf[ 7];

   /**/
   /**/
   /**/

   code = mulle_char7_encode32( ".u", 2);
   mulle_char7_decode32( code, buf, 2);

   printf( "%llx\n", (long long) code);

   for( i = 0; i < 2; i++)
      putchar( mulle_char7_get( code, i));
   printf( "\n", d);

   /**/
   /**/
   /**/

   memset( buf, 'X', sizeof( buf));

   code = mulle_char7_encode32( "LMPR", 4);
   mulle_char7_decode32( code, buf, 4);

   if( buf[ 5] != 'X')
   {
      printf( "encode decode overflow\n");
      abort();
   }
   if( strncmp( buf, "LMPR", 4))
   {
      printf( "encode decode failed\n");
      abort();
   }

   printf( "%llx\n", (long long) code);

   for( i = 0; i < 4; i++)
      putchar( mulle_char7_get( code, i));

   printf( "\n", d);

   return( 0);
}

