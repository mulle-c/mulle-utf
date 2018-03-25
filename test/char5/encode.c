#include <mulle-utf/mulle-utf.h>

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

   for( i = 0; i < 255; i++)
   {
      c = mulle_char5_encode_character( i);
      if( c < 0)
         continue;

      d = mulle_char5_decode_character( c);
      if( d != i)
      {
         printf( "%d failed\n", i);
         abort();
      }

      if( d)
         printf( "%c", d);
   }

   printf( "\n");

   /**/
   /**/
   /**/

   code = mulle_char5_encode32( ".u", 2);
   mulle_char5_decode32( code, buf, 2);

   printf( "%llx\n", (long long) code);

   for( i = 0; i < 2; i++)
      putchar( mulle_char5_get32( code, i));
   printf( "\n");

   /**/
   /**/
   /**/

   memset( buf, 'X', sizeof( buf));

   code = mulle_char5_encode32( "DEINOP", 6);
   mulle_char5_decode32( code, buf, 6);

   if( buf[ 6] != 'X')
   {
      printf( "encode decode overflow\n");
      abort();
   }
   if( strncmp( buf, "DEINOP", 6))
   {
      printf( "encode decode failed\n");
      abort();
   }

   printf( "%llx\n", (long long) code);

   for( i = 0; i < 6; i++)
      putchar( mulle_char5_get( code, i));

   printf( "\n");

   return( 0);
}

