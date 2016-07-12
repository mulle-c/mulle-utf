#include <mulle_standalone_utf/mulle_standalone_utf.h>

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
      c = mulle_char5_encode( i);
      if( c < 0)
         continue;

      d = mulle_char5_decode( c);
      if( d != i)
      {
         printf( "%d failed\n", i);
         abort();
      }

      if( d)
         printf( "%c", d);
   }

   printf( "\n", d);

   /**/
   /**/
   /**/

   code = mulle_char5_encode32_ascii( ".u", 2);
   mulle_char5_decode32_ascii( code, buf, 2);

   printf( "%llx\n", (long long) code);

   for( i = 0; i < 2; i++)
      putchar( mulle_char5_at32( code, i));
   printf( "\n", d);

   /**/
   /**/
   /**/

   memset( buf, 'X', sizeof( buf));

   code = mulle_char5_encode32_ascii( "LMPRST", 6);
   mulle_char5_decode32_ascii( code, buf, 6);

   if( buf[ 6] != 'X')
   {
      printf( "encode decode overflow\n");
      abort();
   }
   if( strncmp( buf, "LMPRST", 6))
   {
      printf( "encode decode failed\n");
      abort();
   }

   printf( "%llx\n", (long long) code);

   for( i = 0; i < 6; i++)
      putchar( mulle_char5_at32( code, i));

   printf( "\n", d);
}

