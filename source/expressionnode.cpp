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

#include "token.h"
#include "expressionnode.h"
namespace LSystem {


//------------------------------------------------------------------------------
ExpressionNode::ExpressionNode( Token *tok, ExpressionNode *l, ExpressionNode * r )
	:
	myToken( tok ),
	myLeftChild( l ),
	myRightChild( r )
{
}



//------------------------------------------------------------------------------
ExpressionNode::~ExpressionNode ()
{
	if( myToken ) delete myToken;
	if( myLeftChild ) delete myLeftChild;
	if( myRightChild ) delete myRightChild;
}


//------------------------------------------------------------------------------
const Token *const ExpressionNode::getToken() const
{
	return myToken;
}


//------------------------------------------------------------------------------
const ExpressionNode *const ExpressionNode::getRightChild() const
{
	return myRightChild;
}


//------------------------------------------------------------------------------
const ExpressionNode *const ExpressionNode::getLeftChild() const
{
	return myLeftChild;
}


//------------------------------------------------------------------------------
void ExpressionNode::setToken( Token *token )
{
	if( myToken ) delete myToken;
	myToken = token;
}


//------------------------------------------------------------------------------
void ExpressionNode::setRightChild( ExpressionNode * rightChild )
{
	if( myRightChild ) delete myRightChild;
	myRightChild = rightChild;
}


//------------------------------------------------------------------------------
void ExpressionNode::setLeftChild( ExpressionNode * leftChild )
{
	if( myLeftChild ) delete myLeftChild;
	myLeftChild = leftChild;
}


//------------------------------------------------------------------------------
double ExpressionNode::evaluateNode(
	std::map<std::string, double> &lookupTable ) {
	LDEBUG( std::cout << "Begin evaluateNode"; )
	//Base Case: FLOAT
	if( myToken->getType() == Token::FLOAT ) {
		LDEBUG( std::cout << "Float: " << myToken->getAttribute(); )
		return (double)atof( myToken->getAttribute().c_str() );
	//Base Case: INT
	} else if( myToken->getType() == Token::INT ) {
		std::string f = myToken->getAttribute() + ".0";
		LDEBUG( std::cout << "Integer: " << f; )
		return (double)atof( f.c_str() );
	//Base Case: IDENTIFIER
	} else if( myToken->getType() == Token::IDENTIFIER ) { 
		double d = lookupTable[ myToken->getAttribute() ];
		LDEBUG( std::cout << "Identifier: " << myToken->getAttribute() << "=" << d; )
		return d;
	//Non Base cases.
	} else if( myToken->getType() == Token::PRODUCTION &&
				(myToken->getAttribute() == "+" ||
				myToken->getAttribute() == "-") ) {
		assert( myLeftChild );
		if( myToken->getAttribute() == "+" ) {
			assert( myRightChild );
			LDEBUG( std::cout << "Adding..."; )
			return myLeftChild->evaluateNode( lookupTable ) + 
					myRightChild->evaluateNode( lookupTable );
		} else if( myToken->getAttribute() == "-" ) {
			if( myRightChild ) {
				LDEBUG( std::cout << "Subtracting.."; )
				return myLeftChild->evaluateNode( lookupTable ) -
						myRightChild->evaluateNode( lookupTable );
			} else {
				LDEBUG( std::cout << "unary minus..."; )
				return -myLeftChild->evaluateNode( lookupTable );
			}
		}
	} else if( myToken->getType() == Token::PRODUCTION &&
				(myToken->getAttribute() == "/" ||
				 myToken->getAttribute() == "*") ) {
		assert( myLeftChild );
		assert( myRightChild );
		double l = myLeftChild->evaluateNode( lookupTable );
		double r = myRightChild->evaluateNode( lookupTable );
		LDEBUG( std::cout << l << myToken->getAttribute() << r; )
		if( myToken->getAttribute() == "*" ) {
			return l*r;
		} else if( myToken->getAttribute() == "/" ) {
			if( r == 0.0 ) LSYSTEM_ERROR("Error: Division by zero\n");
			return l/r;
		}
	}
	LDEBUG( std::cout << "End evaluateNode"; )

	//--------------------------------------------------------------
	//  Should never get here, warn us if we do!
	assert(0);
	return 0;
}

};//end namespace LSystem
