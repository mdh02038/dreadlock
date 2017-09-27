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
 *	   crule.h
 *		- class definition for dependency rules
 *
 *
 ******************************************************************************
 */

#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#include "cvcref.h"


class CException : public CObject
{
private:
	Coord	   loc;		  ///< file coordinates of declaration
	CVcRef*	   port; 
	CSymbol*   moduleName;
public:
	/*
	 * constructor
	 */
	CException( CVcRef* port, CSymbol* moduleName, Coord* loc ) :
		loc(*loc), port(port), moduleName(moduleName) {}
        CVcRef* Port() { return port; }
        CSymbol* ModuleName() { return moduleName; }
	void Dump( FILE* f ) {
	    fprintf( f, "exception: ignore " );
	    port->Dump( f );
	    fprintf( f, " from %s", moduleName->GetName() );
	    fprintf( f, ": defined in '%s' line %lu\n", loc.filename.c_str() , loc.lineno );
	}
};

#endif // CEXCEPTION_H

