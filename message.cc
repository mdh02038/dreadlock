/*****************************************************************************
 * Copyright (C) 1997-2007, Mark Hummel
 * This file is part of Vrq.
 *
 * Vrq is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * Vrq is distributed in the hope that it will be useful,
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

#include "message.h"

static unsigned long errorCount = 0;
static unsigned long warningCount = 0;

/*******************************************************
	info
	- info printing routine
 *******************************************************/

void	info( Coord* loc, const char* format, ... )
{
	va_list	args;

	va_start( args, format );
	if( loc ) {
		logprintf( "INFO \"%s\" line %d: ", loc->filename.c_str(), loc->lineno );
	} else {
		logprintf( "INFO: " );
	}
	vlogprintf( format, args );
	logprintf( "\n" );
	va_end( args );
}

/*******************************************************
	trace
	- trace printing routine
 *******************************************************/

void	trace( Coord* loc, const char* format, ... )
{
	va_list	args;

	return;

	va_start( args, format );
	if( loc ) {
		logprintf( "TRACING \"%s\" line %d: ", loc->filename.c_str(), loc->lineno );
	} else {
		logprintf( "TRACING: " );
	}
	vlogprintf( format, args );
	logprintf( "\n" );
	va_end( args );
}

/*******************************************************
	warning
	- warning printing routine
 *******************************************************/

void	warning( Coord* loc, const char* format, ... )
{
	va_list	args;

	va_start( args, format );
	if( loc ) {
		logprintf( "WARNING \"%s\" line %d: ", loc->filename.c_str(), loc->lineno );
	} else {
		logprintf( "WARNING: " );
	}
	vlogprintf( format, args );
	logprintf( "\n" );
	va_end( args );
	warningCount++;
}

/*******************************************************
	fatal
	- fatal printing routine
 *******************************************************/

void	fatal( Coord* loc, const char* format, ... )
{
	va_list	args;

	va_start( args, format );
	if( loc ) {
		logprintf( "INTERNAL ERROR \"%s\" line %d: ", loc->filename.c_str(), loc->lineno );
	} else {
		logprintf( "INTERNAL ERROR: " );
	}
	vlogprintf( format, args );
	logprintf( "\n" );
	va_end( args );
	abort();
}

/*******************************************************
	error
	- error printing routine
 *******************************************************/

void	error( struct Coord* loc, const char* format, ... )
{
	va_list	args;

	va_start( args, format );
	if( loc ) {
		logprintf( "ERROR \"%s\" line %d: ", loc->filename.c_str(), loc->lineno );
	} else {
		logprintf( "ERROR: " );
	}
	vlogprintf( format, args );
	logprintf( "\n" );
	va_end( args );
	errorCount++;
}

/*******************************************************
	vlogprintf
	- print message to log file 
 *******************************************************/

void	vlogprintf( const char* format, va_list args )
{
	vfprintf( stdout, format, args );
}

/*******************************************************
	logprintf
	- print message to log file 
 *******************************************************/

void	logprintf( const char* format, ... )
{
	va_list	args;

	va_start( args, format );
	vlogprintf( format, args );
	va_end( args );
}
