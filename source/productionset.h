/*
 Copyright (C) 2004  Lakin Wecker
	
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details. 

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef PRODUCTIONSET_H
#define PRODUCTIONSET_H

#include <sstream>
#include <map>
#include <list>
#include <string>

#include "production.h"
#include "module.h"

/**
 * forward decl
 */
double randdouble(double, double);

namespace LSystem {

/**
 * Holds all the productions for an Lsystem and makes matches against modules.
 *
 * Holds all the productions for an Lsystem and allows you to match
 * them by providing a Module.  Takes care of the random chosen 
 * Productions and the ones based on Predicates.
 *
 * ProductionSet mySet;
 * mySet.addProduction( Production( production1 ) );
 * mySet.addProduction( Production( production3 ) );
 * mySet.addProduction( Production( production2 ) );
 * ...
 * Production *p = mySet.match( module ); //Randomly picks one.
 * p.evaluate( symTable );
 *
 * @author Lakin Wecker aka nikal@nucleus.com
 *
 * @see LSystem::Production
 * @see LSystem::Module
 *
 */

//======================================================================
// Typedefs
//======================================================================
typedef std::vector<Module> ModuleVec;
typedef std::map<std::string, std::list<Production> > ProductionMap;
typedef std::map<std::string, double> SymbolTable;

class ProductionSet
{

    //======================================================================
    // Private Variables
    //======================================================================/
    private:

        ProductionMap myProductions;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new ProductionSet using default values.
     */
	ProductionSet()
		:
		myProductions()
	{
    }

	/**
	* Copy constructor.
	*/
	ProductionSet( const ProductionSet &source )
	{
		// Use assignment operator to do the copy
		// NOTE: If you need to do custom initialization in the constructor this may not be enough.
		*this = source;
	}


	/**
	* Assignment operator.
	*/
	ProductionSet &operator= ( const ProductionSet &source )
	{
		//out with the old, in with the new.
		myProductions = source.myProductions;
		// Return this object with new value
		return *this;
	}


	//----------------------------------------------------------------------
	// Destructor

	/**
	* Deletes a ProductionSet instance.
	*/
	virtual ~ProductionSet ()
	{
	}


	//----------------------------------------------------------------------
	// Public API
	/**
	 * Matches and then subsequently evaluates the Module against
	 * the productions, just returns a copy of the module itself 
	 * if no match is found.
	 * 
	 */
	const ModuleVec evaluate( const Module &mod, SymbolTable table ) {
		std::ostringstream outName;
		outName << mod.name << mod.parameters.size();
		ProductionMap::iterator iter = myProductions.find( outName.str() );
		ModuleVec ret;

		if( iter == myProductions.end() ) {
			Module tmp;
			tmp.name = mod.name;
			tmp.parameters = mod.parameters;
			ret.push_back(tmp);
			return ret;
		}

		//////////////////////////////////////////////////////////////////////
		// Find the appropriate production
		double pos = 0.0;
		double rand = randdouble( 0.0, 1.0 );
		std::list<Production>::iterator i;
		for( i = iter->second.begin(); i != iter->second.end(); ++i ) {
			//randomly pick one
			if( rand <= pos + i->getProbability() ) {
				break; // we found it break out and use this one.
			} else {
				pos = pos + i->getProbability(); //increment probability
			}
		}

		////////////////////////////////////////////////////////////////////////////
		// Create the symbol lookup table.
		// Local variable silently override global values.
		IdentVec identList = i->getIdentVec();
		for( IdentVec::size_type identI = 0; identI < identList.size(); ++identI ) {
			LDEBUG( std::cout << "Symbol Table: [" << identList[identI] << "]= " 
					<< mod.parameters[identI]; )
			table[ identList[identI] ] = mod.parameters[identI];
		}
		
		///////////////////////////////////////////////////////////////////////////
		// Loop through the successors, evaluating them against the lookup
		// table to create a new list of Modules for us to return.
		SuccessorVec v = i->getSuccessorVec();
		for( SuccessorVec::size_type n = 0; n < v.size(); ++n ) {
			ExpressionPtrVec ev = v[n].getExpressionPtrVec();
			Module module;
			module.name = v[n].getName()[0];
			
			//Evaluate the expressions against the symbol table.
			for( ExpressionPtrVec::size_type e = 0; e < ev.size(); ++e ) {
				module.parameters.push_back( ev[e]->evaluateExpression( table ) );
			}	
			ret.push_back( module );
		}
		return ret;
	}

	/**
	 * Adds a production to our list.
	 *
	 * @param prod The Production to add to the set
	 */
	void push_back( Production prod ) {
		std::ostringstream outName;
		outName << prod.getName() << prod.getIdentVec().size();
		myProductions[ outName.str() ].push_back( prod );
	}
	
	/**
	 * Clears the lists.
	 */
	void clear() {
		myProductions.clear();
	}
	
	/**
	 * Normalizes the probabilities.
	 */
	void normalize() {
		ProductionMap::iterator index, end;
		index = myProductions.begin();
		end = myProductions.end();
		while( index != end ) {
			std::list<LSystem::Production>::iterator i, e;
			i = index->second.begin();
			e = index->second.end();
			double total = 0.0;
			while( i != e ) {
				total += i->getProbability();
				++i;
			}
			i = index->second.begin();
			e = index->second.end();
			while( i != e ) {
				i->setProbability( i->getProbability() / total );
				++i;
			}
			++index;
		}
	}
	//----------------------------------------------------------------------
	// Setters

	/**
	 * Sets the value of Productions of this ProductionSet
	 */
	void setProductions( std::map<std::string, std::list<Production> > productions )
	{
		myProductions = productions ;
	}

}; // End of ProductionSet

} // End of LSystem namespace

#endif
