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


extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);


%}

%union {
    char* symbol;
    rulepair rpair;
    symbolpair spair;
};

%token <symbol> SYMBOL
%token '{' '}' '.' BUS VC CHECK RUN UNIT ARROW DOUBLE_ARROW CONFIG

%type<spair> vc_spec
%type<rpair> rule 

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

bus_definition: BUS SYMBOL '{' vc_declaration_list '}'
	{ printf( "bus %s\n", $2 ); }
;

vc_declaration_list: 
	| vc_declaration_list vc_declaration
;

vc_declaration: VC SYMBOL 
    { printf( "vc %s\n", $2 ) }
;

bus_declaration: SYMBOL SYMBOL 
    { printf( "%s is an instance of BUS %s\n", $2, $1 ); }

unit_definition: 
	UNIT SYMBOL '{' unit_statements '}'
	;

unit_statements:
	| unit_statements unit_statement
	;

unit_statement: bus_declaration
	| rule
	;

rule:  vc_spec ARROW vc_spec
	{ $$.first = $1; $$.second = $3; 
	    printf( "rule: %s.%s -> %s.%s\n", $1.first, $1.second, $3.first, $3.second );
        }
	;

vc_spec:  SYMBOL
	{ $$.first = $1; $$.second = strdup("*"); }
        | SYMBOL '.' '*' 
	{ $$.first = $1; $$.second = strdup("*"); }
	| SYMBOL '.' SYMBOL
	{ $$.first = $1; $$.second = $3; }
	;

check_statement: CHECK SYMBOL
	{ printf( "check %s\n", $2 ); }
	;

run_statement: RUN SYMBOL
	{ printf( "run %s\n", $2 ); }
	;

config_statements:
	| config_statements config_statement
	;

config_statement: unit_instance
	| connect_statement
	;

configuration_block: CONFIG SYMBOL '{' config_statements '}'
	{ printf( "config %s\n", $2 ); }
	;
	
unit_instance: SYMBOL SYMBOL
	{ printf( "unit_instance %s %s\n", $1, $2 ); }
	;

connect_statement: SYMBOL '.' SYMBOL DOUBLE_ARROW SYMBOL '.' SYMBOL
	{ printf( "connect: %s.%s <=> %s.%s\n", $1, $3, $5, $7 ); }
	;

//bus_type, bus_name, vc_name, unit_name: [a-zA-Z][a-zA-Z0-9_]
//
//
//
//statement: 
//	configuration_statement
//	check_statement
//	run_statement
//
//
//configuration_statement: CONFIG config_name { (unit_instance | connect_statement)* }
//

%%
int parse( const char* filename ) {
	yyin = fopen( filename, "r" );
	if( !yyin ) {
	    fprintf( stderr, "Cannot open file: %s\n", filename );
	    return 0;
	}
	do { 
		yyparse();
	} while(!feof(yyin));
        fclose( yyin );
	yyin = NULL;
	return 0;
}
void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}

