/*****************************************************************************
 * Copyright (C) 1997-2007, Mark Hummel
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
 *	   cobject.hpp
 *			- base class for bulk allocated objects 
 *
 *
 ******************************************************************************
 */

#ifndef COBJECT_HPP
#define COBJECT_HPP

#include "cobstack.h"

/**
 * Base class for vrq objects. 
 * Gives objects the ability to be allocated with CObstack allocators
 * 
 */
class CObject
{
public:
	/**
 	 * Allocate object on CObstack heap
 	 * \param size Size of object to create.
 	 * \param stack Heap to use for allocation.
 	 * \return pointer to allocated object.
 	 */
	void* operator new( size_t size, CObstack* stack )
	{
		return stack->Alloc( size );
	};	
	/**
 	 * Destroy object.
 	 * Does nothing as obstacks are release all at once.
 	 * \param object Object to destroy.
 	 */
	void  operator delete( void* object )
	{
	};
};

#endif // COBJECT_HPP
