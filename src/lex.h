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

#ifndef LEX_H
#define LEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "defs.h"
#include "csymbol.h"
#include "cmodule.h"
#include "cbustype.h"
#include "cbus.h"


typedef struct {
    CBus* first;
    CSymbol* second;
} symbolpair;

#endif // LEX_H


