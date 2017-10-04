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
 *	   cportconn.cpp
 *		- class definition of portconn declaration
 *
 ******************************************************************************
 */

#include "cportconn.h"




/****************************************************
	Constructor
*****************************************************/
CPortConn::CPortConn( CSymbol* name, CSymbol* external, Coord* aLoc ) 
    : external(external), CDecl( name, aLoc, ePORTCONN ){
}

/****************************************************
	Clone
	- Create a new declaration that is a deep
	  of this declaration.
*****************************************************/
CDecl* CPortConn::Clone( CObstack* heap )
{
    CPortConn* clone = new(heap) CPortConn( GetSymbol(), external, GetCoord() );
    clone->Copy( heap, *this );
    return clone;
}

/****************************************************
	Copy 
	- perform a deep copy
*****************************************************/
void CPortConn::Copy( CObstack* heap, CPortConn& portconn )
{
    CDecl::Copy( heap, portconn );
}

/****************************************************
	Dump
*****************************************************/
void	CPortConn::Dump( FILE* f )
{
    fprintf( f, "%s ", declName[GetType()] );
    fprintf( f, ": %s(%s), defined in ", GetName(), external->GetName() ); 
    CDecl::Dump( f );
    fprintf( f, "\n" );
}	

