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

#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

/**
 * Short cut for signed 32 bit integer.
 */
typedef long		   INT32;
/**
 * Short cut for unsigned 32 bit integer.
 */
typedef unsigned long	   UINT32;
/**
 * Short cut for signed 64 bit integer.
 */
typedef signed long long   INT64;
/**
 * Short cut for unsigned 64 bit integer.
 */
typedef unsigned long long UINT64;

/**
 * Add invariant checks to code. Failure results in a fatal internal error.
 * Condition \a c must be true.
 * \ingroup utilities
 */
#define ASSERT(c) if(!(c)) { xassert( __FILE__, __LINE__ ); abort(); }

/**
 * Add invariant checks to code. Failure results in a fatal internal error.
 * Pointer \a p must not be NULL.
 * \ingroup utilities
 */
#define THROW_NIL(p) MASSERT(p!=NULL);

#endif // DEFS_H


