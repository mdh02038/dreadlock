/*****************************************************************************
 * Copyright (C) 1997-2017, Mark Hummel
 * This file is part of Dreadlock.
 *
 * Dreadlock is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * Dreadlock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *****************************************************************************
 */
/******************************************************************************
 *
 *
 *	   cobstack.cpp
 *			- methods for stack-like bulk allocation
 *
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "cobstack.h"

list<CObstack*>* CObstack::obstackList = NULL;	///< list of all obstacks

/*****************************************************************************
	OnExitDumpStats
		- on exit dump memory allocation stats
*****************************************************************************/
void CObstack::_OnExitDumpStats()
{
    printf( "Obstack Statistics\n" );
    printf( "==================\n" );
    if( !obstackList ) {
	return;
    }
    list<CObstack*>::iterator ptr;
    for( ptr = obstackList->begin(); ptr != obstackList->end(); ++ptr ) {
	(*ptr)->DumpStats();
    }
}

void CObstack::OnExitDumpStats()
{
    atexit( _OnExitDumpStats );
}

/*****************************************************************************
	DumpStats
		- dump memory allocation stats
*****************************************************************************/
void CObstack::DumpStats()
{
    printf( "obstack: %s\n", name.c_str() );
    Chunk_t* cptr = currentChunk;
    int	     finalChunkCount = 0;
    int	     finalByteCount = 0;
    while( cptr ) {
	finalChunkCount++;
	finalByteCount += cptr->freeOffset;
	cptr = cptr->next;
    }
    printf( "    final chunk count: %d\n", finalChunkCount );  
    printf( "    final byte count: %d\n", finalByteCount );  
    printf( "    max byte count: %ld\n", maxChunkCount * defaultSize );  
}

/*****************************************************************************
	Constructor
		- initialize obstack with given chunksize
*****************************************************************************/

CObstack::CObstack( const char* name, int defaultChunkSize ) :
	name(name)
{
	Chunk_t	*chunk;

	defaultSize 	= defaultChunkSize;
	chunk = (Chunk_t*) xmalloc( defaultSize ); 
	chunkCount = 1;
	maxChunkCount = 1;

	chunk->next 		= NULL;
	chunk->freeOffset 	= 0;
	chunk->objectOffset = 0;
	chunk->size 		= defaultSize - (sizeof(Chunk_t) - 4);

	alignment 		= 8;		/* Sparcs like 8 */
	currentChunk 	= chunk;			
	if( !obstackList ) {
	    obstackList = new list<CObstack*>();
	}
	obstackList->push_back( this );
}

/*****************************************************************************
	Destructor
		- free memory
*****************************************************************************/

CObstack::~CObstack( void )
{
	Chunk_t*	chunk;

	while( currentChunk ) {
		chunk = currentChunk;
		currentChunk = chunk->next;
		xfree( chunk, defaultSize );
	}
	obstackList->remove( this );
}

/****************************************************************************
	NextFree
		- return next free byte
****************************************************************************/
void*	CObstack::NextFree( void )
{
	Chunk_t *chunk;

	ASSERT( currentChunk != NULL );
	
	chunk = currentChunk;
	ASSERT( chunk->freeOffset <= chunk->size );
	ASSERT( chunk->objectOffset <= chunk->size );
	ASSERT( chunk->freeOffset >= 0 );
	ASSERT( chunk->objectOffset >= 0 );
	return( &chunk->data[chunk->freeOffset] );
}
/***************************************************************************
	GetBase
		- return base of current object
***************************************************************************/
void*	CObstack::GetBase( void )
{
	Chunk_t	*chunk;

	ASSERT( currentChunk != NULL );

	chunk = currentChunk;
	ASSERT( chunk->freeOffset <= chunk->size );
	ASSERT( chunk->objectOffset <= chunk->size );
	ASSERT( chunk->freeOffset >= 0 );
	ASSERT( chunk->objectOffset >= 0 );
	return( &chunk->data[chunk->objectOffset] );
}
/*************************************************************************
	PtrGrow
		- grow obstack by one pointer and copy object ptr into it
*************************************************************************/
void	CObstack::PtrGrow( void *object )
{
	Grow( &object, sizeof(object) );
}
/************************************************************************
	GetObjectSize
		- return size of current object
************************************************************************/
INT32	CObstack::GetObjectSize( void ) 
{
	Chunk_t	*chunk;

	ASSERT( currentChunk != NULL );
	
	chunk = currentChunk;
	ASSERT( chunk->freeOffset <= chunk->size );
	ASSERT( chunk->objectOffset <= chunk->size );
	ASSERT( chunk->freeOffset >= 0 );
	ASSERT( chunk->objectOffset >= 0 );
	return( chunk->freeOffset - chunk->objectOffset );	
}
/***********************************************************************
	Copy
		- copy object into obstack, returns ptr to data in obstack
***********************************************************************/
void*	CObstack::Copy( const void *object, INT32 size )
{
	void	*ptr;

	ptr = Alloc( size );
	memcpy( ptr, object, size );
	return( ptr );
}
/****************************************************************************
	Copy0
		- copy object into obstack with null terminator, returns
		  ptr to data in obstack
****************************************************************************/
void*	CObstack::Copy0( const void *object, INT32 size )
{
	void	*ptr;

	ptr = Alloc( size+1 );
	memcpy( ptr, object, size );
	((char*)ptr)[size] = 0;
	return( ptr );
}
/***************************************************************************
	Finish
		- finalize object by advance object base to next free 
		  spot(aligned)
***************************************************************************/
void*	CObstack::Finish( void )
{
	Chunk_t*	chunk;
	INT32		offset;
	INT32		startOffset;

	ASSERT( currentChunk != NULL );
	
	chunk = currentChunk;
	ASSERT( chunk->freeOffset <= chunk->size );
	ASSERT( chunk->objectOffset <= chunk->size );
	ASSERT( chunk->freeOffset >= 0 );
	ASSERT( chunk->objectOffset >= 0 );

	offset = chunk->freeOffset;
	startOffset = chunk->objectOffset;
	offset = ((offset + alignment -1 )/alignment) * alignment;
	if( offset > chunk->size ) {
		offset = chunk->size;
	}
	chunk->freeOffset = offset;
	chunk->objectOffset = offset;
	return( &chunk->data[startOffset] );
} 
/*************************************************************************
	Alloc
		- allocate space and advance object. return start of object
**************************************************************************/
void*	CObstack::Alloc( INT32 size )
{
	void		*ptr;
	
	ptr = GrowChunk( size );	
	ASSERT( ptr == GetBase() );
	
	return( Finish() );
}
/**************************************************************************
	Grow
		- append data to current object
**************************************************************************/
void	CObstack::Grow( const void *data, INT32 size )
{
	void 	*ptr;
	
	ptr = GrowChunk( size );
	memcpy( ptr, data, size );
}
/**************************************************************************
	Grow
		- append empty space to current object
**************************************************************************/
void	CObstack::Grow( INT32 size )
{
    GrowChunk( size );
}
/**************************************************************************
	Free
		- free all storage after and including object
			if object is NULL, free all
***************************************************************************/
void	CObstack::Free( void *object )
{
	Chunk_t	*chunk;

	ASSERT( currentChunk != NULL );
	
	chunk = currentChunk;
	while( chunk && chunk->next ) {
		ASSERT( chunk->freeOffset >= 0 );
		ASSERT( chunk->objectOffset >= 0 );
		ASSERT( chunk->freeOffset <= chunk->size );
		ASSERT( chunk->objectOffset <= chunk->size );
		if( (char*)object >= &chunk->data[0] && ((char*)object <= (&chunk->data[0] + chunk->size)) ) {
			break;
		}
		currentChunk = chunk->next;
		xfree( chunk, defaultSize  );
		chunkCount--;
		chunk = currentChunk;
	}
	if( object == NULL ) {
		chunk->freeOffset = 0;
		chunk->objectOffset = 0;
	} else {
		ASSERT( (char*)object >= &chunk->data[0] && ((char*)object <= (&chunk->data[0] + chunk->size)) );
		chunk->objectOffset = (char*)object - &chunk->data[0];
		chunk->freeOffset = chunk->objectOffset;
	}
	
} 
/***************************************************************************
	GrowChunk
		- make room for data, allocate new hunk if needed
****************************************************************************/
void*	CObstack::GrowChunk( INT32 size ) 
{
	Chunk_t	*chunk;
	Chunk_t	*newChunk;
	INT32	currentSize;
	INT32	newSize;
	void*	ptr;

	ASSERT( currentChunk != NULL );

	chunk = currentChunk;
	ASSERT( chunk->freeOffset <= chunk->size );
	ASSERT( chunk->objectOffset <= chunk->size );
	ASSERT( chunk->freeOffset >= 0 );
	ASSERT( chunk->objectOffset >= 0 );

	if( chunk->size - chunk->freeOffset < size ) {
		newSize = defaultSize - (sizeof(Chunk_t) + 4);
		if( newSize < size + GetObjectSize() ) {
			newSize = (size + GetObjectSize())*4;
		}
		currentSize = chunk->freeOffset - chunk->objectOffset;
		newChunk = (Chunk_t*) xmalloc( newSize + (sizeof(Chunk_t)-4) );
		chunkCount++;
		if( chunkCount > maxChunkCount ) {
		    maxChunkCount = chunkCount;
		}
		newChunk->next = chunk;
		newChunk->freeOffset = currentSize;
		newChunk->objectOffset = 0;
		newChunk->size = newSize;
		memcpy( &newChunk->data[0], &chunk->data[chunk->objectOffset], currentSize );
		currentChunk = newChunk;
		chunk = newChunk;
	}
	
	ASSERT( chunk->freeOffset+size <= chunk->size );
	ptr = &chunk->data[chunk->freeOffset];
	chunk->freeOffset += size;
	return( ptr );
	
}
/***************************************************************************
	IsOwner
	- return non-zero if this pointer was allocated from this obstack
****************************************************************************/
int     CObstack::IsOwner( void* ptr)
{
    for( Chunk_t *chunk = currentChunk; chunk; chunk = chunk->next ) {
	if( ptr >= &chunk->data[0] && ptr < &chunk->data[chunk->freeOffset] ) {
	    return true;
	} 
    }
    return false;
}
