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
 *	   cbus.cpp
 *		- class definition of bus declaration
 *
 ******************************************************************************
 */

#include "cbus.h"
#include "cbustype.h"




/****************************************************
	Constructor
*****************************************************/
CBus::CBus( CSymbol* name, Coord* aLoc ) 
    : validated(false), isPort(false), busType(NULL), CDecl( name, aLoc, eBUS ){
}

/****************************************************
	Clone
	- Create a new declaration that is a deep
	  of this declaration.
*****************************************************/
CDecl* CBus::Clone( CObstack* heap )
{
    CBus* clone = new(heap) CBus( GetSymbol(), GetCoord() );
    clone->Copy( heap, *this );
    return clone;
}

/****************************************************
	Copy 
	- perform a deep copy
*****************************************************/
void CBus::Copy( CObstack* heap, CBus& bus )
{
    CDecl::Copy( heap, bus );
}

/****************************************************
	Validate
*****************************************************/
void CBus::Validate( CSymtab<CDecl>& gsymtab )
{
    if( validated ) return;
    validated = true;
    CBusType* bt = CDecl::Resolve<CBusType>( gsymtab, busType );
    if( !bt ) {
	error( GetCoord(), "bus type '%s' is not defined", busType->GetName() );
    }
}

/****************************************************
	Dump
*****************************************************/
void	CBus::Dump( FILE* f )
{
    fprintf( f, "%s ", isPort ? "port" : declName[GetType()] );
    fprintf( f, ": %s is type %s, defined in ", GetName(), busType->GetName() ); 
    CDecl::Dump( f );
    fprintf( f, "\n" );
}	

