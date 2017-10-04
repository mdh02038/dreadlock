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
 *	   model.h
 *		- class definition of model
 *
 ******************************************************************************
 */

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <algorithm>
#include <list>
#include "defs.h"
#include "cdecl.h"
#include "cbustype.h"
#include "cmodule.h"


/**
 * Declaration object for variables.
 */
class CModel {
private:
    list<CBusType*> busses;
    list<CModule*>  modules;
    CSymtab<CDecl>  symtab;
    list<CSymbol*>  runs;
    list<CSymbol*>  checks;
public:
	/*
 	 * constructor.
 	 */
	CModel() {}
	/*
 	 * add bus to model
 	 */
	void Add( CBusType* bus ) { busses.push_back( bus ); }
	/*
 	 * add module to model
 	 */
	void Add( CModule* module ) { modules.push_back( module ); }
	/*
 	 * add symbol table to model
 	 */
	void Add( CSymtab<CDecl> symtab ) { this->symtab = symtab; }
	/*
 	 * add check to model
 	 */
	void Check( CSymbol* name ) { checks.push_back( name ); }
	/*
 	 * add run to model
 	 */
	void Run( CSymbol* name ) { runs.push_back( name ); }
	/*
 	 * validate model
 	 */
	void Validate();
	/*
 	 * dump alloy model to file
 	 */
	void DumpAlloy( FILE* f );
	/*
 	 * dump model to file
 	 */
	void Dump( FILE* f );
};

#endif // MODEL_H

