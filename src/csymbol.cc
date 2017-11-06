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
 *	   csymbol.cpp
 *		- method definition for symbol CSymbol 
 *
 *
 ******************************************************************************
 */

#define CSYMBOL_CPP

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "csymbol.h"
#include "lex.h"

/********************************************************
	Initialize global values
*********************************************************/

INT32 		CSymbol::nextLabelId = 0;
CObstack*	CSymbol::obstack = NULL;
set<CSymbol*,CSymbol::SymbolCompare> CSymbol::hashTable;
CSymbol* 	CSymbol::freeSymbol = NULL;

/********************************************************
	Constructor	
*********************************************************/

CSymbol::CSymbol( const char* aName )
{

	name = aName;
	hash = Hash( aName );
}
/********************************************************
	Destructor	
*********************************************************/

CSymbol::~CSymbol( void )
{
}

/********************************************************
	SymbolCompare
	- functor used by set to order symbols	
*********************************************************/

bool CSymbol::SymbolCompare::operator() ( CSymbol* const& lhs, CSymbol* const& rhs )
{
    if( lhs->hash != rhs->hash ) {
	return lhs->hash < rhs->hash;
    }
    return strcmp(lhs->name, rhs->name) < 0;
}
/********************************************************
	GetName	
		return name of symbol	
*********************************************************/

const char* CSymbol::GetName( void ) const
{
	return( name );
}
/********************************************************
	SetObstack	
	- set obstack for csymbol allocation
*********************************************************/

void	CSymbol::SetObstack( CObstack* aObstack )
{
	obstack = aObstack;
}

/********************************************************
	Lookup	
		return symbol node for given name.
		create symbol node if it doesn't exist	
*********************************************************/

CSymbol* CSymbol::Lookup( const char* aName )
{
	if( !freeSymbol ) {
	    freeSymbol = new(obstack) CSymbol(aName);
	} else {
	    freeSymbol->name = aName;
	    freeSymbol->hash = Hash(aName);
	}

	pair<set<CSymbol*,SymbolCompare>::iterator,bool> result;
	result = hashTable.insert( freeSymbol );
	if( !result.second ) {
	    return *result.first;
	}
	
	char* ptr = (char*)obstack->Copy0(aName, strlen(aName) );
	CSymbol* symbol = freeSymbol;
	freeSymbol = NULL;
	symbol->name = ptr;
	return symbol;
	
}
/********************************************************
	GenSymbol	
		create unique symbol node using prefix 
*********************************************************/

CSymbol* CSymbol::GenSymbol( const char* prefix )
{
	char		buffer[32];

	ASSERT( prefix );
	ASSERT( strlen(prefix) < 16 );

	sprintf( buffer, "%s%04ld", prefix, nextLabelId );
	nextLabelId++;
	return Lookup( buffer );
}
/********************************************************
	DumpTable	
		print symbol table
*********************************************************/
	
void 	CSymbol::DumpTable()
{
	INT32		i;
	CSymbol*	symbol;

	logprintf(  "Symbol Table:\n" );
	set<CSymbol*,SymbolCompare>::iterator ptr;
	for( ptr = hashTable.begin(); ptr != hashTable.end(); ++ptr ) {
	    logprintf( "%s\n", (*ptr)->name );
	}
}
/********************************************************
	Hash	
	- create unique hash for string
 ********************************************************/
INT32	CSymbol::Hash( const char* string )
{
	INT32	length;
	INT32	i;
	unsigned	sum;

	length = strlen( string );
	sum = 5381;
		
	for( i = 0; i < length; i++ ) {
		sum = (sum<<5) + sum + (unsigned char) string[i];
	} 

	return( sum );
}
/*******************************************************
	Escaped 
 	- Determine if string is an escaped symbol.
 	  return non-zero if string is escaped
 *******************************************************/
int CSymbol::Escaped( void ) const
{
    return (name[0] == '\\');
}

/*******************************************************
 	External
	- Determine if symbol is external reference
	  return non-zero if symbol is external
 ******************************************************/
int CSymbol::External() const
{
	int escape = 0;
	for( const char* ptr = GetName(); *ptr; ++ptr ) {
	    switch( *ptr ) {
	    case '\\': 
		escape = 1; 
		break;
	    case ' ':
		escape = 0;
		break;
	    case '.':
		if( !escape ) {
		    return true;
		}
	    }
	}
	return false;
}

/*******************************************************
        Identifier
        - convert identifier to printable form
          note this returns a pointer to a shared
          static string.
********************************************************/

const char* Identifier( const char* s )
{
	/*
	 * hack for now, if string contains :: don't escape
	 * it to make namespace reference work
	 */
	if( strstr( s, "::" ) ) {
	    return s;
	}
	if( s[0] == '\\' ) {
	    return s;
	}
	// ??? mdh  - this routine should be completely removed
	/*
	 * currently this code is only used by the flatten plugin
	 * this should be deleted and the plugin fixed as it really
	 * doesn't do the right thing for flatten anyway
	 */

        static char* result = NULL;
        static unsigned   resultLength = 0;
        if( resultLength < strlen(s)+2 ) {
                resultLength = 2*(strlen(s)+2);
                if( result ) {
                        delete[] result;
                }
                result = new char[resultLength];
        }
        int escape = false;
        const char* sptr = s;
        if( *sptr != 0 && !isalpha(*sptr) && *sptr != '_' ) {
                escape = true;
                sptr++;
        }
        for( ; *sptr ; ++sptr ) {
                if( !isalpha(*sptr) && !isdigit(*sptr) &&
                    *sptr != '$' && *sptr != '_' ) {
                        escape = true;
                }
        }
        if( !escape ) {
                return s;
        }

        strcpy( result, "\\" );
        strcat( result, s );
        strcat( result, " " );
        return result;
}

/*********************************************************
 	DumpStats
	- dumps hash table status
 *********************************************************/
void CSymbol::DumpStats()
{
   printf( "Symbol table stats:\n" );
   printf( "  total entrys = %lu\n", hashTable.size() );
}
    
