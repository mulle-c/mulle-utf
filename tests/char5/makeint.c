#include <mulle_utf/mulle_utf.h>

#include <stdio.h>
#include <stdlib.h>




static void   bit32( char *s)
{
   uint32_t   encoded;
   char       decoded[ 6];
   size_t     len;
   size_t     decoded_len;

   len = strlen( s);
   if( ! mulle_char5_is32bit( s, len))
      return;

   encoded     = mulle_char5_encode32_ascii( s, len);
   decoded_len = mulle_char5_decode32_ascii( encoded, decoded, sizeof( decoded));

   if( decoded_len != len || strncmp( decoded, s, decoded_len))
   {
      printf( "%s failed\n", s);
      abort();
   }
   printf( "32bit: %.*s (%d): 0x%x\n", decoded_len, decoded, decoded_len, encoded);
}


static void   bit64( char *s)
{
   uint64_t   encoded;
   char       decoded[ 12];
   size_t     len;
   size_t     decoded_len;

   len = strlen( s);
   if( ! mulle_char5_is64bit( s, len))
      return;

   encoded     = mulle_char5_encode64_ascii( s, len);
   decoded_len = mulle_char5_decode64_ascii( encoded, decoded, sizeof( decoded));
   if( decoded_len != len || strncmp( decoded, s, decoded_len))
   {
      printf( "%s failed\n", s);
      abort();
   }
   printf( "64bit: %.*s (%d): 0x%lx\n", decoded_len, decoded, decoded_len, encoded);
}

static void  test( char *s)
{
   bit32( s);
   bit64( s);
}


int  main()
{
   int  c;
   int  i;
   int  d;

   test( "");
   test( "a");
   test( "ab");
   test( "abc");
   test( "abcd");
   test( "abcde");
   test( "abcdea");
   test( "abcdeab");
   test( "abcdeabc");
   test( "abcdeabcd");
   test( "abcdeabcde");
   test( "abcdeabcdea");
   test( "abcdeabcdeab");
   test( "abcdeabcdeabc");  // 13
}

