#include <mulle_standalone_utf/mulle_standalone_utf.h>

#include <stdio.h>
#include <stdlib.h>



int  main()
{
   int  c;
   int  i;
   int  d;

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
}

