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
 *	   main.h
 *		- globals for main
 *
 ******************************************************************************
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "defs.h"
#include "cmodel.h"
#include "cdecl.h"

extern CModel model;
extern CSymtab<CDecl> symbolTable;;

#endif // MAIN_H
