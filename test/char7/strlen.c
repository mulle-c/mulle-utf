#include <mulle-utf/mulle-utf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int  main()
{
   int           i;
   uint32_t      code32;
   uint64_t      code64;
   size_t        strlen1;
   size_t        strlen2;
   static char   text[ ] =  {  '.', '0', '1', '2', 'A', 'C', 'E', 'I',
      'L', 'M', 'P', 'R', 'S', 'T', '_', 'a',
      'b', 'c', 'd', 'e', 'g', 'i', 'l', 'm',
      'n', 'o', 'p', 'r', 's', 't', 'u', 0 };

   for( i = 0; i <= mulle_char7_maxlength32; i++)
   {
      code32    = mulle_char7_encode32( text, i);
      strlen1 = mulle_char7_strlen32( code32);
      strlen2 = mulle_char7_fstrlen32( code32);
      printf( "%.*s (%lx) -> %ld %ld\n",  i,  text, (long) code32, (long) strlen1, (long) strlen2);
   }

   for( i = 0; i <= mulle_char7_maxlength64; i++)
   {
      code64  = mulle_char7_encode64( text, i);
      strlen1 = mulle_char7_strlen64( code64);
      strlen2 = mulle_char7_fstrlen64( code64);
      printf( "%.*s (%llx) -> %ld %ld\n",  i,  text, (long long) code64, (long) strlen1, (long) strlen2);
   }

   return( 0);
}

