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
 *	   cportconn.hpp
 *		- class definition of portconn declaration
 *
 ******************************************************************************
 */

#ifndef CPORTCONN_H
#define CPORTCONN_H

#include <stdio.h>
#include "defs.h"
#include "cdecl.h"



/*
 * handle variable declarations
 */

/**
 * Declaration object for variables.
 */
class CPortConn: public CDecl
{
private:
    CSymbol* external;
    CSymbol* nc;
public:
	static Decl_t DeclType() { return ePORTCONN; };
	/**
 	 * Create a register declaration.
 	 * \param symbol declaration symbol.
 	 * \param aLoc file coordinates.
 	 * \param dataType variable data type.
 	 * \param undefined non-zero if register is undefined in source.
 	 */
	CPortConn( CSymbol* internal, CSymbol* external, Coord* aLoc );
	/**
 	 * Create a clone of this declaration.
 	 * \param heap heap to use for allocation.
 	 * \return new declaration.
 	 */
	virtual CDecl* Clone( CObstack* heap );
	/*
 	 * return internal symbol
	 */
	CSymbol* Internal() { return GetSymbol(); }
	/*
 	 * return external symbol
	 */
	CSymbol* External() { return external; }
        void Validate( CSymtab<CDecl> gsymtab, CSymtab<CDecl> lsymtab, CSymtab<CDecl> msymtab );
	/**
 	 * Dump Bus info to file.
 	 * \param f file descriptor.
 	 */
	virtual void Dump( FILE* f );
protected:
	/*
 	 * Deep Copy.
 	 */
	void Copy( CObstack* heap, CPortConn& var );
private:
	/*
 	 * Disable copy constructor.
 	 */
	CPortConn( const CPortConn& portconn );
};

#endif // CPORTCONN_H
