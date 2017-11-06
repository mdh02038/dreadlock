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
 *	   cbustype.cpp
 *		- class definition of bus definition
 *
 ******************************************************************************
 */

#include "cbustype.h"
#include "crule.h"




/****************************************************
	Constructor
*****************************************************/
CBusType::CBusType( CSymbol* name, Coord* aLoc ) 
    : validated(false), CDecl( name, aLoc, eBUS_TYPE ){
}

/****************************************************
	Clone
	- Create a new declaration that is a deep
	  of this declaration.
*****************************************************/
CDecl* CBusType::Clone( CObstack* heap )
{
    CBusType* clone = new(heap) CBusType( GetSymbol(), GetCoord() );
    clone->Copy( heap, *this );
    return clone;
}

/****************************************************
	Copy 
	- perform a deep copy
*****************************************************/
void CBusType::Copy( CObstack* heap, CBusType& busType )
{
    CDecl::Copy( heap, busType );
    symtab = busType.symtab;
}

/****************************************************
	Validate
*****************************************************/
void CBusType::Validate( CSymtab<CDecl> gsymtab )
{
    if( validated ) return;
    validated = true;
    for( list<CRule*>::const_iterator r = rules.begin(); r != rules.end(); ++r ) {
	(*r)->Validate( gsymtab, symtab );	
    }
}

/****************************************************
	Dump
*****************************************************/
void	CBusType::Dump( FILE* f )
{
    fprintf( f, "%s ", declName[GetType()] );
    fprintf( f, ": %s, defined in ", GetName() ); 
    CDecl::Dump( f );
    fprintf( f, "\n" );
    symtab.Dump( f, 0 );
    list<CVc*>::iterator pvc;
    for( pvc = vcs.begin(); pvc != vcs.end(); ++pvc ) {
	fprintf( f, "\t" ); (*pvc)->Dump( f );
    }
    list<CRule*>::iterator pr;
    for( pr = rules.begin(); pr != rules.end(); ++pr ) {
	fprintf( f, "\t" ); (*pr)->Dump( f );
    }
}	

