//
//  mulle_bytebuffer.c
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

#include "mulle_bytebuffer.h"

#if DEBUG
# define _mulle_bytebuffer_min_size   16
#else
# define _mulle_bytebuffer_min_size   64
#endif


void   _mulle_bytebuffer_reset( struct mulle_bytebuffer *buffer);

void   *_mulle_bytebuffer_extract( struct mulle_bytebuffer *buffer)
{
   size_t   size;
   void     *block;

   // check for static
   if( buffer->_storage != buffer->_initialstorage)
   {
      block = buffer->_storage;
   }
   else
   {
      size  = _mulle_bytebuffer_get_length( buffer);
      block = mulle_allocator_malloc( buffer->_allocator, size);
      memcpy( block, buffer->_storage, size);
   }
   
   buffer->_storage  =
   buffer->_curr     = 
   buffer->_sentinel = 
   buffer->_initialstorage  = NULL;

   return( block);
}


void    _mulle_bytebuffer_shrink_to_fit( struct mulle_bytebuffer *buffer)
{
   size_t   length;
   
   if( _mulle_bytebuffer_is_inflexable( buffer))
      return;
      
   if( buffer->_storage == buffer->_initialstorage)
      return;
      
   length  = _mulle_bytebuffer_get_length( buffer);

   buffer->_storage  = mulle_allocator_realloc( buffer->_allocator, buffer->_storage, sizeof( uint8_t) * length);
   buffer->_curr     = &buffer->_storage[ length];
   buffer->_sentinel = &buffer->_storage[ length];
   buffer->_size     = length;
}


int   _mulle_bytebuffer_grow( struct mulle_bytebuffer *buffer, size_t min_amount)
{
   void     *malloc_block;
   size_t   new_size;
   size_t   len;
   
   if( _mulle_bytebuffer_is_inflexable( buffer))
   {  
      struct mulle_flushablebytebuffer  *ibuffer;
      
      if( ! _mulle_bytebuffer_is_flushable( buffer))
      {
         buffer->_curr = buffer->_sentinel + 1;  // set "overflowed"
         return( -1);
      }
         
      ibuffer = (struct mulle_flushablebytebuffer *) buffer;
      if( 1 != (*ibuffer->_flusher)( ibuffer->_storage, ibuffer->_curr - ibuffer->_storage, 1, ibuffer->_userinfo))
      {
         ibuffer->_curr = ibuffer->_sentinel + 1;  // set "overflowed"
         return( -2);
      }
      
      ibuffer->_curr = ibuffer->_storage;
      return( 0);
   }
   
   malloc_block = NULL;
   if( buffer->_storage != buffer->_initialstorage)
      malloc_block = buffer->_storage;
   
   new_size = buffer->_size;
   if( new_size < min_amount)
      new_size = min_amount;
   new_size *= 2;
   if( new_size < _mulle_bytebuffer_min_size)
      new_size = _mulle_bytebuffer_min_size;
      
   len               = buffer->_curr - buffer->_storage;
   buffer->_storage  = mulle_allocator_realloc( buffer->_allocator, malloc_block, sizeof( uint8_t) * new_size);
   buffer->_curr     = &buffer->_storage[ len];
   buffer->_sentinel = &buffer->_storage[ new_size];
   assert( buffer->_sentinel >= buffer->_curr);
   buffer->_size     = new_size;
   
   return( 0);
}


//
// this transforms a buffer
// into a inflexableBuffer 
//
void   _mulle_bytebuffer_make_inflexable_with_static_bytes( struct mulle_bytebuffer *buffer, void *buf, size_t length)
{
   if( buffer->_storage != buffer->_initialstorage)
      mulle_allocator_free( buffer->_allocator, buffer->_storage);
   
   buffer->_storage        =
   buffer->_initialstorage = buf;
   
   buffer->_curr     = 
   buffer->_sentinel = &buffer->_storage[ length];
   buffer->_size     = (size_t) -1;
}


void   _mulle_bytebuffer_done( struct mulle_bytebuffer *buffer)
{
   if( buffer->_storage != buffer->_initialstorage)
      mulle_allocator_free( buffer->_allocator, buffer->_storage);
}


struct mulle_bytebuffer    *_mulle_bytebuffer_alloc( struct mulle_allocator *allocator)
{
   struct mulle_bytebuffer  *buffer;
   
   buffer = mulle_allocator_malloc( allocator, sizeof( struct mulle_bytebuffer));
   _mulle_bytebuffer_init( buffer, allocator);
   return( buffer);
}


void   _mulle_bytebuffer_free( struct mulle_bytebuffer *buffer)
{
   mulle_allocator_free( buffer->_allocator, buffer);
}


size_t   _mulle_bytebuffer_set_length( struct mulle_bytebuffer *buffer, size_t length)
{
   ssize_t   diff;
   
   diff = (ssize_t) length - (ssize_t) _mulle_bytebuffer_get_length( buffer);
   if( diff <= 0)
   {
      buffer->_curr = &buffer->_curr[ diff];
      _mulle_bytebuffer_shrink_to_fit( buffer);
      return( 0);
   }
   _mulle_bytebuffer_advance( buffer, (size_t) diff);
   return( diff);
}


void   _mulle_bytebuffer_set_length_and_zero( struct mulle_bytebuffer *buffer, size_t length)
{
   ssize_t   diff;
   
   diff = _mulle_bytebuffer_set_length( buffer, length);
   memset( buffer->_curr, 0, (size_t) diff);
}

