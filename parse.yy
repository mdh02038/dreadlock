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
%{
#include "defs.h"
#include "lex.h"
#include "cobstack.h"
#include "message.h"
#include "csymbol.h"
#include "csymtab.h"
#include "cvc.h"
#include "cbus.h"
#include "cbustype.h"
#include "cmodule.h"
#include "cinstance.h"


extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

Coord loc;
CSymtab<CDecl> symbolTable;
CObstack declHeapObject("declarations");
CObstack* declHeap = &declHeapObject;


%}

%union {
    CSymbol* symbol;
    symbolpair spair;
};

%token <symbol> SYMBOL
%token '{' '}' '.' ';' ',' BUS VC CHECK RUN MODULE ARROW IGNORE FROM

%type<spair> port_spec
%type <symbol> vc_name bus_type bus_name module_name instance_name


%start description

%%
description: 
	| description statement
;

statement: bus_definition
	| check_statement
	| run_statement
	| module
;

bus_definition: BUS bus_type '{' bus_statements '}'
	{ printf( "bus %s\n", $2->GetName() ); }
;

bus_statements: 
	| bus_statements bus_statement 
;

bus_statement: VC vc_name  ';'
    { printf( "vc %s\n", $2->GetName() ) }
    | SYMBOL ARROW SYMBOL ';'
;


port_spec:  bus_name
	{ $$.first = $1; $$.second = CSymbol::Lookup("*"); }
        | bus_name '.' '*' 
	{ $$.first = $1; $$.second = CSymbol::Lookup("*"); }
	| bus_name '.' vc_name
	{ $$.first = $1; $$.second = $3; }
	;

check_statement: CHECK module_name ';'
	{ printf( "check %s\n", $2->GetName() ); }
	;

run_statement: RUN module_name ';'
	{ printf( "run %s\n", $2->GetName() ); }
	;

module_statements:
	| module_statements module_statement
	;

port_list_o: 
	| port_list
	;

port_list: port
	| port_list ',' port
	;
	
port: SYMBOL '(' ')'
	| SYMBOL '(' SYMBOL ')'
	;

module_statement: 
        module_name instance_name '(' port_list_o ')' ';'
	{ printf( "instance_name %s of %s\n", $2->GetName(), $1->GetName() ); }
        | port_spec ARROW port_spec ';'
	{ printf( "rule: %s.%s -> %s.%s\n", $1.first->GetName(), $1.second->GetName(), $3.first->GetName(), $3.second->GetName() ); }
	| IGNORE port_spec FROM module_name
	;

module: MODULE module_name '(' port_decl_list_o ')' '{' module_statements '}'
	{ printf( "module %s\n", $2->GetName() ); }
	;

port_decl_list_o: 
	| port_decl_list
	;

port_decl_list: port_decl
	| port_decl_list ',' port_decl
	;
	
port_decl: SYMBOL SYMBOL
	;

vc_name: SYMBOL
    {
 	CDecl* decl = symbolTable.LookupTop($1);	
	if( decl && decl->GetType() != eVC ) {
	    error( &loc, "Variable %s is already defined\n", $1->GetName() );
	}
	if( !decl ) {
	    decl = new(declHeap) CVc( $1, &loc );
	    symbolTable.Add( $1, decl );
	}
    }
    ;
bus_name: SYMBOL
    {
 	CDecl* decl = symbolTable.LookupTop($1);	
	if( decl && decl->GetType() != eBUS ) {
	    error( &loc, "Variable %s is already defined\n", $1->GetName() );
	}
	if( !decl ) {
	    decl = new(declHeap) CBus( $1, &loc );
	    symbolTable.Add( $1, decl );
	}
    }
    ;
bus_type: SYMBOL
    {
 	CDecl* decl = symbolTable.LookupTop($1);	
	if( decl && decl->GetType() != eBUS_TYPE ) {
	    error( &loc, "Variable %s is already defined\n", $1->GetName() );
	}
	if( !decl ) {
	    decl = new(declHeap) CBusType( $1, &loc );
	    symbolTable.Add( $1, decl );
	}
    }
    ;
module_name: SYMBOL
    {
 	CDecl* decl = symbolTable.LookupTop($1);	
	if( decl && decl->GetType() != eMODULE ) {
	    error( &loc, "Variable %s is already defined\n", $1->GetName() );
	}
	if( !decl ) {
	    decl = new(declHeap) CModule( $1, &loc );
	    symbolTable.Add( $1, decl );
	}
    }
    ;
instance_name: SYMBOL
    {
 	CDecl* decl = symbolTable.LookupTop($1);	
	if( decl && decl->GetType() != eINSTANCE ) {
	    error( &loc, "Variable %s is already defined\n", $1->GetName() );
	}
	if( !decl ) {
	    decl = new(declHeap) CInstance( $1, &loc );
	    symbolTable.Add( $1, decl );
	}
    }
    ;


%%
int parse( const char* filename ) {
	yyin = fopen( filename, "r" );
	if( !yyin ) {
	    fprintf( stderr, "Cannot open file: %s\n", filename );
	    return 0;
	}
	loc.lineno = 1;
	loc.filename = filename;
	do { 
		yyparse();
	} while(!feof(yyin));
        fclose( yyin );
	yyin = NULL;
	symbolTable.Dump( stderr );
	return 0;
}
void yyerror(const char* s) {
	error( &loc, "Parse error: %s\n", s);
	exit(1);
}

