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

#ifndef PRODUCTION_H
#define PRODUCTION_H

#include "module.h"
#include "successor.h"
#include <vector>
#include <string>

namespace LSystem {

/**
 * A production in the LSystem.
 *
 * A production in the LSystem, this contains it's predecessor,
 * it's probability, and it's successor's.
 *
 * @author Lakin Wecker aka nikal@nucleus.com
 *
 * @see LSystem::Module
 * @see LSystem::Successor
 */

//======================================================================
// Typedefs
//======================================================================/
typedef std::vector<std::string> IdentVec;
typedef std::vector<Successor> SuccessorVec;

class Production
{

    //======================================================================
    // Private Variables
    //======================================================================/
    private:

		std::string myName;
        double myProbability;
        IdentVec myIdentifierVec;
        SuccessorVec mySuccessorVec;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new Production using default values.
     */
    Production( std::string name, double prob )
		:
		myName( name ),
		myProbability( prob ),
		myIdentifierVec(),
		mySuccessorVec()
    {
    }

    /**
     * Copy constructor.
     */
    Production( const Production &source )
    {
        // Use assignment operator to do the copy
        *this = source;
    }


    /**
     * Assignment operator.
     */
    Production &operator= ( const Production &source )
    {
        // Copy fields from source class to this class here.
		myName = source.myName;
		myProbability = source.myProbability;
		myIdentifierVec = source.myIdentifierVec;
		mySuccessorVec = source.mySuccessorVec;

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a Production instance.
     */
    virtual ~Production ()
    {
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of Probability of this Production
     */
    double getProbability() const
    {
        return myProbability;
    }

    /**
     * Gets the value of Name of this Production
     */
    std::string getName() const
    {
        return myName;
    }

	/**
	 * Returns a reference to the result list
	 */
	SuccessorVec &getSuccessorVec() {
		return mySuccessorVec;
	}
	/**
	 * Returns a reference to the ident list
	 */
	IdentVec &getIdentVec() {
		return myIdentifierVec;
	}
    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of Probability of this Production
     */
    void setProbability( double probability )
    {
        myProbability = probability ;
    }
	
    /**
     * Sets the value of IdentVec of this Production
     */
    void setIdentVec( IdentVec identVec )
    {
        myIdentifierVec = identVec ;
    }

    /**
     * sets the value of SuccessorVec of this production
     */
    void setSuccessorVec( SuccessorVec resultVec )
    {
        mySuccessorVec = resultVec;
    }

    /**
     * Sets the value of Name of this Production
     */
    void setName( std::string name )
    {
        myName = name ;
    }

}; // End of Production

} // End of LSystem namespace

#endif
