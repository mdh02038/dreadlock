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

#ifndef CRULE_H
#define CRULE_H

#include "cvcref.h"


class CRule : public CObject
{
private:
	Coord	   loc;		  ///< file coordinates of declaration
	CVcRef*	   lhs;
	CVcRef*	   rhs;
public:
	/*
	 * constructor
	 */
	CRule( CVcRef* lhs, CVcRef* rhs, Coord* loc ) :
		loc(*loc), lhs(lhs), rhs(rhs) {}
        CVcRef* LHS() { return lhs; }
        CVcRef* RHS() { return rhs; }
	void Dump( FILE* f ) {
	    fprintf( f, "rule: " );
	    lhs->Dump( f );
	    fprintf( f, " -> " );
	    rhs->Dump( f );
	    fprintf( f, ": defined in '%s' line %lu\n", loc.filename.c_str() , loc.lineno );
	}
};

#endif // CRULE_H

