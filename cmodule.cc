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
 *	   cmodule.cpp
 *		- class definition for module
 *
 ******************************************************************************
 */

#include "cmodule.h"
#include "cinstance.h"
#include "cbus.h"




/****************************************************
	Constructor
*****************************************************/
CModule::CModule( CSymbol* name, Coord* aLoc ) 
    : CDecl( name, aLoc, eMODULE ), validated( false) {
}

/****************************************************
	Clone
	- Create a new declaration that is a deep
	  of this declaration.
*****************************************************/
CDecl* CModule::Clone( CObstack* heap )
{
    CModule* clone = new(heap) CModule( GetSymbol(), GetCoord() );
    clone->Copy( heap, *this );
    return clone;
}

/****************************************************
	Copy 
	- perform a deep copy
*****************************************************/
void CModule::Copy( CObstack* heap, CModule& module )
{
    CDecl::Copy( heap, module );
    symtab = module.symtab;
}
/****************************************************
	Validate
*****************************************************/
void CModule::Validate( CSymtab<CDecl>& gsymtab )
{
    if( validated ) return;
    validated = true;
    for( list<CBus*>::const_iterator b = busses.begin(); b != busses.end(); ++b ) {
	(*b)->Validate( gsymtab );	
    }
    for( list<CBus*>::const_iterator p = ports.begin(); p != ports.end(); ++p ) {
	(*p)->Validate( gsymtab );	
    }
    for( list<CRule*>::const_iterator r = rules.begin(); r != rules.end(); ++r ) {
	(*r)->Validate( gsymtab, symtab );	
    }
    for( list<CInstance*>::const_iterator i = instances.begin(); i != instances.end(); ++i ) {
	(*i)->Validate( gsymtab, symtab );	
    }
}

/****************************************************
	Dump
*****************************************************/
void	CModule::Dump( FILE* f )
{
    fprintf( f, "%s ", declName[GetType()] );
    fprintf( f, ": %s, defined in ", GetName() ); 
    CDecl::Dump( f );
    fprintf( f, "\n" );
    symtab.Dump( f, 0 );
    list<CBus*>::iterator bp;
    for( bp = ports.begin(); bp != ports.end(); ++bp ) {
	fprintf( f, "\t" ); (*bp)->Dump( f );
    }
    for( bp = busses.begin(); bp != busses.end(); ++bp ) {
	fprintf( f, "\t" ); (*bp)->Dump( f );
    }
    list<CInstance*>::iterator pin;
    for( pin = instances.begin(); pin != instances.end(); ++pin ) {
	fprintf( f, "\t" ); (*pin)->Dump( f );
    }
    list<CRule*>::iterator pr;
    for( pr = rules.begin(); pr != rules.end(); ++pr ) {
	fprintf( f, "\t" ); (*pr)->Dump( f );
    }
}	

