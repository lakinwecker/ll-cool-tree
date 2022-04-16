//------------------------------------------------------------------------------
// Copyright (C) 2003  Lakin Wecker
//
// tree is free software; you can redistribute it and/or modify
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


#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H

#include "error.h"

#include <map>
#include <string>
#include <cassert>


#ifdef LSYSTEM_ERROR
#undef LSYSTEM_ERROR
#endif
#define LSYSTEM_ERROR(x) throw new Error(x, -1,-1,__FILE__,__LINE__)

namespace LSystem {

//------------------------------------------------------------------------------
// Predeclaration of Token
class Token;

///-----------------------------------------------------------------------------
/// One node in the expression tree.
/// 
/// This is a node in an expression tre which is built by an Expression
/// to represent itself.  This probably shouldn't be used outside of an 
/// Expression. Currently allows for an identifier, number, or an operator. 
/// 
/// @author Lakin Wecker aka nikal@nucleus.com
///
/// @see Token::Type
///-----------------------------------------------------------------------------
class ExpressionNode {


//==============================================================================
// Private Variables
//==============================================================================
private:

	Token *myToken;
	ExpressionNode *myLeftChild;
	ExpressionNode *myRightChild;

//==============================================================================
// Public Methods
//==============================================================================
public:

	//----------------------------------------------------------------------
	// Constructors

	///---------------------------------------------------------------------
	/// Creates a new ExpressionNode using default values.
	///---------------------------------------------------------------------
	ExpressionNode( Token *tok, ExpressionNode *l, ExpressionNode * r );


	//----------------------------------------------------------------------
	// Destructor

	///---------------------------------------------------------------------
	/// Deletes a ExpressionNode instance.
	///---------------------------------------------------------------------
	virtual ~ExpressionNode ();


	//----------------------------------------------------------------------
	// Getters

	// Example of a getter method:

	///---------------------------------------------------------------------
	/// Gets the value of Value of this ExpressionNode.
	///
	/// @return A const pointer to this node's Token.
	///---------------------------------------------------------------------
	const Token *const getToken() const;


	///---------------------------------------------------------------------
	/// Gets the value of RightChild of this ExpressionNode.
	/// 
	/// @return a const pointer to the right child.
	///---------------------------------------------------------------------
	const ExpressionNode *const getRightChild() const;


	///---------------------------------------------------------------------
	/// Gets the value of LeftChild of this ExpressionNode.
	/// 
	/// @return a const pointer to the left child.
	///---------------------------------------------------------------------
	const ExpressionNode *const getLeftChild() const;


	//----------------------------------------------------------------------
	// Setters

	///---------------------------------------------------------------------
	/// Sets the value of Value of this ExpressionNode.
	/// Note: If the node already has a token, this will
	/// delete the current token before taking on the
	/// new one.
	///
	/// @param token Pointer to the token which is to become
	/// the "value" of this node. The token represents 
	/// the node and it's actions.
	///---------------------------------------------------------------------
	void setToken( Token *token );


	///---------------------------------------------------------------------
	/// Sets the value of RightChild of this ExpressionNode
	/// Note: If the node already has a right child, this will
	/// delete the current right child before taking on
	/// the new one.
	/// 
	/// @param rightChild Pointer to another ExpressionNode which is to become
	/// the right child of this node. This node will be responsible
	/// for the deletion of the child.  If the child is deleted before
	/// this node is deleted, behavior is undefined.
	///---------------------------------------------------------------------
	void setRightChild( ExpressionNode * rightChild );


	///---------------------------------------------------------------------
	/// Sets the value of LeftChild of this ExpressionNode.
	/// Note: if the node already has a left child, this will
	/// delete the current left child befor taking on 
	/// the new one.
	/// 
	/// @param leftChild Pointer to another ExpressionNode which is to become
	/// the left child of this node. This node will be responsible
	/// for the deletion of this child.  If the child is deleted before
	/// this node is deleted, behavior is undefined.
	///---------------------------------------------------------------------
	void setLeftChild( ExpressionNode * leftChild );


	//----------------------------------------------------------------------
	// Evaluation methods

	///---------------------------------------------------------------------
	/// Recursively evaluate this node. Using the lookup table if needed.
	///	
	/// @param lookupTable A reference to a table used to find values of
	/// identifiers.
	///  @return the value that this node and it's children evalute to.
	///---------------------------------------------------------------------
	double evaluateNode(
		std::map<std::string, double> &lookupTable );

//==============================================================================
// Disabled constructors and operators
//==============================================================================
private:
	///---------------------------------------------------------------------
	/// Disabled Copy constructor.
	///---------------------------------------------------------------------
	ExpressionNode( const ExpressionNode &source );


	///---------------------------------------------------------------------
	/// Disabled Assignment operator.
	///---------------------------------------------------------------------
	ExpressionNode &operator= ( const ExpressionNode &source );


}; // End of ExpressionNode

}; // End of LSystem namespace

#endif
