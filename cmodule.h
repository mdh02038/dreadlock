/*****************************************************************************
 * Copyright (C) 1997-2010, Mark Hummel
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
 *	   cmodule.hpp
 *		- class definition for module
 *
 ******************************************************************************
 */

#ifndef CMODULE_H
#define CMODULE_H

#include <stdio.h>
#include "defs.h"
#include "cdecl.h"



/*
 * handle variable declarations
 */

/**
 * Declaration object for variables.
 */
class CModule: public CDecl
{
private:
public:
	/**
 	 * Create a register declaration.
 	 * \param symbol declaration symbol.
 	 * \param aLoc file coordinates.
 	 * \param dataType variable data type.
 	 * \param undefined non-zero if register is undefined in source.
 	 */
	CModule( CSymbol* symbol, Coord* aLoc );
	/**
 	 * Create a clone of this declaration.
 	 * \param heap heap to use for allocation.
 	 * \return new declaration.
 	 */
	virtual CDecl* Clone( CObstack* heap );
	/**
 	 * Dump Bus info to file.
 	 * \param f file descriptor.
 	 */
	virtual void Dump( FILE* f );
protected:
	/*
 	 * Deep Copy.
 	 */
	void Copy( CObstack* heap, CModule& var );
private:
	/*
 	 * Disable copy constructor.
 	 */
	CModule( const CModule& bus );
};

#endif // CVC_H
