/*****************************************************************************
 * Copyright (C) 1997-2007, Mark Hummel
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
 *	   csymtab.hpp
 *		- class definition for symbol tables 
 *
 *
 ******************************************************************************
 */

#ifndef CSYMTAB_HPP
#define CSYMTAB_HPP

#include "defs.h"
#include "cdecl.h"
#include <map>
#include <list>
#include <algorithm>



class CSymbol;
			
/**
 * Aux class used to create symbol table scoping.
 * \cond
 */

template<class T1>
class CSymtabEntry : public map<CSymbol*,T1*>
{
private:
	CSymtabEntry*	previous;
public:

/*********************************************************
	Constructor
**********************************************************/
CSymtabEntry( CSymtabEntry* parent ) 
{
	previous     = parent;
}

/*********************************************************
	GetPrevious
	- return previous level 
**********************************************************/
CSymtabEntry*	GetPrevious() {
	return previous;
}

/*********************************************************
	Lookup
	- find symbol by recursively searching table 
**********************************************************/

T1* Lookup( CSymbol* key, int skip=0 )
{
	T1*			    result;
	typename map<CSymbol*,T1*>::iterator ptr;

	result = NULL;
	ptr = this->find( key );
	if( skip==0 && ptr != this->end() ) {
		return ptr->second;
	}
	if( previous ) {
		result = previous->Lookup( key, skip>0 ? skip-1 : 0 );
	}
	return result;
}	

/*********************************************************
	LookupTop
	- find symbol at top level only 
**********************************************************/

T1* LookupTop( CSymbol* key )
{
	T1*	                    result;
	typename map<CSymbol*,T1*>::iterator ptr;

	result = NULL;
	ptr = this->find( key );
	if( ptr != this->end() ) {
		result = ptr->second;
	} 
	return result;
}	

void	Dump( FILE *f, int recurse, int level = 0 ) 
{
	typename map<CSymbol*,T1*>::iterator ptr;

	for( ptr = this->begin(); ptr != this->end(); ++ptr) {
		fprintf( f, "\t%d::%s => ", level, ptr->first->GetName() );
		ptr->second->DumpDeclInfo( f );
	}
	if( recurse && previous != NULL ) {
		previous->Dump( f, recurse, level+1 );
	}
}
		
};
/**
 * \endcond
 */

/**
 * Class for a type safe hierarchical symbol table. Each
 * level of the table represents a different nested scope.
 * Searches a the leaves of the table seach upward to the
 * table root if needed.
 */
template<class T1>
class CSymtab
{
private:
	CSymtabEntry<T1>*	table;		///< current table level
	list<CSymtab<T1> >	importPath;	///< import path
public:

/**
 * hook for namespaces
 */
static T1* Resolve( CSymbol* sym );

/**
 * Create a symbol table.
 */
CSymtab() 
{
	table        = new CSymtabEntry<T1>( NULL );
}
/**
 * Jump back to parent scope.
 */
void PopScope()
{
	ASSERT( table != NULL );

	table = table->GetPrevious();
}	
/**
 * Create a new scope for table.
 */
void PushScope()
{
	table = new CSymtabEntry<T1>( table );
}

/**
 * Add a symbol and it's assocated object at the current level.
 * Create a new level if needed.
 * \param sym symbol to add to table.
 * \param obj object to associated with symbol.
 */
void	Add( CSymbol* sym, T1* obj )
{
	(*table)[sym] = obj;
}
		
/**
 * Lookup symbol only in current scope.
 * \param sym symbol to search for.
 * \return object associated with symbol. NULL if symbol
 * 		  is not found.
 */
T1*	LookupTop( CSymbol* sym )
{

	return table->LookupTop( sym );
}
		
/**
 * Lookup symbol in all scopes starting at the current scope.
 * \param sym symbol to search for.
 * \param skip number of levels to skip
 * \return object associated with symbol. NULL if symbol
 * 		  is not found.
 */
T1*	Lookup( CSymbol* sym, int skip = 0 )
{
	T1* result = table->Lookup( sym, skip );
	if( !result ) {
	    typename list<CSymtab<T1> >::iterator ptr;
	    for( ptr = importPath.begin(); ptr != importPath.end(); ++ptr ) {
	        result = ptr->Lookup( sym );
	        if( result ) {
	    	    break;
	        }
	    }
	}
	if( !result ) {
	    /*
	     * if symbol can be resolved externally, replicate
	     * entry with new symbol. 
	     */
	    result = Resolve( sym );
	    if( result ) {
	        Add( sym, result );
	    }
	}
	return result;
}

/**
 * Import all symbols given package
 * \param symbol symbol for package name to import
 */
void ImportSearchTable( CSymtab<T1>& table )
{
     importPath.push_back( table );
}

/**
 * Dump all symbols in table to file descriptor.
 * \param f file descriptor.
 * \param recurse zero if only current depth should be dumped.
 */
void	Dump( FILE *f, int recurse=true ) 
{
	table->Dump( f, recurse );
}

};

#endif // CSYMTAB_HPP
