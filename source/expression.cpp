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

#include "expressionnode.h"
#include "expression.h"

namespace LSystem {
//------------------------------------------------------------------------------
Expression::Expression( const Expression &source ) {
	myHead = clone( source.myHead );
}


//-----------------------------------------------------------------------------
Expression &Expression::operator= ( const Expression &source ) {
	if( myHead ) {
		delete myHead;
	}
	myHead = clone( source.myHead );
	return *this;
}


//-----------------------------------------------------------------------------
Expression::~Expression ()
{
	if( myHead ) delete myHead;
}


//-----------------------------------------------------------------------------
Expression *Expression::parseExpression( Scanner &scan ) {
	return new Expression( Expression::parseExpressionReal( scan ) );
}


//-----------------------------------------------------------------------------
double Expression::evaluateExpression(
	std::map<std::string, double> &lookupTable )
{
	assert( myHead );
	return myHead->evaluateNode( lookupTable );
}


//-----------------------------------------------------------------------------
std::ostream &operator<<( std::ostream &out, Expression *expr ) {
	return expr->printTree( out, expr->myHead );
}


//-----------------------------------------------------------------------------
Expression::Expression(ExpressionNode *head)
	:
	myHead( head )
{
}


//-----------------------------------------------------------------------------
std::ostream &Expression::printTree(
	std::ostream &out,
	const ExpressionNode * const expr )
{
	if( !expr ) return out;
	printTree( out, expr->getLeftChild() );
	printTree( out, expr->getRightChild() );
	const Token * const t = expr->getToken();
	out << t->getAttribute();
	return out;
}


//-----------------------------------------------------------------------------
ExpressionNode *Expression::parseExpressionReal(
	Scanner &scan )
{
	ExpressionNode *ret = NULL;
	ret = parseTerm( scan );
	Token *tok = scan.lex();

	while( ret && // Make sure we found a term to begin with
			tok->getType() == Token::PRODUCTION &&
			(tok->getAttribute() == "-" || 
			tok->getAttribute() == "+") ) {
		ExpressionNode *next;
		next = parseTerm( scan );
		if( !next ) { 
			delete ret;
			LSYSTEM_ERROR(
				"Error: a " + tok->getAttribute()
				+ " requires that a expression follow it." );
		}
		ret = new ExpressionNode( tok, ret, next );
		tok = scan.lex();
	}
	scan.unlex( tok );
	return ret;
}


//------------------------------------------------------------------------------
ExpressionNode *Expression::parseTerm( Scanner &scan ) {
	ExpressionNode *ret = NULL;
	ret = parseFactor( scan );
	Token *tok = scan.lex();

	while( ret &&  // Make sure we have a factor to begin with
			tok->getType() == Token::PRODUCTION &&
			(tok->getAttribute() == "/" ||
			 tok->getAttribute() == "*") ) {
		ExpressionNode *next;
		next = parseTerm( scan );
		if( !next ) { 
			delete ret;
			LSYSTEM_ERROR(
				"Error: a " + tok->getAttribute()
				+ " requires that a term follow it." );
		}
		ret = new ExpressionNode( tok, ret, next );
		tok = scan.lex();
	}
	scan.unlex( tok );
	return ret;
}


//------------------------------------------------------------------------------
ExpressionNode *Expression::parseFactor( Scanner &scan ) {
	Token *tok = scan.lex();
	// '(' Expression ')'
	if( tok->getType() == Token::START_PARENS ) {
		ExpressionNode *ret = NULL;
		ret = parseExpressionReal( scan );
		if( !ret ) {
			std::string attr = tok->getAttribute();
			delete tok;
			LSYSTEM_ERROR("Error: Found: '"+attr+"' expected follow up expression.");
		}
		delete tok;
		tok = scan.lex();
		if( tok->getType() != Token::END_PARENS ) {
			std::string err = "Error: Found: '" + tok->getAttribute() + "' but expected a ')'.";
			delete tok;
			LSYSTEM_ERROR(err);
		}
		delete tok;
		return ret;
	// '-' Factor
	} else if( tok->getType() == Token::PRODUCTION && tok->getAttribute() == "-" ) {
		ExpressionNode *ret = NULL;
		ret = parseFactor( scan );
		if( !ret ) {
			std::string err = "Error: Expecting a factor after a unary minus.";
			delete tok;
			LSYSTEM_ERROR(err);
		}
		return new ExpressionNode( tok, ret, NULL );
	// Number
	} else if( tok->getType() == Token::FLOAT || tok->getType() == Token::INT ) {
		return new ExpressionNode( tok, NULL, NULL );
	// Identifier
	} else if( tok->getType() == Token::IDENTIFIER ) {
		return new ExpressionNode( tok, NULL, NULL );
	}
	//Not a factor!!!
	scan.unlex( tok );
	return NULL;
}


//------------------------------------------------------------------------------
ExpressionNode *Expression::clone( const ExpressionNode * const head ) {
	//Base case
	if( !head ) { return NULL; }
	//Recursive Step
	const Token *const t = head->getToken();
	return new ExpressionNode( 
		// Clone the token.
		new Token( t->getType(), t->getAttribute() ),
		//Clone the left child.
		clone( head->getLeftChild() ),
		//Clone the right child.
		clone( head->getRightChild() )
	);
}

};//end namespace LSystem

