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

#include <set>
#include <vector>
#include <string.h>
#include "main.h"
#include "cmodel.h"
#include "cbus.h"
#include "cportconn.h"

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
    "abstract sig Wire {\n"
    "    unit: one Unit,\n"
    "    bus: one Bus,\n"
    "    vc: one Vc,\n"
    "	direction: one Direction,\n"
    "    dep: set Wire\n"
    "}\n"
    
    "pred deadlock_free {\n"
    "    acyclic[dep]\n"
    "}\n"
    "// utility functions\n"
    "pred irreflexive[rel: Wire->Wire]           { no iden & rel }\n"
    "pred acyclic[rel: Wire->Wire]               { irreflexive[^rel] }\n"
;

/*
 * dump alloy model to file
 */
void CModel::DumpAlloy( FILE* f )
{
    /*
     * dump out common sigs and functions
     */
    fprintf( f, "%s", boilerPlate );

    /*
     * dump port sigs
     */
    fprintf( f, "\n// Port signatures\n" );
    fprintf( f, "one sig " );
    set<CSymbol*>::const_iterator pp;
    for( pp = ports.begin(); pp != ports.end(); ) {
	fprintf( f, "%s", (*pp)->GetName() );
	pp++;
	if( pp!=ports.end() ) {
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
    set<CSymbol*> vcNames;
    list<CBusType*>::const_iterator btp;
    for( btp=busses.begin(); btp!=busses.end(); ++btp  ) {
	list<CVc*>::const_iterator vp;
	for( vp = (*btp)->Vcs().begin(); vp != (*btp)->Vcs().end(); ++vp ) {
 	    vcNames.insert( (*vp)->GetSymbol() );	
        }
    }
    fprintf( f, "\n// Vc signatures\n" );
    fprintf( f, "one sig " );
    set<CSymbol*>::const_iterator sp;
    for( sp = vcNames.begin(); sp != vcNames.end();  ) {
	fprintf( f, "%s", (*sp)->GetName() );
	sp++;
	if( sp!=vcNames.end() ) {
	    fprintf( f, ", " );
	}
    }
    fprintf( f, " extends Vc {}\n" );


    /*
     * dump unit sigs
     */
    fprintf( f, "\n// Unit signatures\n" );
    fprintf( f, "one sig " );
    list<CInstance*>::const_iterator up;
    for( up = instances.begin(); up != instances.end(); ++up ) {
	fprintf( f, "%s", (*up)->GetName() );
	up++;
	if( up!=instances.end() ) {
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
	CModule* m = CModel::Resolve<CModule>( symtab, (*ip)->ModuleName() );
	ASSERT( m );
	string instanceName = (*ip)->GetName();
        fprintf( f, "\n// wire sigs for %s \n", instanceName.c_str() );
	list<CBus*>::const_iterator bp;
	for( bp = m->Ports().begin(); bp != m->Ports().end(); ++bp ) {
	    CBusType* busType = CModel::Resolve<CBusType>( symtab, (*bp)->BusType() );
	    ASSERT( busType );
	    string busName = (*bp)->GetName();
	    list<CVc*>::const_iterator vp;
	    for( vp = busType->Vcs().begin(); vp != busType->Vcs().end(); ++vp ) {
	        string vcName = (*vp)->GetName();
	        for( unsigned d = 0; d < 2; ++d ) {
	            string direction = directions[d];
	            string portName = instanceName + "_" + busName + "_" + vcName + "_" + direction;
                    fprintf( f, "one sig %s extends Wire {} { unit=%s bus=%s vc=%s direction=%s }\n", 
                          portName.c_str(), instanceName.c_str(), busName.c_str(), vcName.c_str(), direction.c_str() );
	        }
            }
        }
    }

    /*
     * dump bus connection rules
     */
    fprintf( f, "\n\n// bus connection rules" );
    for( btp=busses.begin(); btp!=busses.end(); ++btp  ) {
        // ??? mdh fix this to create bus specific connection rules
	map<CSymbol*,set<CSymbol*> > deps;
        fprintf( f, "// %s connection rules", (*btp)->GetName() );
        fprintf( f, "\npred %s_noconnect[ b: Bus, u: Unit ] {\n", (*btp)->GetName() );
	for( list<CVc*>::const_iterator vcp = (*btp)->Vcs().begin(); vcp != (*btp)->Vcs().end(); ++vcp) {
            fprintf( f, "    all v: %s, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]\n", (*vcp)->GetName() );
	    deps[(*vcp)->GetSymbol()].insert( (*vcp)->GetSymbol() );
	}
        fprintf( f, "}\n" );
        fprintf( f, "\n" );
	for( list<CRule*>::const_iterator rp = (*btp)->Rules().begin(); rp != (*btp)->Rules().end(); ++rp ) {
	    // ??? mdh - validate these bus/vc combinations are valid
	    CSymbol* lhs_bus = (*rp)->LHS()->Bus();
	    CSymbol* lhs_vc = (*rp)->LHS()->Vc();
	    CSymbol* rhs_bus = (*rp)->RHS()->Bus();
	    CSymbol* rhs_vc = (*rp)->RHS()->Vc();
	    ASSERT( !strcmp( lhs_bus->GetName(), "*" ) && !strcmp( rhs_bus->GetName(), "*" ) );
	    deps[lhs_vc].insert( rhs_vc );
	}
        fprintf( f, "\npred %s_connect[ b1: Bus, u1: Unit, b2: Bus, u2: Unit ] {\n", (*btp)->GetName() );
	for( list<CVc*>::const_iterator vcp = (*btp)->Vcs().begin(); vcp != (*btp)->Vcs().end(); ++vcp) {
	    CSymbol* lhs_vc = (*vcp)->GetSymbol();
	    string rhs;
	    for( set<CSymbol*>::const_iterator sp = deps[lhs_vc].begin(); sp != deps[lhs_vc].end(); ++sp ) {
		if( sp != deps[lhs_vc].begin() ) {
		    rhs += "+";
		}
		rhs += (*sp)->GetName();
	    }
	    CSymbol* rhs_vc = *(deps[lhs_vc].begin());
            fprintf( f, "    all v1: %s, v2: %s | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = \n", 
				lhs_vc->GetName(), rhs.c_str() );
            fprintf( f, "				u2.~unit & b2.~bus & v2.~vc & IN.~direction\n" );
            fprintf( f, "    all v1: %s, v2: %s | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = \n", 
				lhs_vc->GetName(), rhs.c_str() );
            fprintf( f, "					u1.~unit & b1.~bus & v2.~vc & IN.~direction\n" );
	}
        fprintf( f, "}\n" );
    }

    /*
     * dump module rules
     */
    // ??? mdh add instance rule generation
    fprintf( f, "\n// instance rules\n" );
    for( up = instances.begin(); up != instances.end(); ++up ) {
        fprintf( f, "\n// %s rules\n", (*up)->GetName() );
        fprintf( f, "fact {\n" );
        fprintf( f, "}\n" );
    }

    /*
     * dump module connections
     */
    CSymbol* empty = CSymbol::Lookup("");
    list<CInstance*>::const_iterator currentInstance;
    for( currentInstance = instances.begin(); currentInstance != instances.end(); ++currentInstance ) {
        map<CSymbol*,vector<ConnectionInfo> > name2Connection;
        vector<ConnectionInfo> noConnects;

        // find all connections
        CModule* m = CModel::Resolve<CModule>( symtab, (*currentInstance)->ModuleName() );
	ASSERT( m );
        for( ip = m->Instances().begin(); ip != m->Instances().end(); ++ip ) {
            ASSERT( m );
            list<CPortConn*>::const_iterator pcp;
            for( pcp = (*ip)->PortConnections().begin(); pcp != (*ip)->PortConnections().end(); ++pcp ) {
                ConnectionInfo ci;
	        CModule* definingModule = CModel::Resolve<CModule>( symtab, (*ip)->ModuleName() );
	        ASSERT( definingModule );
	        CBus* b = CModel::Resolve<CBus>( definingModule->Symtab(), (*pcp)->Internal() ); 
	        ASSERT( b );
                ci.instance = *ip;
                ci.name     = (*pcp)->Internal();
		ci.busType = b->BusType();
                name2Connection[(*pcp)->External()].push_back( ci );
            }
        }

	if( name2Connection.size() == 0 ) {
	    continue;
	}

        fprintf( f, "\n// connections for %s {\n", (*currentInstance)->GetName() );
        fprintf( f, "pred %s {\n", (*currentInstance)->GetName() );

	CSymtab<CDecl> msymtab = m->Symtab();
	string prefix = (*currentInstance)->GetName();
        map<CSymbol*,vector<ConnectionInfo> >::iterator mcip;
        for( mcip = name2Connection.begin(); mcip != name2Connection.end(); ++mcip ) {
            if( mcip->first == empty ) {
                noConnects = mcip->second;
            } else if( mcip->second.size() < 2 ) {
                ASSERT( false );
            } else if( mcip->second.size() == 2 ) {
              string bus1 = mcip->second[0].name->GetName();
	      CBus* b1 = CModel::Resolve<CBus>( msymtab, mcip->first ); 
	      ASSERT( b1 );
              string instance1 = mcip->second[0].instance->GetName();
              string bus2 = mcip->second[1].name->GetName();
	      CBus* b2 = CModel::Resolve<CBus>( msymtab, mcip->first ); 
	      ASSERT( b2 );
              string instance2 = mcip->second[1].instance->GetName();
	      ASSERT( b1->BusType() == b2->BusType() );
	      CBusType* bt = CModel::Resolve<CBusType>( symtab, b1->BusType() );
	      ASSERT( bt );
	      ASSERT( bt->GetSymbol() == mcip->second[0].busType );
	      ASSERT( bt->GetSymbol() == mcip->second[1].busType );
	      instance1 = prefix + "_" + instance1;
	      instance2 = prefix + "_" + instance2;
              fprintf( f, "\t%s_connect[%s, %s, %s, %s]\n", b1->BusType()->GetName(), bus1.c_str(), 
				instance1.c_str(), bus2.c_str(), instance2.c_str() );
            } else {
                ASSERT( false );
            }
        }

        vector<ConnectionInfo>::const_iterator cip;
        for( cip = noConnects.begin(); cip != noConnects.end(); ++cip ) {
              string bus = cip->name->GetName();
              string instance = cip->instance->GetName();
	      string busType = cip->busType->GetName();
	      instance = prefix + "_" + instance;
              fprintf( f, "\t%s_noconnect[%s, %s]\n", busType.c_str(), bus.c_str(), instance.c_str() );
        }
        fprintf( f, "}\n" );
    }


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
	    CModule* m = CModel::Resolve<CModule>( symtab, (*ip)->ModuleName() );
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

void Elaborate( string prefix, CModule* m, list<CInstance*>& instances, set<CSymbol*>& ports )
{
    list<CInstance*>::const_iterator ip;
    for( ip = m->Instances().begin(); ip != m->Instances().end(); ++ip ) {
	string moduleName = (*ip)->ModuleName()->GetName();
	string newPrefix = prefix + "_" + moduleName;
	CInstance* instance = new(declHeap) CInstance( CSymbol::Lookup(newPrefix.c_str()),(*ip)->GetCoord() );
	instance->ModuleName( (*ip)->ModuleName() );
        list<CPortConn*>::const_iterator pcp;
	for( pcp = (*ip)->PortConnections().begin(); pcp != (*ip)->PortConnections().end(); ++pcp ) {
	    string newName = (*pcp)->External()->GetName();	
	    if( newName.size() > 0 ) {
 	        newName = prefix + "_" + newName;	
	    }
	    CPortConn* pc = new(declHeap) CPortConn( (*pcp)->GetSymbol(), CSymbol::Lookup(newName.c_str()), (*pcp)->GetCoord() );
	    instance->Add( pc );
	}
	instances.push_back( instance );
	CModule* currentModule = CModel::Resolve<CModule>( symbolTable, (*ip)->ModuleName() );
        list<CBus*>::const_iterator bp;
	for( bp = currentModule->Ports().begin(); bp != currentModule->Ports().end(); ++bp ) {
	    ports.insert( (*bp)->GetSymbol() );	
	}
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
	Elaborate( newPrefix, *mp, instances, ports );
    }
    
    return instances;
}
