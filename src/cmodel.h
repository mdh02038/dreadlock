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
#include <set>
#include "defs.h"
#include "cdecl.h"
#include "cbustype.h"
#include "cmodule.h"

struct ConnectionInfo {
    CInstance* instance;
    CSymbol*   name;
    CSymbol*   busType;
};


/**
 * Declaration object for variables.
 */
class CModel {
private:
    list<CBusType*> busses;
    list<CModule*>  modules;
    set<CSymbol*>  ports;
    CSymtab<CDecl>  symtab;
    list<CSymbol*>  runs;
    list<CSymbol*>  checks;
    // post build model structures
    list<CModule*>  topLevelModules;
    list<CInstance*>  instances;
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
 	 * build model
 	 */
	void Build( const string& topModuleName );
	/*
 	 * dump alloy model to file
 	 */
	void DumpAlloy( FILE* f );
	/*
 	 * dump model to file
 	 */
	void Dump( FILE* f );

protected:
        const list<CModule*> CollectTopLevelModules();	
	const list<CInstance*> FlattenAndExtractInstances( const list<CModule*>& moduleList );
};

#endif // MODEL_H

