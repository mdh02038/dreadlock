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
 *	   cdecl.h
 *		- abstract class for declaration 
 *
 *
 ******************************************************************************
 */

#ifndef CDECL_H
#define CDECL_H

#include <stdio.h>
#include <vector>
#include <list>
#include "csymbol.h"

class CDecl;

/**
 * Declaration types
 */
enum Decl_t {
	eVC = 0,	///< virtual channel declaration
	eBUS = 1,	///< bus declaration
	eBUS_TYPE = 2,	///< bus definition
	eMODULE = 3,	///< module definition
        eINSTANCE = 4,	///< module instance
        ePORT = 5,	///< port instance
	eNONE		///< unspecified declaration used for wildcard
};

#if defined CDECL_CC

/**
 * Array to convert Decl_t to character string
 */
const char* declName[] = {
        "vc",
  	"bus",
  	"bustype",
	"module",
	"instance",
	"port",
	"none"
};
#else
extern char* declName[];
#endif



/**
 * Base class for describing declaration objects
 */
class CDecl : public CObject
{
private:
	Coord	   loc;		  ///< file coordinates of declaration
	CSymbol*   symbol;	  ///< symbol for declaration
	Decl_t	    type;	  ///< declaration type
public:
	/**
 	 * Create a deep copy of declaration
 	 * \param heap heap to use for allocation.
 	 * \return new declaration.
 	 */
	virtual CDecl* Clone( CObstack* heap ) = 0;

        /**
         * Get declaration type
         * \return declaration type.
         */
        Decl_t GetType( void ) { return type; }

        /**
         * Get declaration type as a string
         * \return declaration type.
         */
        const char* GetTypeName( void ) { return declName[type]; }
               
        /**
         * Set declaration coordinate.
         * \param aLoc set file coordinates for declaration
         */
        void SetCoord( Coord* aLoc ) { loc = *aLoc; }
        
        /**
         * Get file coordinates for declaration.
         * \return file coordinates.
         */
        Coord*	GetCoord( void ) {
        	return &loc;
        }
        
        /**
         * Dump declaration info to file descriptor.
         * \param f file descriptor.
         */
        virtual void Dump( FILE* f ) 
        	{ fprintf( f, "'%s' line %lu", loc.filename.c_str() , loc.lineno ); }

	/**
	* Dump declaration name, type and location to file descriptor.
	* \param f file descriptor.
	*/
	virtual void DumpDeclInfo( FILE* f )
	{
	    fprintf( f, "%s: %s, defined in '%s' line %lu\n", 
		declName[GetType()], GetName(), loc.filename.c_str() , loc.lineno );
	}

        /**
         * Shortcut to get declaration's name
         * \return char pointer to name.
         */
        const char* GetName( void ) { return symbol->GetName(); }
        /**
         * Set declaration's symbol
         * \param aSymbol new symbol for declaration.
         */
        void SetSymbol( CSymbol* aSymbol ) { symbol = aSymbol; }
        /**
         * Get declaration's symbol
         * \return declaration's symbol.
         */
        CSymbol* GetSymbol( void ) { return symbol; }
protected:
        /**
         * Create instance of declaration.
         * This should never be call directly.
         * \param aSymbol declaration for symbol.
         * \param aLoc file coordinates of declaration.
         * \param aType declaration type.
         */
        CDecl( CSymbol* aSymbol, Coord* aLoc, 
	       Decl_t aType )  {
        	loc 	 = *aLoc;
        	symbol   = aSymbol;	
        	type 	 = aType;
        }
	/**
 	 * Perform deep copy of given object to this one
 	 * This should never be call directly, only by subclasses.
 	 * \param heap heap to use for allocation.
 	 * \param o declaration to copy.
 	 */
	void Copy( CObstack* heap, const CDecl& o );
	/**
 	 * Copy constructor to do deep copy.
 	 * This should never be called directly.
 	 * \param o declaration to copy.
 	 */
private:
	/*
 	 * disable copy constructor
 	 */
	CDecl( const CDecl& o );

};

#endif // CDECL_HPP

