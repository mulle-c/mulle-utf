#include <mulle_standalone_utf/mulle_standalone_utf.h>

#include <stdio.h>
#include <stdlib.h>


static mulle_utf32_t   random_char( mulle_utf32_t mask)
{
   mulle_utf32_t  c;

   do
   {
      c = rand() % mulle_utf32_max + 1;
      c &= mask;
      if( ! c)
         continue;
   }
   while( mulle_utf16_is_bom_char( c) || mulle_utf32_is_noncharacter( c) || mulle_utf32_is_privatecharacter( c));

   return( c);
}



static void   random_text( mulle_utf32_t *text, size_t n, mulle_utf32_t mask)
{
   mulle_utf32_t   *sentinel;

   sentinel = &text[ n];
   while( text < sentinel)
      *text++ = random_char( mask);
}


struct buffer
{
   union
   {
      mulle_utf32_t   _32[ 8];
      mulle_utf16_t   _16[ 16];
      mulle_utf8_t    _8[ 32];
   } text;
   size_t         n;
};


static void  buffer_add( struct buffer *p, void *bytes, size_t len)
{
   memcpy( &p->text._8[ p->n], bytes, len);
   p->n += len;
}



static void   test( mulle_utf32_t text[ 4])
{
   struct buffer   buffer32;
   struct buffer   buffer16;
   struct buffer   buffer8;

   memset( &buffer32, 0, sizeof( buffer32));
   memset( &buffer8, 0, sizeof( buffer8));

   mulle_utf32_convert_to_utf8_bytebuffer( &buffer8, (void *) buffer_add, text, 4);
   mulle_utf8_convert_to_utf32_bytebuffer( &buffer32, (void *) buffer_add, buffer8.text._8, buffer8.n);

   // buffer.n is bytes!
   if( buffer32.n != 16 || memcmp( text, buffer32.text._32, 4))
   {
      printf( "failed with %ls\n", text);
      abort();
   }

   memset( &buffer32, 0, sizeof( buffer32));
   memset( &buffer16, 0, sizeof( buffer16));

   mulle_utf32_convert_to_utf16_bytebuffer( &buffer16, (void *) buffer_add, text, 4);
   mulle_utf16_convert_to_utf32_bytebuffer( &buffer32, (void *) buffer_add, buffer16.text._16, buffer16.n / sizeof( mulle_utf16_t));

   // buffer.n is bytes!
   if( buffer32.n != 16 || memcmp( text, buffer32.text._32, 4))
   {
      printf( "failed with %ls\n", text);
      abort();
   }

   memset( &buffer32, 0, sizeof( buffer32));
   memset( &buffer16, 0, sizeof( buffer16));
   memset( &buffer8, 0, sizeof( buffer8));

   mulle_utf32_convert_to_utf16_bytebuffer( &buffer16, (void *) buffer_add, text, 4);
   mulle_utf16_convert_to_utf8_bytebuffer( &buffer8, (void *) buffer_add, buffer16.text._16, buffer16.n / sizeof( mulle_utf16_t));

   memset( &buffer16, 0, sizeof( buffer16));
   mulle_utf8_convert_to_utf16_bytebuffer( &buffer16, (void *) buffer_add, buffer8.text._8, buffer8.n);
   mulle_utf16_convert_to_utf32_bytebuffer( &buffer32, (void *) buffer_add, buffer16.text._16, buffer16.n / sizeof( mulle_utf16_t));

   // buffer.n is bytes!
   if( buffer32.n != 16 || memcmp( text, buffer32.text._32, 4))
   {
      printf( "failed with %S\n", text);
      abort();
   }
}


static void   stress_test()
{
   mulle_utf32_t   text[ 4];
   unsigned int    i;
   mulle_utf32_t   mask;

   mask = 0;
   for( i = 0; i < 1000000; i++)
   {
      switch( rand() % 5)
      {
      case 0 : mask = 0x7F; break;
      case 1 : mask = 0xFF; break;
      case 2 : mask = 0x7FFF; break;
      case 3 : mask = 0xFFFF; break;
      case 4 : mask = 0x1FFFFF; break;
      }

      random_text( text, 4, mask);

      test( text);
   }
}



int  main()
{
   mulle_utf32_t   killer[ 4] = { 65279, 47177, 29938, 18497 };

   test( killer);
   stress_test();
}

