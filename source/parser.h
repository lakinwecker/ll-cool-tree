//------------------------------------------------------------------------------
// Copyright (C) 2004  Lakin Wecker
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//------------------------------------------------------------------------------


#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "successor.h"
#include "production.h"
#include "productionset.h"
#include "module.h"

#include <vector>
#include <map>


namespace LSystem {


//==============================================================================
// Typedefs
//==============================================================================
typedef std::vector<Production> ProductionVec;
typedef std::vector<std::string> NumberVec;
typedef std::map<char, int> ModelMap;

///-----------------------------------------------------------------------------
/// A class to parse an LSystem from an iostream using a recursive descent algo.
///
/// @author Lakin Wecker aka nikal@nucleus.com
///
/// @see LSystem::Scanner
/// @see LSystem::Production
/// @see LSystem::Successor
/// @see LSystem::Expression
/// @see LSystem::ExpressionNode
/// @see LSystem::Token
///-----------------------------------------------------------------------------
class Parser {


//==============================================================================
// Private Variables
//==============================================================================
private:

	LSystem::Scanner myScanner;
	int myIterations;
	ProductionSet myProductionSet;
	ModuleVec myStartList;
	SymbolTable myGlobals;
	ModelMap myModels;
	

//==============================================================================
// Public Methods
//==============================================================================
public:

	//----------------------------------------------------------------------
	// Constructors

	///---------------------------------------------------------------------
	/// Creates a new Parser using default values.
	///---------------------------------------------------------------------
	Parser( std::istream &in )
		:
		myScanner( in ),
		myIterations( 0 ),
		myProductionSet(),
		myStartList(),
		myGlobals(),
		myModels()
	{
	}



	//----------------------------------------------------------------------
	// Destructor

	///---------------------------------------------------------------------
	/// Deletes a Parser instance.
	///---------------------------------------------------------------------
	virtual ~Parser ()
	{
	}

	//----------------------------------------------------------------------
	// Public API

	///---------------------------------------------------------------------
	/// Parses the LSystem.
	/// Throws pointers to Error's on errors.
	///---------------------------------------------------------------------
	void parseLSystem();


	///---------------------------------------------------------------------
	/// Evaluate the system
	///---------------------------------------------------------------------
	ModuleVec evaluateSystem();


	///---------------------------------------------------------------------
	/// Model Lookup
	///---------------------------------------------------------------------
	int operator[]( char c ) {
		return myModels[c];
	}


	///---------------------------------------------------------------------
	/// Model Lookup
	///---------------------------------------------------------------------
	int getModel( char c ) {
		return myModels[c];
	}


//==============================================================================
// Private Methods
//=================================================================;
private:
	//----------------------------------------------------------------------
	// Internally used Parsing functions.
	//----------------------------------------------------------------------


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseStartState();


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseIterations();


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseStartModules();


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseProduction();


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parsePredecessor( std::string &name, IdentVec &identList );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseSuccessorList( SuccessorVec &successorList );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseSuccessor( std::string &name, ExpressionPtrVec &expressionList );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseNumberList( NumberVec &numberList );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseExpressionList( ExpressionPtrVec &expressionList );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool parseIdentifierList( IdentVec &identifierList );



//==============================================================================
// Disabled constructors and operators
//==============================================================================
private:
	///---------------------------------------------------------------------
	/// Disabled Copy constructor.
	///---------------------------------------------------------------------
	Parser( const Parser &source );


	///---------------------------------------------------------------------
	/// Disabled Assignment operator.
	///---------------------------------------------------------------------
	Parser &operator= ( const Parser &source );


}; // End of Parser

} // End of LSystem namespace

#endif
