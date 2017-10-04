/*****************************************************************************
 * Copyright (C) 1997-2010, Mark Hummel
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
 *	   model.cc
 *		- class definition of model
 *
 ******************************************************************************
 */

#include "cmodel.h"
#include "cbus.h"


extern CObstack* declHeap;

/*
 * build model
 */
void CModel::Build()
{
	// bus rule references
	// model rule references
	// exception references
	// port list match
	// check/run model names are valid
	// check for model recursion
	// rules are consistant
	// bus/wire connectivity
	// unconnected busses
	// non-point to point connections
	// instance refs use valid models

	// collect top level models
        topLevelModules = CollectTopLevelModules();	

        // flatten model extract instances
	instances = FlattenAndExtractInstances( topLevelModules );
}

/*
 * dump model to file
 */
void CModel::Dump( FILE* f )
{
    fprintf( f, "Symbol Table:\n" );
    symtab.Dump( f );
    list<CBusType*>::iterator btp;
    for( btp=busses.begin(); btp!=busses.end(); ++btp ) {
	(*btp)->Dump( f );
    }
    set<CModule*> topSet(topLevelModules.begin(), topLevelModules.end());
    list<CModule*>::iterator mp;
    for( mp=modules.begin(); mp!=modules.end(); ++mp ) {
	if( topSet.count(*mp) > 0 ) {
	    fprintf( f, "#" );
	}
	(*mp)->Dump( f );
    }
    list<CSymbol*>::iterator cp;
    for( cp=checks.begin(); cp!=checks.end(); ++cp ) {
	fprintf( f, "check: %s\n", (*cp)->GetName() );
    }
    list<CSymbol*>::iterator rp;
    for( rp=runs.begin(); rp!=runs.end(); ++rp ) {
	fprintf( f, "run: %s\n", (*rp)->GetName() );
    }
    list<CInstance*>::iterator ip;
    for( ip=instances.begin(); ip!=instances.end(); ++ip ) {
	(*ip)->Dump( f );
    }
}

const char* boilerPlate = 
    "abstract sig Direction {}\n"
    "one sig IN, OUT extends Direction {}\n"
    "abstract sig Bus {}\n"
    "abstract sig Vc {}\n"
    "abstract sig Boolean {}\n"
    "abstract sig Unit {}\n"
    "abstract sig Port {\n"
    "    unit: one Unit,\n"
    "    bus: one Bus,\n"
    "    vc: one Vc,\n"
    "	direction: one Direction,\n"
    "    dep: set Port\n"
    "}\n"
    
    "pred deadlock_free {\n"
    "    acyclic[dep]\n"
    "}\n"
    "pred noconnect[ b: Bus, u: Unit ] {\n"
    "    all v: Vc, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]\n"
    "}\n"
    "\n"
    "pred connect[ b1: Bus, u1: Unit, b2: Bus, u2: Unit ] {\n"
    "    all v: Vc | let p = u1.~unit & b1.~bus & v.~vc & OUT.~direction | some p => dep[p] = \n"
    "					u2.~unit & b2.~bus & v.~vc & IN.~direction\n"
    "    all v: Vc | let p = u2.~unit & b2.~bus & v.~vc & OUT.~direction | some p => dep[p] = \n"
    "					u1.~unit & b1.~bus & v.~vc & IN.~direction\n"
    "}\n"
    "// utility functions\n"
    "pred irreflexive[rel: Port->Port]           { no iden & rel }\n"
    "pred acyclic[rel: Port->Port]                { irreflexive[^rel] }\n"
;

/*
 * dump alloy model to file
 */
void CModel::DumpAlloy( FILE* f )
{
//    list<CBusType*> busses;
//    list<CModule*>  modules;
//    CSymtab<CDecl>  symtab;
//    list<CSymbol*>  runs;
//    list<CSymbol*>  checks;


    /*
     * dump out common sigs and functions
     */
    fprintf( f, "%s", boilerPlate );

    /*
     * dump bus sigs
     */
    fprintf( f, "\n// Bus signatures\n" );
    list<CBusType*>::iterator btp;
    fprintf( f, "one sig " );
    for( btp=busses.begin(); btp!=busses.end();  ) {
	fprintf( f, "%s", (*btp)->GetName() );
	btp++;
	if( btp!=busses.end() ) {
	    fprintf( f, ", " );
	}
    }
    fprintf( f, " extends Bus {}\n" );

    /*
     * dump bus rules
     */

    /*
     * dump vc sigs
     */

    /*
     * dump unit sigs
     */
    fprintf( f, "\n// Unit signatures\n" );
    list<CModule*>::iterator up;
    fprintf( f, "one sig " );
    for( up=modules.begin(); up!=modules.end();  ) {
	fprintf( f, "%s", (*up)->GetName() );
	up++;
	if( up!=modules.end() ) {
	    fprintf( f, ", " );
	}
    }
    fprintf( f, " extends Unit {}\n" );

    /*
     * dump instance wire sigs
     */
    string directions[2] = { "IN", "OUT" };
    list<CInstance*>::const_iterator ip;
    for( ip = instances.begin(); ip != instances.end(); ++ip ) {
	CDecl* d = symtab.Lookup( (*ip)->ModuleName() );
	ASSERT( d );
	ASSERT( d->GetType() == eMODULE );
	CModule* m = (CModule*)d;
	string instanceName = (*ip)->GetName();
        fprintf( f, "\n// %s \n", instanceName.c_str() );
	list<CBus*>::const_iterator bp;
	for( bp = m->Ports().begin(); bp != m->Ports().end(); ++bp ) {
	    string busName = (*bp)->GetName();
	    CDecl* d = symtab.Lookup( (*bp)->BusType() );
	    ASSERT( d );
	    ASSERT( d->GetType() == eBUS_TYPE );
	    CBusType* busType = (CBusType*)d;
	    list<CVc*>::const_iterator vp;
	    for( vp = busType->Vcs().begin(); vp != busType->Vcs().end(); ++vp ) {
	        string vcName = (*vp)->GetName();
	        for( unsigned d = 0; d < 2; ++d ) {
	            string direction = directions[d];
	            string portName = instanceName + "_" + busName + "_" + vcName + "_" + direction;
                    fprintf( f, "one sig %s extends Port {} { unit=%s bus=%s vc=%s direction=%s }\n", 
                          portName.c_str(), instanceName.c_str(), busName.c_str(), vcName.c_str(), direction.c_str() );
	        }
            }
        }
    }

    /*
     * dump module rules
     */

    /*
     * dump module connections
     */

    /*
     * dump run statements
     */
    fprintf( f, "\n// Run statements\n" );
    list<CSymbol*>::iterator cp;
    for( cp=runs.begin(); cp!=runs.end(); ++cp ) {
	fprintf( f, "check %s { %s => deadlock_free }\n", (*cp)->GetName(), (*cp)->GetName() );
    }

    /* 
     * dump check statements
     */
    fprintf( f, "\n// Check statements\n" );
    list<CSymbol*>::iterator rp;
    for( rp=runs.begin(); rp!=runs.end(); ++rp ) {
	fprintf( f, "run %s { %s }\n", (*rp)->GetName(), (*rp)->GetName() );
    }

}


const list<CModule*> CModel::CollectTopLevelModules()
{
    map<CModule*,unsigned long> moduleCount;
    list<CModule*>::iterator mp;
    for( mp = modules.begin(); mp != modules.end(); ++mp ) {
	list<CInstance*>::const_iterator ip;
	for( ip = (*mp)->Instances().begin(); ip != (*mp)->Instances().end(); ++ip ) {
	    CDecl* d = symtab.Lookup( (*ip)->ModuleName() );
	    ASSERT( d );
	    ASSERT( d->GetType() == eMODULE );
	    CModule* m = (CModule*)d;
	    moduleCount[m]++;
	}
    }
    list<CModule*> topModules;
    for( mp = modules.begin(); mp != modules.end(); ++mp ) {
	if( moduleCount[*mp] == 0 ) {
	    topModules.push_back( *mp );
	}
    }
    return topModules;
}

void Elaborate( string prefix, CModule* m, list<CInstance*>& instances )
{
    list<CInstance*>::const_iterator ip;
    for( ip = m->Instances().begin(); ip != m->Instances().end(); ++ip ) {
	string moduleName = (*ip)->ModuleName()->GetName();
	string newPrefix = prefix + "_" + moduleName;
	CInstance* instance = new(declHeap) CInstance( CSymbol::Lookup(newPrefix.c_str()),(*ip)->GetCoord() );
	instance->ModuleName( (*ip)->ModuleName() );
	instances.push_back( instance );
	// ??? mdh - need to find module decl an recurse
    }
}

const list<CInstance*> CModel::FlattenAndExtractInstances( const list<CModule*>& modules )
{
    list<CInstance*> instances;
    /*
     * create an instance for each top level model
     */
    list<CModule*>::const_iterator mp;
    for( mp = modules.begin(); mp != modules.end(); ++mp ) {
	string moduleName = (*mp)->GetName();
	string newPrefix = moduleName;
	CInstance* instance = new(declHeap) CInstance( CSymbol::Lookup(newPrefix.c_str()),(*mp)->GetCoord() );
	instance->ModuleName( (*mp)->GetSymbol() );
	instances.push_back( instance );
	/*
	 * build sub instances
	 */
	Elaborate( newPrefix, *mp, instances );
    }
    
    return instances;
}
