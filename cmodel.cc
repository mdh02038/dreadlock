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

#define ERROR(args...) error(args); exit(1)

extern CObstack* declHeap;
/*
 * build model
 */
void CModel::Build( const string& topModuleName )
{
	// validate all modules
	for( list<CModule*>::const_iterator m = modules.begin(); m != modules.end(); ++m ) {
	    (*m)->Validate( symtab );
	}
	// validate all bus types
        for( list<CBusType*>::const_iterator btp=busses.begin(); btp!=busses.end(); ++btp  ) {
	    (*btp)->Validate( symtab );
	}
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
	if( topModuleName == "" ) {
            topLevelModules = CollectTopLevelModules();	
	    if( topLevelModules.size() != 1 ) {
		error( NULL, "only one top level module is allowed" );
	    }
	} else {
            CModule* m = CDecl::Resolve<CModule>( symtab, CSymbol::Lookup( topModuleName.c_str() ) );
	    if( !m ) {
	         error( NULL, "top level module '%s' does not exist", topModuleName.c_str() );
		 return;
	    }
	    topLevelModules.push_back( m );
	}
	
	for( list<CModule*>::const_iterator mp = topLevelModules.begin(); mp != topLevelModules.end(); ++mp ) {
	    checks.push_back( (*mp)->GetSymbol() );
	    runs.push_back( (*mp)->GetSymbol() );
	}

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
    "	 direction: one Direction,\n"
    "    dep: set Wire,\n"
    "    cdep: set Wire\n"
    "}\n"
    
    "pred deadlock_free {\n"
    "    acyclic[dep]\n"
    "}\n"
    "// utility functions\n"
    "pred irreflexive[rel: Wire->Wire]           { no iden & rel }\n"
    "pred acyclic[rel: Wire->Wire]               { irreflexive[^rel] }\n"
    "fact { all disj p1,p2: Wire | p1->p2 in cdep <=> p1->p2 in dep and  some p2.(^dep) :> p1 }\n"
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
;

/*
 * dump alloy model to file
 */
void CModel::DumpAlloy( FILE* f )
{
    /*
     * calculate port dependencies
     */
    map<string,map<string,set<string> > > deps;
    map<string,map<string,set<string> > > rdeps;
    for( list<CInstance*>::const_iterator up = instances.begin(); up != instances.end(); ++up ) {
        CModule* m = CDecl::Resolve<CModule>( symtab, (*up)->ModuleName() );
	ASSERT( m ); // OK
	string instanceName = (*up)->GetName();
	for( list<CBus*>::const_iterator bp = m->Ports().begin(); bp != m->Ports().end(); ++bp ) {
	    CBusType* busType = CDecl::Resolve<CBusType>( symtab, (*bp)->BusType() );
	    ASSERT( busType );
	    string busName = (*bp)->GetName();
	    list<CVc*>::const_iterator vp;
	    for( vp = busType->Vcs().begin(); vp != busType->Vcs().end(); ++vp ) {
	        string vcName = (*vp)->GetName();
	        string portNameIn = (*up)->GetName() + string("_") + busName + "_" + vcName + string("_IN");
		deps[instanceName][portNameIn].clear();
	        string portNameOut = (*up)->GetName() + string("_") + busName + "_" + vcName + string("_OUT");
		rdeps[instanceName][portNameOut].clear();
            }
        }
	for( list<CRule*>::const_iterator rp = m->Rules().begin(); rp != m->Rules().end(); ++rp ) {
	    // ??? mdh - validate these bus/vc combinations are valid
	    string lhs_bus = (*rp)->LHS()->Bus()->GetName();;
	    string lhs_vc = (*rp)->LHS()->Vc()->GetName();;
	    string rhs_bus = (*rp)->RHS()->Bus()->GetName();;
	    string rhs_vc = (*rp)->RHS()->Vc()->GetName();;
            CBus* lb = CDecl::Resolve<CBus>( m->Symtab(), (*rp)->LHS()->Bus() );
	    ASSERT( lb );
	    CBusType* lbt = CDecl::Resolve<CBusType>( symtab, lb->BusType() );
	    ASSERT( lbt );
            CBus* rb = CDecl::Resolve<CBus>( m->Symtab(), (*rp)->RHS()->Bus() );
	    ASSERT( rb );
	    CBusType* rbt = CDecl::Resolve<CBusType>( symtab, rb->BusType() );
	    ASSERT( rbt );
	    if( lhs_vc == "*" && rhs_vc == "*" ) {
		// Vector dependency
  	        ASSERT( lbt == rbt );
		for( list<CVc*>::const_iterator vp = lbt->Vcs().begin(); vp != lbt->Vcs().end(); ++vp ) {
	            string lhs = string((*up)->GetName()) + "_" + lhs_bus + "_" + (*vp)->GetName() + string("_IN");
	            string rhs = string((*up)->GetName()) + "_" + rhs_bus + "_" + (*vp)->GetName() + string("_OUT");
	            deps[instanceName][lhs].insert(rhs); 
	            rdeps[instanceName][rhs].insert(lhs); 
		}
	    } else if( lhs_vc != "*" && rhs_vc != "*" ) {
		// scalar dependency
	        string lhs = string((*up)->GetName()) + "_" + lhs_bus + "_" + lhs_vc + string("_IN");
	        string rhs = string((*up)->GetName()) + "_" + rhs_bus + "_" + rhs_vc + string("_OUT");
	        deps[instanceName][lhs].insert(rhs); 
	        rdeps[instanceName][rhs].insert(lhs); 
	    } else {
		// one to many or many to one dependency
		// ??? mdh - add hybrid checks
	        ASSERT( false );
	    }
        }
    }
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
    for( up = instances.begin(); up != instances.end();  ) {
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
	CModule* m = CDecl::Resolve<CModule>( symtab, (*ip)->ModuleName() );
	ASSERT( m );
	string instanceName = (*ip)->GetName();
        fprintf( f, "\n// wire sigs for %s \n", instanceName.c_str() );
	list<CBus*>::const_iterator bp;
	for( bp = m->Ports().begin(); bp != m->Ports().end(); ++bp ) {
	    CBusType* busType = CDecl::Resolve<CBusType>( symtab, (*bp)->BusType() );
	    ASSERT( busType );
	    string busName = (*bp)->GetName();
	    list<CVc*>::const_iterator vp;
	    for( vp = busType->Vcs().begin(); vp != busType->Vcs().end(); ++vp ) {
	        string vcName = (*vp)->GetName();
	        for( unsigned d = 0; d < 2; ++d ) {
	            string direction = directions[d];
	            string portName = instanceName + "_" + busName + "_" + vcName + "_" + direction;
		    if( direction == "IN" && deps[instanceName][portName].size() == 0 ) {
			fprintf( f, "// " );
		    }
		    if( direction == "OUT" && rdeps[instanceName][portName].size() == 0 ) {
			fprintf( f, "// " );
		    }
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
	map<CSymbol*,set<CSymbol*> > busDeps;
        fprintf( f, "// %s connection rules", (*btp)->GetName() );
        fprintf( f, "\npred %s_noconnect[ b: Bus, u: Unit ] {\n", (*btp)->GetName() );
	for( list<CVc*>::const_iterator vcp = (*btp)->Vcs().begin(); vcp != (*btp)->Vcs().end(); ++vcp) {
            fprintf( f, "    all v: %s, d:  OUT | no dep[ u.~unit & b.~bus & v.~vc & d.~direction ]\n", (*vcp)->GetName() );
	    busDeps[(*vcp)->GetSymbol()].insert( (*vcp)->GetSymbol() );
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
	    busDeps[lhs_vc].insert( rhs_vc );
	}
        fprintf( f, "\npred %s_connect[ b1: Bus, u1: Unit, b2: Bus, u2: Unit ] {\n", (*btp)->GetName() );
	for( list<CVc*>::const_iterator vcp = (*btp)->Vcs().begin(); vcp != (*btp)->Vcs().end(); ++vcp) {
	    CSymbol* lhs_vc = (*vcp)->GetSymbol();
	    string rhs;
	    for( set<CSymbol*>::const_iterator sp = busDeps[lhs_vc].begin(); sp != busDeps[lhs_vc].end(); ++sp ) {
		if( sp != busDeps[lhs_vc].begin() ) {
		    rhs += " + ";
		}
		rhs += (*sp)->GetName();
	    }
	    CSymbol* rhs_vc = *(busDeps[lhs_vc].begin());
//            fprintf( f, "    all v1: %s, v2: %s | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = \n", 
//				lhs_vc->GetName(), rhs.c_str() );
//            fprintf( f, "				u2.~unit & b2.~bus & v2.~vc & IN.~direction\n" );
//            fprintf( f, "    all v1: %s, v2: %s | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = \n", 
//				lhs_vc->GetName(), rhs.c_str() );
//            fprintf( f, "					u1.~unit & b1.~bus & v2.~vc & IN.~direction\n" );

            fprintf( f, "    all v1: %s | let p = u1.~unit & b1.~bus & v1.~vc & OUT.~direction | some p => dep[p] = \n", 
				lhs_vc->GetName() );
            fprintf( f, "				u2.~unit & b2.~bus & (%s).~vc & IN.~direction\n", rhs.c_str() );
            fprintf( f, "    all v1: %s | let p = u2.~unit & b2.~bus & v1.~vc & OUT.~direction | some p => dep[p] = \n", 
				lhs_vc->GetName() );
            fprintf( f, "					u1.~unit & b1.~bus & (%s).~vc & IN.~direction\n", rhs.c_str() );
	}
        fprintf( f, "}\n" );
    }

    /*
     * dump module rules
     */
    fprintf( f, "\n// instance rules\n" );
    for( up = instances.begin(); up != instances.end(); ++up ) {
        CModule* m = CDecl::Resolve<CModule>( symtab, (*up)->ModuleName() );
	ASSERT( m );
        fprintf( f, "\n// %s rules\n", (*up)->GetName() );
        fprintf( f, "fact {\n" );
	string instanceName = (*up)->GetName();
	for( map<string,set<string> >::const_iterator sp = deps[instanceName].begin(); sp != deps[instanceName].end(); ++sp ) {
	    string lhs = sp->first;
	    if( sp->second.size() == 0 ) {
	        fprintf( f, "//    no dep[%s]\n", lhs.c_str() );
	    } else {
	        string rhs;
	        for( set<string>::const_iterator stp = sp->second.begin(); stp != sp->second.end(); ++stp ) {
	            if( stp != sp->second.begin() ) {
	                rhs += " + ";
	            }
	            rhs += *stp; 
	        }
	        fprintf( f, "    dep[%s] =  %s\n", lhs.c_str(), rhs.c_str() );
	    }
	    
	}
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
        CModule* m = CDecl::Resolve<CModule>( symtab, (*currentInstance)->ModuleName() );
	ASSERT( m );
        for( ip = m->Instances().begin(); ip != m->Instances().end(); ++ip ) {
            ASSERT( m );
            list<CPortConn*>::const_iterator pcp;
            for( pcp = (*ip)->PortConnections().begin(); pcp != (*ip)->PortConnections().end(); ++pcp ) {
                ConnectionInfo ci;
	        CModule* definingModule = CDecl::Resolve<CModule>( symtab, (*ip)->ModuleName() );
	        ASSERT( definingModule );
	        CBus* b = CDecl::Resolve<CBus>( definingModule->Symtab(), (*pcp)->Internal() ); 
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
	      CBus* b1 = CDecl::Resolve<CBus>( msymtab, mcip->first ); 
	      ASSERT( b1 );
              string instance1 = mcip->second[0].instance->GetName();
              string bus2 = mcip->second[1].name->GetName();
	      CBus* b2 = CDecl::Resolve<CBus>( msymtab, mcip->first ); 
	      ASSERT( b2 );
              string instance2 = mcip->second[1].instance->GetName();
	      ASSERT( b1->BusType() == b2->BusType() );
	      CBusType* bt = CDecl::Resolve<CBusType>( symtab, b1->BusType() );
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
    fprintf( f, "\n// Check statements\n" );
    list<CSymbol*>::iterator cp;
    for( cp=runs.begin(); cp!=runs.end(); ++cp ) {
	fprintf( f, "check %s { %s => deadlock_free }\n", (*cp)->GetName(), (*cp)->GetName() );
    }

    /* 
     * dump check statements
     */
    fprintf( f, "\n// Run statements\n" );
    list<CSymbol*>::iterator rp;
    for( rp=runs.begin(); rp!=runs.end(); ++rp ) {
	fprintf( f, "run %s { %s }\n", (*rp)->GetName(), (*rp)->GetName() );
    }

}


const list<CModule*> CModel::CollectTopLevelModules()
{
    set<CModule*> checkSet;
    set<CModule*> runSet;
    
    for( list<CSymbol*>::const_iterator mp = checks.begin(); mp != checks.end(); ++mp ) {
        CModule* m = CDecl::Resolve<CModule>( symtab, *mp );
	ASSERT( m );
	checkSet.insert( m );
    }
    for( list<CSymbol*>::const_iterator mp = runs.begin(); mp != runs.end(); ++mp ) {
        CModule* m = CDecl::Resolve<CModule>( symtab, *mp );
	ASSERT( m );
	runSet.insert( m );
    }
    map<CModule*,unsigned long> moduleRefCount;
    list<CModule*>::iterator mp;
    for( mp = modules.begin(); mp != modules.end(); ++mp ) {
	list<CInstance*>::const_iterator ip;
	for( ip = (*mp)->Instances().begin(); ip != (*mp)->Instances().end(); ++ip ) {
	    CModule* m = CDecl::Resolve<CModule>( symtab, (*ip)->ModuleName() );
	    ASSERT( m );
	    moduleRefCount[m]++;
	}
    }
    list<CModule*> topModules;
    /*
     * top level modules must have no ports
     * must have no ports and must no be instantiated by other modules
     */
    for( mp = modules.begin(); mp != modules.end(); ++mp ) {
	if( moduleRefCount[*mp] == 0 && (*mp)->Ports().size() == 0 ) {
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
	string newPrefix = prefix + "_" + (*ip)->GetName();
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
	CModule* currentModule = CDecl::Resolve<CModule>( symbolTable, (*ip)->ModuleName() );
	if( !currentModule ) return;
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
