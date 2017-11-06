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
 *	   cvcref.h
 *		- class definition for references to vc's
 *
 *
 ******************************************************************************
 */

#ifndef CVCREF_H
#define CVCREF_H

#include "csymbol.h"
#include "cbus.h"
#include "cbustype.h"
#include "cvc.h"
#include "message.h"


class CVcRef : public CObject
{
private:
	Coord	   loc;		  ///< file coordinates of declaration
	CSymbol*   bus;	  	  ///< symbol for bus (may be *)
	CSymbol*   vc;	  	  ///< symbol for vc
        bool	   validated;
	CSymbol*   wildcard;
public:
	/*
	 * constructor
	 */
	CVcRef( CSymbol* bus, CSymbol* vc, Coord* loc ) :
		wildcard(CSymbol::Lookup("*")), validated(false), loc(*loc), bus(bus), vc(vc) {}
        CSymbol* Bus() { return bus; }
        CSymbol* Vc() { return vc; }
        void Validate( CSymtab<CDecl> gsymtab, CSymtab<CDecl> lsymtab )
	{
	    if( validated ) return;
	    validated = true;
	    if( bus == wildcard ) {
		
	    } else {
	        CBus* b = CDecl::Resolve<CBus>( lsymtab, bus );
	        if( !b ) {
		    error( &loc, "bus '%s' is not declared", bus->GetName() );
		    return;
	 	}
		CBusType* bt = CDecl::Resolve<CBusType>( gsymtab, b->BusType() );
	        if( !bt ) {
		    error( &loc, "bus type '%s' is not declared", b->BusType()->GetName() );
		    return;
	 	}
		lsymtab = bt->Symtab();
	    }
	    CVc* v = CDecl::Resolve<CVc>( lsymtab, vc );
	    if( !v && vc != wildcard ) {
	        error( &loc, "vc '%s' is not declared", vc->GetName() );
	    }
		
	}
	void Dump( FILE* f ) {
	    fprintf( f, "%s.%s", bus->GetName(), vc->GetName() );
	}
};

#endif // CVCREF_H

