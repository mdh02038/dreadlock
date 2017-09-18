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
%option noyywrap

%{
#include <stdio.h>
#include "defs.h"
#include "lex.h"
#include "csymbol.h"

#define YY_DECL int yylex()

#include "parse.tab.hh"
extern Coord loc;

#define NEW_LINE	loc.lineno++;

%}

%x COMMENT

%%


[ \t\f\r]	; // ignore all whitespace

"vc"		{return VC;}
"bus"		{return BUS;}
"check"		{return CHECK;}
"run"		{return RUN;}
"module"	{return MODULE;}
"ignore"	{return IGNORE;}
"from"		{return FROM;}
"{"		{return '{';}
"}"		{return '}';}
"."		{return '.';}
","		{return ',';}
"("		{return '(';}
")"		{return ')';}
";"		{return ';';}
"*"		{return '*';}
"->"		{return ARROW;}
"//".*		{}
[a-zA-Z][a-zA-Z0-9_]*	{yylval.symbol = CSymbol::Lookup(yytext); return SYMBOL;}
"/*"		{ BEGIN COMMENT; }
<COMMENT>[^*\n]* 	{}
<COMMENT>"*"+[^*/\n]* 	{ NEW_LINE; }
<COMMENT>\n		{} 
<COMMENT>"*"+"/"	{ BEGIN INITIAL; }
\n		{ yymore(); NEW_LINE; }
.		{ return yytext[0]; } 


%%

