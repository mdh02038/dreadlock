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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "main.h"
#include "cobstack.h"
#include "csymbol.h"

using namespace std;
int parse( const char* );

vector<string> fileList;
CObstack permStack("permStack");	// used for symbols
bool dumpModel = false;
CModel model;
CSymtab<CDecl> symbolTable;;

extern unsigned long errorCount;
extern unsigned long warningCount;

#define DREADLOCK "Dreadlock"
#define VERSION "0.0.1"

char copyright[] = "Copyright (C) 1997-2017 Mark Hummel\n\n"
     "Dreadlock comes with ABSOLUTELY NO WARRANTY; This is free\n"
     "software, and you are welcome to redistribute it under the\n"
     "terms of the GNU General Public License as published by\n"
     "the Free Software Foundation; either version 2 of the License,\n"
     "or (at your option) any later version. "
     "\n\n";

/*************************************************
	PrintUsage 
	- print command line help 
 *************************************************/

void PrintUsage( void )
{

    printf( "\n" );
    printf( "'Dreadlock' is a tool for analysing system deadlock.\n\n" );
    printf( "Usage: dreadlock [options] <file1> [<file2 ...]\n\n" );
    printf( "Options:\n" );
    printf( " --version                  Print version\n" );
    printf( " --help                     This message\n" );
    printf( " --dump                     Dump model\n" );
}

/*************************************************
	ParseArguments 
	- parse command line arguments
 *************************************************/

void	ParseArguments( int argc, const char** argv ) 
{
    int 		i;
    
    for( i = 1; i < argc; i++ ) {
        switch( argv[i][0] ) {
        case '-':
            if( !strcmp( &argv[i][1], "-version" ) ) {
                printf("%s %s, \n\n", DREADLOCK, VERSION );
                printf( "%s",  copyright );
                printf( "\nWritten by Mark Hummel\n" );
                exit(0);
            } else if( !strcmp( &argv[i][1], "-help" ) ) {
                PrintUsage();
                exit(0);
            } else if( !strcmp( &argv[i][1], "-dump" ) ) {
                dumpModel = true;
		break;
            } else {
		PrintUsage();
		exit(1);
	    }
            break;
        default:
            fileList.push_back( argv[i] );
            break;
        }
    }
}

void Dump( FILE* f ) {
    model.Dump( f );
}

struct ParseFile {
    ParseFile() {}
    void operator()( const string &s ) {
	cout << "parsing: " << s << " ..." << endl;
	parse( s.c_str() );
    }
};

int main( int argc, const char** argv ) {
    CSymbol::SetObstack( &permStack );
    model.Add( symbolTable );
    ParseArguments( argc, argv );
    for_each( fileList.begin(), fileList.end(), ParseFile() );
    if( dumpModel ) {
	Dump( stderr );
    }
    fprintf( stderr, " Warnings: %lu, Errors: %lu\n", warningCount, errorCount );
    if( errorCount > 0 ) {
	exit(1);
    }
}

