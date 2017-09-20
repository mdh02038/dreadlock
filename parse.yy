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
CSymtab<CDecl> symbolTable;;
CSymtab<CDecl> topSymbolTable = symbolTable;
CObstack declHeapObject("declarations");
CObstack* declHeap = &declHeapObject;
list<CModule*> modules;
list<CBusType*> busTypes;;

template<typename T> T* CreateVariable( CSymbol* sym, bool unique, T* ) {
    CDecl* decl = symbolTable.LookupTop(sym);	
    if( decl && (!unique || decl->GetType() != T::DeclType() ) ) {
        error( &loc, "Variable %s is already defined at line %lu\n", sym->GetName(), decl->GetCoord()->lineno );
    } else {
        decl = new(declHeap) T( sym, &loc );
        symbolTable.Add( sym, decl );
    }
    return (T*)decl;
}

%}

%union {
    CSymbol*   symbol;
    CModule*   module;
    CVc*       vc;
    CBusType*  bustype;
    CBus*      bus;
    CInstance* instance;
    symbolpair spair;
};

%token <symbol> SYMBOL
%token '{' '}' '.' ';' ',' BUS VC CHECK RUN MODULE ARROW IGNORE FROM

%type <spair> port_spec
%type <vc> vc_name
%type <symbol> vc_name_ref
%type <bustype> bus_type
%type <symbol> bus_type_ref
%type <module> module_name
%type <symbol> module_name_ref
%type <instance> instance_name 
%type <bus> bus_name 


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

bus_definition: BUS bus_type 
	    { symbolTable.PushScope(); }
	'{' bus_statements '}'
	    { 
	      $2->Symtab( symbolTable );
	      busTypes.push_back( $2 );
	      symbolTable.PopScope();
	      printf( "bus %s\n", $2->GetName() ); 
	    }
;

bus_statements: 
	| bus_statements bus_statement 
;

bus_statement: VC vc_name  ';'
    { printf( "vc %s\n", $2->GetName() ) }
    | vc_name_ref ARROW vc_name_ref ';'
;


port_spec:  bus_name
	{ $$.first = $1; $$.second = CSymbol::Lookup("*"); }
        | bus_name '.' '*' 
	{ $$.first = $1; $$.second = CSymbol::Lookup("*"); }
	| bus_name '.' vc_name_ref
	{ $$.first = $1; $$.second = $3; }
	;

check_statement: CHECK module_name_ref ';'
	{ printf( "check %s\n", $2->GetName() ); }
	;

run_statement: RUN module_name_ref ';'
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
	
port: port_name '(' ')'
	| port_name '(' bus_name ')'
	;

module_statement: 
        module_name_ref instance_name '(' port_list_o ')' ';'
	{ printf( "instance_name %s of %s\n", $2->GetName(), $1->GetName() ); }
        | port_spec ARROW port_spec ';'
	{ printf( "rule: %s.%s -> %s.%s\n", $1.first->GetName(), $1.second->GetName(), $3.first->GetName(), $3.second->GetName() ); }
	| IGNORE port_spec FROM module_name_ref
	;

module: MODULE module_name 
	    { symbolTable.PushScope(); }
	'(' port_decl_list_o ')' '{' module_statements '}'
	    { 
	      $2->Symtab( symbolTable );
	      modules.push_back($2);
	      symbolTable.PopScope();
	      printf( "module %s\n", $2->GetName() );
	    }
	;

port_decl_list_o: 
	| port_decl_list
	;

port_decl_list: port_decl
	| port_decl_list ',' port_decl
	;
	
port_decl: bus_type_ref bus_name
	;

vc_name: SYMBOL
    { $$ = CreateVariable( $1, true, (CVc*)0 ) }

vc_name_ref: SYMBOL

bus_name: SYMBOL
    { $$ = CreateVariable( $1, true, (CBus*)0 ) }

bus_type: SYMBOL
    { $$ = CreateVariable( $1, true, (CBusType*)0 ) }

bus_type_ref: SYMBOL

module_name: SYMBOL
    { $$ = CreateVariable( $1, true, (CModule*)0 ) }

module_name_ref: SYMBOL

instance_name: SYMBOL
    { $$ = CreateVariable( $1, false, (CInstance*)0 ) }

port_name: SYMBOL
    ;


%%
void Dump( FILE* f ) {
    fprintf( f, "Top level symbol table\n" );
    fprintf( f, "======================\n" );
    symbolTable.Dump( stderr );

    list<CBusType*>::iterator pbt;
    for( pbt = busTypes.begin(); pbt != busTypes.end(); ++pbt ) {
         (*pbt)->Dump( f );
    }

    list<CModule*>::iterator pm;
    for( pm = modules.begin(); pm != modules.end(); ++pm ) {
         (*pm)->Dump( f );
    }
}
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
	Dump( stderr );
	return 0;
}
void yyerror(const char* s) {
	error( &loc, "Parse error: %s\n", s);
	exit(1);
}

