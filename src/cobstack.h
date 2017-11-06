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
 *	   cobstack.hpp
 *			- class definition for stack like bulk allocation
 *
 *
 ******************************************************************************
 */

#ifndef COBSTACK_HPP
#define COBSTACK_HPP

#include <string>
#include <vector>
#include <list>
using namespace std;
#include "defs.h"
#include "message.h"

#define xfree(p,s) free(p)
#define xmalloc(s) malloc(s)

/**
 * Bulk object allocation object.
 * Objects are allocated one at a time, but freed
 * all at once. This is very efficient for both persistant
 * and temporary storage. 
 */
class CObstack
{
private:
	static  list<CObstack*>* obstackList;	///< list of all obstacks

	/**
 	 * obstack chunk definition
 	 */
	typedef struct 	obstackChunk_tag {
		struct 	obstackChunk_tag *next;	///< pointer to next allocated chunk
		long	size;			///< data size of this chunk
		long	objectOffset;		///< offset of current object
		long	freeOffset;		///< offset of next free byte
		char	data[4];		///< start of data area
	} Chunk_t;

	static const 	int OBSTACK_DEFAULT_BLOCK_SIZE = 
						1024*64; ///< allocation size

	string		name;		///< name of obstack
	Chunk_t*	currentChunk;	///< pointer to first chunk
	long		alignment;	///< number of bytes to align to
	long		defaultSize;	///< default number of data byte to 
					///< align to
	int		chunkCount;	///< number of allocate chunks
	int		maxChunkCount;	///< maximum number of chunks allocated
public:
	/**
 	 * OnExitDumpStats
 	 * Dump memory allocation stats for each obstack at exit
	 */
	static void OnExitDumpStats();
	/**
 	 * Create object stack with given chunk size
 	 * \param name name of obstack (used for printing stats)
 	 * \param chunkSize allocation block size. Optional parameter 
 	 * 	  defaults to OBSTACK_DEFAULT_BLOCK_SIZE
 	 */
	CObstack( const char* name, int chunkSize = OBSTACK_DEFAULT_BLOCK_SIZE );
	/**
 	 * Destroy and free object stack storage.
 	 */
	~CObstack( void );	
	/**
 	 * Allocate block of storage with given size.
	 * \param size number of bytes to allocate.
	 * \return pointer to storage.
	 */
	void*	Alloc( INT32 size );
	/**
 	 * Free all storage including and after object.
 	 * \param object if object is NULL, free all objects on heap.
 	 */
	void	Free( void* object );
	/**
 	 * Get base of current object.
 	 * Object has not been finalized so pointer may 
 	 * change as more space is allocated.
 	 * \return pointer to base. 
 	 */
	void*	GetBase( void );
	/**
 	 * Get next free byte of current object.
 	 * Object has not been finalized so pointer may
 	 * change as more space is allocated.
 	 * \return pointer to base. 
 	 */
	void*	NextFree( void );
	/**
 	 * Finialized the object. 
 	 * After this call the object cannot be grown. The 
 	 * pointer returned will not be relocated after this point.
 	 * \return pointer to base of object.
 	 */
	void*	Finish( void );
	/**
 	 * Allocate storage and copy block of memory to heap.
 	 * \param ptr pointer to block to copy.
 	 * \param size size in bytes of block to copy.
 	 * \return finalized pointer to block on heap.
 	 */
	void*	Copy( const void* ptr, INT32 size );
	/**
 	 * Allocate storage and copy block of memory with a byte of 
 	 * 0 to heap.
 	 * \param ptr pointer to block to copy.
 	 * \param size size in bytes of block to copy.
 	 * \return finalized pointer to block on heap.
 	 */
	void*	Copy0( const void* ptr, INT32 size );
	/**
 	 * Get size of current open object.
 	 * \return size in bytes of current object.
 	 */
	INT32	GetObjectSize( void );
	/**
 	 * Grow current object and copy block of data to heap.
 	 * Object stays open and can be grown further.
 	 * \param ptr pointer to block to copy.
 	 * \param size size in bytes of block to copy.	
 	 */
	void	Grow( const void* ptr, INT32 size );
	/**
 	 * Grow current object 
 	 * Object stays open and can be grown further.
 	 * \param size number of bytes to grow object .	
 	 */
	void	Grow( INT32 size );
	/**
 	 * Grow current object and copy pointer to heap.
 	 * Object stays open and can be grown further.
 	 * \param ptr pointer to copy.
 	 */
	void	PtrGrow( void* ptr );
	/**
 	 * Determine if pointer was allocated from this obstack.
 	 * \param ptr pointer to block.
 	 * \return non-zero if block was allocated from this obstack.
 	 */
	int	IsOwner( void* ptr );
private:
	/**
 	 * \cond
 	 */
	static void _OnExitDumpStats();
	void DumpStats();
	void*	GrowChunk( INT32 );
	/**
 	 * \endcond
 	 */
};

/**
 * Transfer vector of pointers to a linear
 * array allocated on the given obstack.
 * \param heap pointer to obstack to allocate array in.
 * \param v vector of pointer to move.
 * \return pointer to array of pointers.
 */
template<class T>
inline T** Finalize( CObstack* heap, vector<T*>& v )
{
        typename vector<T*>::iterator ptr;

        for( ptr = v.begin(); ptr != v.end(); ++ptr ) {
                heap->Grow( &*ptr, sizeof(T*) );
        }
        heap->PtrGrow(NULL);
        v.erase( v.begin(), v.end() );
        return (T**)heap->Finish();
}


#endif // COBSTACK_HPP
