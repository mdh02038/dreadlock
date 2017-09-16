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
 *	   csymbol.hpp
 *		- class definition for symbol CSymbol 
 *
 *
 ******************************************************************************
 */

#ifndef CSYMBOL_HPP
#define CSYMBOL_HPP

#include "defs.h"
#include "cobject.h"
#include <set>


/**
 * Holder for character strings. All tokens are registered
 * as symbols to allow pointers to be compared for string 
 * equality instead of the contents of the strings.
 */
class CSymbol : public CObject
{
private:
	struct SymbolCompare {
	    bool operator()( CSymbol* const& lhs, CSymbol* const& rhs );
	};
	static set<CSymbol*,SymbolCompare> hashTable;
	static CSymbol* 	freeSymbol;		  ///< single element free list
	static CObstack*	obstack;		  ///< Heap for allocation
	static INT32		nextLabelId;		  ///< Next id for
							  ///< generated labels

	const char*	name;		///< symbol character string
	INT32		hash;		///< hashed value of name
public:
	/**
 	 * Find symbol for given string. If not found create a symbol
 	 * for it.
 	 * \param name text string to search for.
 	 * \return symbol for name
 	 */
	static 	CSymbol* Lookup( const char* name );
	/**
 	 * Generated a new symbol with a unique name using the 
 	 * given prefix.
 	 * \param prefix to use when generating symbol.
 	 * \return new symbol.
 	 */
	static  CSymbol* GenSymbol( const char* prefix );
	/**
 	 * Dump symbol table.
 	 */
	static 	void 	DumpTable( void );
	/**
 	 * Set heap to use for storage.
 	 */
	static	void 	SetObstack( CObstack* aObstack );
	/**
 	 * Create a symbol.
 	 * \param aName text for symbold.
 	 */
	CSymbol( const char* aName );
	/**
 	 * Destroy symbol
 	 */
	~CSymbol( void );
	/**
 	 * Get symbol's text.
 	 * \return text string for symbol.
 	 */
	const char* GetName( void ) const;
	/**
 	 * Determine if string is an escaped symbol
 	 * \return non-zero if string is escaped
 	 */
	int Escaped( void ) const;
	/**
	 * Determine if symbol is external reference
	 * return non-zero if symbol is external
         */
	int External() const;
	/**
 	 * Dump hash table stats
 	 */
	static void DumpStats();
/**
 * \cond
 */
private:
	static INT32	Hash( const char* string );
/**
 * \endcond
 */
};

/**
 * Convert a string representing an identifier to an
 * escaped format for printing (if needed). Verilog identifier escaping
 * rules are used.
 * \ingroup utilities
 * \param s text to check and convert if needed.
 * \return converted text. Storage for this result may be from a
 *         shared static buffer and must be either used or copied
 *         immediately.
 */
const char* Identifier( const char* s );

#endif // CSYMBOL_HPP
