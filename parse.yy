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
#include "message.h"


extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

Coord loc;


%}

%union {
    char* symbol;
    rulepair rpair;
    symbolpair spair;
};

%token <symbol> SYMBOL
%token '{' '}' '.' BUS VC CHECK RUN UNIT ARROW DOUBLE_ARROW CONFIG

%type<spair> port_spec
%type<rpair> rule 
%type <symbol> vc_name bus_type bus_name unit_type unit_name config_name


%start description

%%
description: 
	| description statement
;

statement: bus_definition
	| unit_definition
	| check_statement
	| run_statement
	| configuration_block
;

bus_definition: BUS bus_type '{' vc_declaration_list '}'
	{ printf( "bus %s\n", $2 ); }
;

vc_declaration_list: 
	| vc_declaration_list vc_declaration
;

vc_declaration: VC vc_name 
    { printf( "vc %s\n", $2 ) }
;

bus_declaration: bus_type bus_name 
    { printf( "%s is an instance of BUS %s\n", $2, $1 ); }

unit_definition: 
	UNIT unit_type '{' unit_statements '}'
	;

unit_statements:
	| unit_statements unit_statement
	;

unit_statement: bus_declaration
	| rule
	;

rule:  port_spec ARROW port_spec
	{ $$.first = $1; $$.second = $3; 
	    printf( "rule: %s.%s -> %s.%s\n", $1.first, $1.second, $3.first, $3.second );
        }
	;

port_spec:  bus_name
	{ $$.first = $1; $$.second = strdup("*"); }
        | bus_name '.' '*' 
	{ $$.first = $1; $$.second = strdup("*"); }
	| bus_name '.' vc_name
	{ $$.first = $1; $$.second = $3; }
	;

check_statement: CHECK config_name
	{ printf( "check %s\n", $2 ); }
	;

run_statement: RUN config_name
	{ printf( "run %s\n", $2 ); }
	;

config_statements:
	| config_statements config_statement
	;

config_statement: unit_instance
	| connect_statement
	;

configuration_block: CONFIG config_name '{' config_statements '}'
	{ printf( "config %s\n", $2 ); }
	;
	
unit_instance: unit_type unit_name
	{ printf( "unit_instance %s %s\n", $1, $2 ); }
	;

connect_statement: bus_name '.' vc_name DOUBLE_ARROW bus_name '.' vc_name
	{ printf( "connect: %s.%s <=> %s.%s\n", $1, $3, $5, $7 ); }
	;

vc_name: SYMBOL
bus_type: SYMBOL
bus_name: SYMBOL
unit_type: SYMBOL
unit_name: SYMBOL
config_name: SYMBOL

//bus_type, bus_name, vc_name, unit_name: [a-zA-Z][a-zA-Z0-9_]

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

