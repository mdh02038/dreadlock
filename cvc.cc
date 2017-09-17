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
 *	   cvc.cpp
 *		- class definition of vc
 *
 ******************************************************************************
 */

#include "cvc.h"




/****************************************************
	Constructor
*****************************************************/
CVc::CVc( CSymbol* name, Coord* aLoc ) 
    : CDecl( name, aLoc, eVC ){
}

/****************************************************
	Clone
	- Create a new declaration that is a deep
	  of this declaration.
*****************************************************/
CDecl* CVc::Clone( CObstack* heap )
{
    CVc* clone = new(heap) CVc( GetSymbol(), GetCoord() );
    clone->Copy( heap, *this );
    return clone;
}

/****************************************************
	Copy 
	- perform a deep copy
*****************************************************/
void CVc::Copy( CObstack* heap, CVc& vc )
{
    CDecl::Copy( heap, vc );
}

/****************************************************
	Dump
*****************************************************/
void	CVc::Dump( FILE* f )
{
    fprintf( f, "%s ", declName[GetType()] );
    fprintf( f, ": %s, defined in ", GetName() ); 
    CDecl::Dump( f );
    fprintf( f, "\n" );
}	

