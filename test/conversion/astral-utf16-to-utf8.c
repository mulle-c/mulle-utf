//
// mulle_utf16_convert_to_utf8_string() sizes the result buffer from
// mulle_utf16_information().utf8len. That value is one byte too large for
// every character outside the BMP (see information/utf16-utf8len), so the byte
// before the terminating zero is never written:
//
//    "hi" U+1F600  ->  68 69 F0 9F 98 80 ?? 00
//                                        ^^ never written
//
// The result is a string of the wrong length whose tail is whatever was in the
// allocation. It is usually not even valid UTF8, so it does not survive a
// round trip.
//
// A poisoning allocator is used so the defect is deterministic instead of
// depending on what the heap happened to contain.
//
#include <mulle-utf/mulle-utf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int   fails;


# pragma mark - poisoning allocator

static void   *poison_calloc( size_t n, size_t size, struct mulle_allocator *allocator)
{
   void   *p;

   MULLE_C_UNUSED( allocator);

   p = malloc( n * size);
   if( p)
      memset( p, 0xAA, n * size);
   return( p);
}


//
// mulle_allocator_malloc() routes through realloc, so this is the one that
// has to poison
//
static void   *poison_realloc( void *block, size_t size, struct mulle_allocator *allocator)
{
   void   *p;

   MULLE_C_UNUSED( allocator);

   p = realloc( block, size);
   if( p && ! block)
      memset( p, 0xAA, size);
   return( p);
}


static void   poison_free( void *block, struct mulle_allocator *allocator)
{
   MULLE_C_UNUSED( allocator);

   free( block);
}


static struct mulle_allocator   poison_allocator =
{
   poison_calloc,
   poison_realloc,
   poison_free,
   NULL,
   NULL,
   NULL
};


# pragma mark - tests

static void   check_utf16_to_utf8( char *description,
                                   mulle_utf16_t *src,
                                   size_t len,
                                   char *expected)
{
   char     *utf8;
   size_t   expected_len;
   size_t   i;

   expected_len = strlen( expected);

   utf8 = mulle_utf16_convert_to_utf8_string( src, len, &poison_allocator);
   if( ! utf8)
   {
      printf( "FAIL: %s (conversion returned NULL)\n", description);
      ++fails;
      return;
   }

   if( strlen( utf8) == expected_len && ! memcmp( utf8, expected, expected_len))
      printf( "PASS: %s\n", description);
   else
   {
      printf( "FAIL: %s\n      expected %zu bytes:", description, expected_len);
      for( i = 0; i < expected_len; i++)
         printf( " %02X", (unsigned char) expected[ i]);
      printf( "\n      got      %zu bytes:", strlen( utf8));
      for( i = 0; i < strlen( utf8); i++)
         printf( " %02X", (unsigned char) utf8[ i]);
      printf( "\n");
      ++fails;
   }

   mulle_allocator_free( &poison_allocator, utf8);
}


//
// utf16 -> utf8 -> utf16 must be the identity
//
static void   check_roundtrip( char *description, mulle_utf16_t *src, size_t len)
{
   char            *utf8;
   mulle_utf16_t   *back;

   utf8 = mulle_utf16_convert_to_utf8_string( src, len, &poison_allocator);
   if( ! utf8)
   {
      printf( "FAIL: %s (utf16 -> utf8 returned NULL)\n", description);
      ++fails;
      return;
   }

   back = mulle_utf8_convert_to_utf16_string( utf8, (size_t) -1, &poison_allocator);
   if( ! back)
   {
      printf( "FAIL: %s (utf8 -> utf16 failed, the utf8 was invalid)\n", description);
      ++fails;
      mulle_allocator_free( &poison_allocator, utf8);
      return;
   }

   if( mulle_utf16_strlen( back) == len &&
       ! memcmp( back, src, len * sizeof( mulle_utf16_t)))
      printf( "PASS: %s\n", description);
   else
   {
      printf( "FAIL: %s (round trip turned %zu units into %zu)\n",
              description, len, mulle_utf16_strlen( back));
      ++fails;
   }

   mulle_allocator_free( &poison_allocator, back);
   mulle_allocator_free( &poison_allocator, utf8);
}


int   main( void)
{
   mulle_utf16_t   ascii[] = { 'h', 'i' };
   mulle_utf16_t   latin[] = { 'h', 'i', 0x00E9 };
   mulle_utf16_t   bmp[]   = { 'h', 'i', 0x20AC };
   mulle_utf16_t   grin[]  = { 0xD83D, 0xDE00 };
   mulle_utf16_t   mixed[] = { 'h', 'i', 0xD83D, 0xDE00 };
   mulle_utf16_t   two[]   = { 0xD83D, 0xDE00, 0xD83D, 0xDE01 };

   check_utf16_to_utf8( "ascii",          ascii, 2, "hi");
   check_utf16_to_utf8( "latin1 U+00E9",  latin, 3, "hi\xC3\xA9");
   check_utf16_to_utf8( "bmp U+20AC",     bmp,   3, "hi\xE2\x82\xAC");
   check_utf16_to_utf8( "astral U+1F600", grin,  2, "\xF0\x9F\x98\x80");
   check_utf16_to_utf8( "mixed astral",   mixed, 4, "hi\xF0\x9F\x98\x80");
   check_utf16_to_utf8( "two astral",     two,   4, "\xF0\x9F\x98\x80\xF0\x9F\x98\x81");

   check_roundtrip( "round trip ascii",        ascii, 2);
   check_roundtrip( "round trip bmp",          bmp,   3);
   check_roundtrip( "round trip astral",       grin,  2);
   check_roundtrip( "round trip mixed astral", mixed, 4);
   check_roundtrip( "round trip two astral",   two,   4);

   if( ! fails)
      printf( "\nAll tests passed!\n");
   else
      printf( "\n%d test(s) failed!\n", fails);

   return( fails);
}
