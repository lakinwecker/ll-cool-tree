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

#ifndef SUCCESSOR_H
#define SUCCESSOR_H

#include "expression.h"

#include <string>
#include <vector>

namespace LSystem {

/**
 * A successor to a production rule within an LSystem.
 *
 * A successor to a production rule within an LSystem, this class
 * contains a name, and a set of ordered expressions which are evaluated
 * when a production is matched.
 *
 * @author Lakin Wecker aka nikal@nucleus.com
 *
 * @see LSystem::Production
 */

//======================================================================
// Private Variables
//======================================================================/
typedef std::vector<Expression *> ExpressionPtrVec;

class Successor
{
	
    //======================================================================
    // Private Variables
    //======================================================================/
    private:

        std::string myName;
        ExpressionPtrVec myExpressionPtrVec;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors
	/**
	 * Copy Constructor.
	 */
    Successor( const Successor &source ) {
		myName = source.myName;
		for( ExpressionPtrVec::size_type i = 0; i < source.myExpressionPtrVec.size(); ++i ) {
			myExpressionPtrVec.push_back( new Expression( *(source.myExpressionPtrVec[i]) ) );
		}
	}
    /**
     * Assignment operator.
     */
    Successor &operator= ( const Successor &source ) {
		myName = source.myName;
		for( ExpressionPtrVec::size_type i = 0; i < myExpressionPtrVec.size(); ++i ) {
			delete myExpressionPtrVec[i];
		}
		myExpressionPtrVec.clear();
		for( ExpressionPtrVec::size_type i = 0; i < source.myExpressionPtrVec.size(); ++i ) {
			myExpressionPtrVec.push_back( new Expression( *(source.myExpressionPtrVec[i]) ) );
		}
		return *this;
	}

    /**
     * Creates a new Successor using default values.
     */
    Successor( std::string name, ExpressionPtrVec exprPtrVec )
		:
		myName( name ),
		myExpressionPtrVec( exprPtrVec )
    {
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Successor instance.
     */
    virtual ~Successor ()
    {
		for( ExpressionPtrVec::size_type i = 0; i < myExpressionPtrVec.size(); ++i ) {
			delete myExpressionPtrVec[i];
		}
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets a reference to the expression for this Successor.
     */
    const ExpressionPtrVec &getExpressionPtrVec() const
    {
        return myExpressionPtrVec;
    }

    /**
     * Gets a reference to the name of this Successor.
     */
    const std::string &getName() const
    {
        return myName;
    }


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of Successor

} // End of LSystem namespace

#endif
