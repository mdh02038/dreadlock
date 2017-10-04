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
 *	   cbustype.hpp
 *		- class definition of bus definition
 *
 ******************************************************************************
 */

#ifndef CBUSTYPE_H
#define CBUSTYPE_H

#include <stdio.h>
#include <list>
#include "defs.h"
#include "cdecl.h"
#include "csymtab.h"
#include "cvc.h"


class CRule;

/*
 * handle variable declarations
 */

/**
 * Declaration object for variables.
 */
class CBusType: public CDecl
{
private:
    CSymtab<CDecl> symtab;
    list<CVc*>     vcs;
    list<CRule*>   rules;
public:
	static Decl_t DeclType() { return eBUS_TYPE; };
	/**
 	 * Create a register declaration.
 	 * \param symbol declaration symbol.
 	 * \param aLoc file coordinates.
 	 * \param dataType variable data type.
 	 * \param undefined non-zero if register is undefined in source.
 	 */
	CBusType( CSymbol* symbol, Coord* aLoc );
	void Add( CVc *vc ) { vcs.push_back( vc ); }
	const list<CVc*>& Vcs () { return vcs; }
	void Add( CRule *rule ) { rules.push_back( rule ); }
	list<CRule*>& Rules() { return rules; }
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
 	 * Dump Bus info to file.
 	 * \param f file descriptor.
 	 */
	virtual void Dump( FILE* f );
protected:
	/*
 	 * Deep Copy.
 	 */
	void Copy( CObstack* heap, CBusType& var );
private:
	/*
 	 * Disable copy constructor.
 	 */
	CBusType( const CBusType& bus );
};

#endif // CBUSTYPE_H
