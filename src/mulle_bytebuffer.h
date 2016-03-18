//
//  mulle_bytebuffer.h
//  mulle-utf 
//
//  Copyright (C) 2011 Nat!, Mulle kybernetiK.
//  All rights reserved.
//
//  Coded by Nat!
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#ifndef mulle_bytebuffer_h__
#define mulle_bytebuffer_h__

#include <mulle_allocator/mulle_allocator.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define MULLE_BYTEBUFFER_BASE       \
   uint8_t   *_sentinel;            \
   uint8_t   *_curr;                \
   uint8_t   *_initialstorage;      \
   uint8_t   *_storage;             \
   size_t    _size;                 \
   struct mulle_allocator   *_allocator;


// _size will be -1 for a non-growing buffer
struct mulle_bytebuffer
{
   MULLE_BYTEBUFFER_BASE;
};

//
// this is fairly conveniently just like fwrite(  _storage, len, nElems, fp)
//
struct mulle_flushablebytebuffer;

typedef size_t     _mulle_flushablebytebuffer_flusher( uint8_t *, size_t, size_t, void *);

// _size will be -2 for a flushable buffer (always inflexable)
struct mulle_flushablebytebuffer
{
   MULLE_BYTEBUFFER_BASE;
   
   _mulle_flushablebytebuffer_flusher   *_flusher;
   void                                 *_userinfo;
};


struct mulle_bytebuffer    *_mulle_bytebuffer_alloc( struct mulle_allocator *allocator);
void                        _mulle_bytebuffer_free( struct mulle_bytebuffer *buffer);

void   _mulle_bytebuffer_done( struct mulle_bytebuffer *buffer);
int    _mulle_bytebuffer_grow( struct mulle_bytebuffer *buffer, size_t min_amount);
void   _mulle_bytebuffer_shrink_to_fit( struct mulle_bytebuffer *buffer);
void   _mulle_bytebuffer_make_inflexable_with_static_bytes( struct mulle_bytebuffer *buffer,
                                                            void *_storage,
                                                            size_t length);
size_t _mulle_bytebuffer_set_length( struct mulle_bytebuffer *buffer, size_t length);
void   _mulle_bytebuffer_set_length_and_zero( struct mulle_bytebuffer *buffer, size_t length);

//
// you only do this once!, because you now own the malloc block
//
void   *_mulle_bytebuffer_extract( struct mulle_bytebuffer *buffer);


static inline void   _mulle_bytebuffer_remove_all( struct mulle_bytebuffer *buffer)
{
   buffer->_curr = buffer->_storage;
}


static inline int     _mulle_bytebuffer_is_inflexable( struct mulle_bytebuffer *buffer)
{
   return( buffer->_size == (size_t) -1 || buffer->_size == (size_t) -2);
}


static inline int     _mulle_bytebuffer_is_flushable( struct mulle_bytebuffer *buffer)
{
   return( buffer->_size == (size_t) -1);
}


static inline void    _mulle_flushablebytebuffer_init( struct mulle_flushablebytebuffer *buffer, 
                                                       void *_storage,
                                                       size_t length,
                                                       _mulle_flushablebytebuffer_flusher *flusher,
                                                       void *userinfo)
{
   assert( _storage && length);
   
   buffer->_initialstorage   =
   buffer->_curr      =  
   buffer->_storage   = _storage;
   buffer->_sentinel  = &buffer->_storage[ length];
   buffer->_size      = (size_t) -2;
   
   buffer->_flusher   = flusher;
   buffer->_userinfo  = userinfo;
   buffer->_allocator = NULL;
}


static inline void    _mulle_bytebuffer_init_with_static_bytes( struct mulle_bytebuffer *buffer, void *_storage, size_t length,  struct mulle_allocator *allocator)
{
   assert( length != (size_t) -1);
   
   buffer->_initialstorage  =
   buffer->_curr      =
   buffer->_storage   = _storage;
   buffer->_sentinel  = &((uint8_t *) _storage)[ length];
   buffer->_size      = length;
   buffer->_allocator = allocator;
}


static inline void    _mulle_bytebuffer_init( struct mulle_bytebuffer *buffer, struct mulle_allocator *allocator)
{
   if( ! allocator)
      allocator = &mulle_default_allocator;
   
   memset( buffer, 0, sizeof( struct mulle_bytebuffer));
   buffer->_allocator = allocator;
}


static inline void    _mulle_bytebuffer_set_intital_capacity( struct mulle_bytebuffer *buffer, size_t capacity) 
{
   assert( buffer->_storage  == 0);
   assert( buffer->_size == 0);
   buffer->_size =  capacity >> 1;
}


static inline void    _mulle_bytebuffer_init_with_capacity( struct mulle_bytebuffer *buffer, size_t capacity)
{
   buffer->_initialstorage  =
   buffer->_curr     =  
   buffer->_storage  =
   buffer->_sentinel = NULL;
   buffer->_size     = capacity >> 1;  // will grow to double _size
}


static inline void    _mulle_bytebuffer_inflexable_init_with_static_bytes( struct mulle_bytebuffer *buffer, void *storage, size_t length)
{
   assert( length != (size_t) -1);
   
   buffer->_initialstorage  =
   buffer->_curr      =
   buffer->_storage   = storage;
   buffer->_sentinel  = &((uint8_t *) storage)[ length];
   buffer->_size      = (size_t) -1;
   buffer->_allocator = NULL;
}


static inline void   *_mulle_bytebuffer_get( struct mulle_bytebuffer *buffer)
{
   return( buffer->_storage);
}


static inline size_t   _mulle_bytebuffer_get_length( struct mulle_bytebuffer *buffer)
{
   return( buffer->_curr - buffer->_storage);
}


static inline void   *_mulle_bytebuffer_advance( struct mulle_bytebuffer *buffer, size_t length)
{
   void    *old;
   
   while( &buffer->_curr[ length] > buffer->_sentinel)
      if( _mulle_bytebuffer_grow( buffer, length))
         return( NULL);
   old           = buffer->_curr;
   buffer->_curr = &buffer->_curr[ length];
   return( old);
}


static inline int   _mulle_bytebuffer_is_full( struct mulle_bytebuffer *buffer)
{
   return( buffer->_curr >= buffer->_sentinel);
}


static inline int   _mulle_bytebuffer_is_empty( struct mulle_bytebuffer *buffer)
{
   return( buffer->_curr == buffer->_storage);
}


static inline int   _mulle_bytebuffer_is_void( struct mulle_bytebuffer *buffer)
{
   return( buffer->_storage == buffer->_sentinel);  
}


static inline int   _mulle_bytebuffer_is_overflown( struct mulle_bytebuffer *buffer)
{
   return( buffer->_curr > buffer->_sentinel); // only ever yes for inflexable buffer
}


static inline void   *_mulle_bytebuffer_guarantee( struct mulle_bytebuffer *buffer, size_t length)
{
   while( &buffer->_curr[ length] > buffer->_sentinel)
      if( _mulle_bytebuffer_grow( buffer, length))
         return( NULL);
   return( buffer->_curr);
}


static inline void    _mulle_bytebuffer_add( struct mulle_bytebuffer *buffer, uint8_t c)
{
   if( _mulle_bytebuffer_is_full( buffer))
      if( _mulle_bytebuffer_grow( buffer, 1))
         return;
   *buffer->_curr++ = c;
}


static inline void    _mulle_bytebuffer_add_uint16( struct mulle_bytebuffer *buffer, unsigned short c)
{
   if( ! _mulle_bytebuffer_guarantee( buffer, 2))
      return;
#if __LITTLE_ENDIAN__     // little end first
   *buffer->_curr++ = c & 0xFF;
   *buffer->_curr++ = c >> 8;
#else
   *buffer->_curr++ = c >> 8;
   *buffer->_curr++ = c & 0xFF;
#endif         
}


static inline void   _mulle_bytebuffer_add_bytes( struct mulle_bytebuffer *buffer, void *bytes, size_t length)
{
   void   *s;
   
   if( length)
   {
      s = _mulle_bytebuffer_advance( buffer, length);
      if( s)
         memcpy( s, bytes, length);
   }
}


static inline void   _mulle_bytebuffer_add_zero_terminated_bytes( struct mulle_bytebuffer *buffer, char *bytes)
{
   char   c;
   
   while( c = *bytes++)
      _mulle_bytebuffer_add( buffer, c);
}


static inline size_t   _mulle_bytebuffer_add_zero_terminated_bytes_with_length( struct mulle_bytebuffer *buffer, char *bytes, size_t length)
{
   char   c;
   
   while( length && (c = *bytes++))
   {
      _mulle_bytebuffer_add( buffer, c);
      --length;
   }
   return( length);
}


static inline void   _mulle_bytebuffer_memset( struct mulle_bytebuffer *buffer, char c, size_t length)
{
   void   *s;
   
   if( length)
   {
      s = _mulle_bytebuffer_advance( buffer, length);
      if( s)
         memset( s, c, length);
   }
}


static inline void   _mulle_bytebuffer_zero_last_byte( struct mulle_bytebuffer *buffer)
{
   if( ! _mulle_bytebuffer_is_void( buffer))
   {
      if( _mulle_bytebuffer_is_overflown( buffer))
         buffer->_curr[ -2] = 0;
      else
         if( _mulle_bytebuffer_is_full( buffer))
            buffer->_curr[ -1] = 0;
         else
            buffer->_curr[ 0] = 0;
   }
}


static inline size_t   _mulle_bytebuffer_get_static_bytes( struct mulle_bytebuffer *buffer)
{
   return( buffer->_storage == buffer->_initialstorage ? (buffer->_storage - buffer->_initialstorage) : 0);
}


static inline void    _mulle_bytebuffer_add_bytebuffer( struct mulle_bytebuffer *buffer, struct mulle_bytebuffer *other)
{
   assert( buffer != other);
   
   _mulle_bytebuffer_add_bytes( buffer, _mulle_bytebuffer_get( other), _mulle_bytebuffer_get_length( other));
}


// _initialstorage storage will be lost
static inline void   _mulle_bytebuffer_reset( struct mulle_bytebuffer *buffer)
{
   _mulle_bytebuffer_done( buffer);
   _mulle_bytebuffer_init( buffer, buffer->_allocator);
}

#endif

