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
 *	   cmodule.hpp
 *		- class definition for module
 *
 ******************************************************************************
 */

#ifndef CMODULE_H
#define CMODULE_H

#include <stdio.h>
#include "defs.h"
#include "cdecl.h"
#include "csymtab.h"
#include "cinstance.h"
#include "crule.h"
#include "cexception.h"

class CBus;



/*
 * handle variable declarations
 */

/**
 * Declaration object for variables.
 */
class CModule: public CDecl
{
private:
    CSymtab<CDecl>   symtab;
    list<CInstance*> instances;
    list<CRule*>     rules;
    list<CBus*>      ports;
    list<CBus*>      busses;
    list<CException*>    exceptions;
public:
	static Decl_t DeclType() { return eMODULE; };
	/**
 	 * Create a register declaration.
 	 * \param symbol declaration symbol.
 	 * \param aLoc file coordinates.
 	 * \param dataType variable data type.
 	 * \param undefined non-zero if register is undefined in source.
 	 */
	CModule( CSymbol* symbol, Coord* aLoc );
	/** 
	 * Set symbol table
	 * \param symtab symbol table
	 */
	void Symtab( CSymtab<CDecl>& symtab ) { this->symtab = symtab; }
	/** 
	 * Return symbol table
	 * \return  symbol table
	 */
	CSymtab<CDecl>& Symtab() { return symtab; }
	/**
 	 * Create a clone of this declaration.
 	 * \param heap heap to use for allocation.
 	 * \return new declaration.
 	 */
	virtual CDecl* Clone( CObstack* heap );
        /**
	 * Add instance to module
	 */
	void Add( CInstance* instance ) { instances.push_back( instance ); }
        /**
	 * Add exception to module
	 */
	void Add( CException* e ) { exceptions.push_back( e ); }
	/*
	 * add rule
	 */
	void Add( CRule* rule ) { rules.push_back( rule ); }
	/*
	 * add port
	 */
	void AddPort( CBus* port ) { ports.push_back( port ); }
	/*
	 * add bus
	 */
	void AddBus( CBus* bus ) { busses.push_back( bus ); }
	/**
 	 * Dump Bus info to file.
 	 * \param f file descriptor.
 	 */
	virtual void Dump( FILE* f );
protected:
	/*
 	 * Deep Copy.
 	 */
	void Copy( CObstack* heap, CModule& var );
private:
	/*
 	 * Disable copy constructor.
 	 */
	CModule( const CModule& bus );
};
#endif // CMODULE_H
