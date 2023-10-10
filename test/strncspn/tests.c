#include <mulle-utf/mulle-utf.h>

#include <stdio.h>


static void   test( char *s, size_t len, char *p)
{
   size_t  result;

   printf( "mulle_utf8_strncspn( ");
   if( ! s)
      printf( "NULL");
   else
      printf( "\"%.*s\"", (int) len, s);

   printf( ", %d", (int) len);

   if( ! p)
      printf( ", NULL");
   else
      printf( ", \"%s\"", p);

   printf( ") = ");

   result = mulle_utf8_strncspn( (char *) s, len, (char *) p);
   printf( "%ld (%ld)\n", (long) result, (long) ((s && p) ? strcspn( s, p) : 0));
}



int   main( int argc, char **argv)
{
   test( NULL, 0, NULL);
   test( "", 0, NULL);
   test( NULL, 0, "");
   test( "", 0, "");
   test( "", 1, "");

   test( NULL, 2, "");

   test( "abcd", -1, "a");
   test( "abcd", 4, "b");
   test( "abcd", 3, "d");
   test( "abcd", 4, "d");
   test( "abcd", 4, "xyz");

   test( "abcd", -1, "xdy");

   test( "abcd", 4, "ab");
   test( "abcd", 4, "bc");

   test( "abcd", 4, "abc");

   test( "abc", 3, "b");
   test( "abc", 3, "abc");

   return( 0);
}