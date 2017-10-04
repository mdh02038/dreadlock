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
#include "main.h"
#include "defs.h"
#include "lex.h"
#include "cobstack.h"
#include "message.h"
#include "csymbol.h"
#include "csymtab.h"
#include "cvc.h"
#include "cbus.h"
#include "cbustype.h"
#include "cbustype.h"
#include "cportconn.h"
#include "cinstance.h"
#include "cvcref.h"
#include "crule.h"
#include "cexception.h"


extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

Coord loc;
CObstack declHeapObject("declarations");
CObstack* declHeap = &declHeapObject;
list<CModule*> modules;
CBusType* currentBusType = NULL;
CModule*  currentModule = NULL;
CInstance*  currentInstance = NULL;
CSymtab<CDecl> topSymbolTable = symbolTable;


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
    CVcRef*    vcref;
};

%token <symbol> SYMBOL
%token '{' '}' '.' ';' ',' BUS VC CHECK RUN MODULE ARROW IGNORE FROM

%type <vcref> port_spec
%type <vc> vc_name
%type <symbol> port_name
%type <symbol> vc_name_ref
%type <bustype> bus_type
%type <module> module_name
%type <symbol> module_name_ref
%type <instance> instance_name 
%type <bus> bus_name 
%type <symbol> bus_name_ref 
%type <symbol> bus_type_ref 


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
	    { 
	       symbolTable.PushScope(); 
	       currentBusType = $2;
            }
	'{' bus_statements '}'
	    { 
	      $2->Symtab( symbolTable );
	      model.Add( $2 );
	      symbolTable.PopScope();
	    }
;

bus_statements: 
	| bus_statements bus_statement 
;

bus_statement: VC vc_name  ';'
    { 
	currentBusType->Add( $2 );
    }
    | vc_name_ref ARROW vc_name_ref ';'
    {
	CVcRef* lhs = new(declHeap) CVcRef( CSymbol::Lookup("*"), $1,  &loc );
	CVcRef* rhs = new(declHeap) CVcRef( CSymbol::Lookup("*"), $3,  &loc );
        CRule* r = new(declHeap) CRule( lhs, rhs, &loc );
        currentBusType->Add( r );
    }
;


port_spec:  bus_name_ref
	{ $$ = new(declHeap) CVcRef( $1, CSymbol::Lookup("*"), &loc ); }
        | bus_name_ref '.' '*' 
	{ $$ = new(declHeap) CVcRef( $1, CSymbol::Lookup("*"), &loc ); }
	| bus_name_ref '.' vc_name_ref
	{ $$ = new(declHeap) CVcRef( $1, $3, &loc ); }
	;	

check_statement: CHECK module_name_ref ';'
	{ model.Check( $2 ); }
	;

run_statement: RUN module_name_ref ';'
	{ model.Run( $2 ); }
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
        {
	    CPortConn* pc = new(declHeap) CPortConn( $1, CSymbol::Lookup(""), &loc );
	    currentInstance->Add( pc );
	} 
	| port_name '(' bus_name_ref ')'
        {
	    CPortConn* pc = new(declHeap) CPortConn( $1, $3, &loc );
	    currentInstance->Add( pc );
	} 
	;

module_statement: 
        module_name_ref instance_name 
	{ 
	    $2->ModuleName( $1 );
	    currentModule->Add( $2 );
 	    currentInstance = $2;
	}
	'(' port_list_o ')' ';'
	{ 
	    currentInstance = NULL;
	}
        | port_spec ARROW port_spec ';'
	{ 
	    CRule* r = new(declHeap) CRule( $1, $3, &loc );
	    currentModule->Add( r );
	}
	| IGNORE port_spec FROM module_name_ref ';'
        {
	    CException* e = new(declHeap) CException( $2, $4, &loc );
	    currentModule->Add( e );
		
	}
        | bus_type_ref ':' bus_name ';'
        {
	    $3->BusType( $1 );
	    currentModule->AddBus( $3 );
	} 
	;

module: MODULE module_name 
	    { 
		currentModule = $2;
		symbolTable.PushScope(); 
	    }
	'(' port_decl_list_o ')' '{' module_statements '}'
	    { 
	      $2->Symtab( symbolTable );
	      model.Add($2);
	      symbolTable.PopScope();
	    }
	;

port_decl_list_o: 
	| port_decl_list
	;

port_decl_list: port_decl
	| port_decl_list ',' port_decl
	;
	
port_decl: bus_type_ref ':' bus_name
        {
	    $3->BusType( $1 );
	    $3->IsPort();
	    currentModule->AddPort( $3 );
	} ;

vc_name: SYMBOL
    { $$ = CreateVariable( $1, true, (CVc*)0 ); }

vc_name_ref: SYMBOL

bus_name: SYMBOL
    { $$ = CreateVariable( $1, true, (CBus*)0 ); }

bus_name_ref: SYMBOL

bus_type_ref: SYMBOL

bus_type: SYMBOL
    { $$ = CreateVariable( $1, true, (CBusType*)0 ); }

module_name: SYMBOL
    { $$ = CreateVariable( $1, true, (CModule*)0 ); }

module_name_ref: SYMBOL

instance_name: SYMBOL
    { $$ = CreateVariable( $1, false, (CInstance*)0 ); }

port_name: SYMBOL
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
	return 0;
}
void yyerror(const char* s) {
	error( &loc, "Parse error: %s\n", s);
	exit(1);
}

