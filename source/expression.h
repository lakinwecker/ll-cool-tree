//------------------------------------------------------------------------------
// Copyright (C) 2003  Lakin Wecker
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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "scanner.h"

#include <iostream>
#include <sstream>
#include <map>


namespace LSystem {

//------------------------------------------------------------------------------
// Predeclaration of expression node,
class ExpressionNode;

///-----------------------------------------------------------------------------
/// An expression tree.
///
/// An expression tree.  Used to store an scan while parsing 
/// the lsystems.  These scans can be evaluated later.
///
/// LSystem::Expression *e = LSystem::parseExpression( scanner );
///
/// @author Lakin Wecker aka nikal@nucleus.com
///
/// @see LSystem::Scanner
/// @see LSystem::Token
///
///-----------------------------------------------------------------------------

class Expression
{
//==============================================================================
// Private Variables
//==============================================================================
private:

	ExpressionNode * myHead;

//==============================================================================
// Public Methods
//==============================================================================
public:
	///---------------------------------------------------------------------
	/// Copy constructor.
	///---------------------------------------------------------------------
	Expression( const Expression &source );


	///---------------------------------------------------------------------
	/// Assignment operator.
	///---------------------------------------------------------------------
	Expression &operator= ( const Expression &source );


	//----------------------------------------------------------------------
	// Destructor

	///---------------------------------------------------------------------
	/// Deletes a Expression instance.
	///---------------------------------------------------------------------
	virtual ~Expression ();


	//----------------------------------------------------------------------
	// Public API

	///---------------------------------------------------------------------
	/// Public method of creating expression trees.
	///---------------------------------------------------------------------
	static Expression *parseExpression( Scanner &scan );


	///---------------------------------------------------------------------
	/// Recursively evaluate this node. Using the lookup table if needed.
	///
	/// @param lookupTable A reference to a table used to find values of
	///  identifiers.
	/// @return the value that this scan evaluates to.
	///---------------------------------------------------------------------
	double evaluateExpression(
		std::map<std::string, double> &lookupTable );


	///---------------------------------------------------------------------
	/// Overloaded << operator for std::ostreams. Postfix Notation.
	///
	/// @param out Reference to the std::ostream to print the tree to.
	/// @param expr Pointer to the expression to print.
	/// @returns a reference to the std::ostream used.
	///---------------------------------------------------------------------
	friend std::ostream &operator<<( std::ostream &out, Expression *expr );

	//======================================================================
	// Private constructors.
	//======================================================================
	private:


	///---------------------------------------------------------------------
	/// Creates a new Expression using default values.
	///---------------------------------------------------------------------
	Expression(ExpressionNode *head);


	//======================================================================
	// Private methods
	//======================================================================
	private:


	///---------------------------------------------------------------------
	/// Recursively print out an expression tree in Postfix notation.
	/// @param out The reference to the std::ostream to use.
	/// @param expr The pointer to the ExpressionNode to print out.
	///---------------------------------------------------------------------
	std::ostream &printTree(
		std::ostream &out,
		const ExpressionNode * const expr );


	///---------------------------------------------------------------------
	/// Recursively parses an expression.
	/// @param scan The Scanner used to lex the incoming tokens.
	/// @return a new Expression Node pertaining to the expression.
	///---------------------------------------------------------------------
	static ExpressionNode *parseExpressionReal(
		Scanner &scan );


	///---------------------------------------------------------------------
	/// Recursively parse a term.
	/// @param scan The Scanner used to lex the incoming tokens.
	/// @return a new ExpressionNode pointer for the term.
	///---------------------------------------------------------------------
	static ExpressionNode *parseTerm( Scanner &scan );


	///---------------------------------------------------------------------
	/// Recursively parse a Factor.
	/// @param scan The Scanne rused to lext the incoming tokens.
	/// @return a new ExpressionNode pointer for the factor.
	///---------------------------------------------------------------------
	static ExpressionNode *parseFactor( Scanner &scan );


	///---------------------------------------------------------------------
	/// Used by the copy constructor and assignment operator to clone
	/// Expression Trees. It does this recursively for each Node.
	/// 
	/// @param head a constant pointer to the head of the tree.
	/// @return an ExpressionNode pointer to a tree with cloned contents of 
	/// the original tree.
	///---------------------------------------------------------------------
	ExpressionNode *clone( const ExpressionNode * const head );


//==============================================================================
// Disabled constructors and operators
//==============================================================================
private:

}; // End of Expression

} // End of LSystem namespace

#endif
