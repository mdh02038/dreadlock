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
 *	   cinstance.hpp
 *		- class definition for instance
 *
 ******************************************************************************
 */

#ifndef CINSTANCE_H
#define CINSTANCE_H

#include <stdio.h>
#include <string>
#include "defs.h"
#include "cdecl.h"

class CModule;
class CPortConn;


/*
 * handle variable declarations
 */

/**
 * Declaration object for variables.
 */
class CInstance: public CDecl
{
private:
    CSymbol*  moduleName; // module type name
    list<CPortConn*> portConnections;
public:
	static Decl_t DeclType() { return eINSTANCE; };
	/**
 	 * Create a register declaration.
 	 * \param symbol declaration symbol.
 	 * \param aLoc file coordinates.
 	 * \param dataType variable data type.
 	 * \param undefined non-zero if register is undefined in source.
 	 */
	CInstance( CSymbol* symbol, Coord* aLoc );
	/**
 	 * Create a clone of this declaration.
 	 * \param heap heap to use for allocation.
 	 * \return new declaration.
 	 */
	virtual CDecl* Clone( CObstack* heap );
	/*
	 * set module type
	 */
	void ModuleName( CSymbol* name ) { moduleName = name; }
        /*
	 * add port connections
	 */
        void Add( CPortConn* pc ) { portConnections.push_back( pc ); }
        /*
	 * get port connections
	 */
        const list<CPortConn*>& PortConnections() { return portConnections; }
        /*
	 * set port connections
	 */
        void PortConnections( const list<CPortConn*>& portConnections ) {this->portConnections = portConnections; }
	/*
	 * get module type
	 */
	CSymbol*  ModuleName() { return moduleName; }
        void Validate( CSymtab<CDecl> gsymtab, CSymtab<CDecl> lsymtab );
	/**
 	 * Dump Bus info to file.
 	 * \param f file descriptor.
 	 */
	virtual void Dump( FILE* f );
protected:
	/*
 	 * Deep Copy.
 	 */
	void Copy( CObstack* heap, CInstance& var );
private:
	/*
 	 * Disable copy constructor.
 	 */
	CInstance( const CInstance& bus );
};

#endif // CVC_H
