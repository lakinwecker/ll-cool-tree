//------------------------------------------------------------------------------
// Copyright (C) 2004  Lakin Wecker
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

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

namespace LSystem {

///-----------------------------------------------------------------------------
/// A token represents the unit that the scanner recognizes for the parser.
///
/// A short piece of example code demonstarting how this class it is used,
/// and in what context, is encouraged.
///
/// @author Lakin Wecker aka nikal@nucleus.com
///
/// @see LSystem::Token::Type
///-----------------------------------------------------------------------------
class Token {


//==============================================================================
// Typedefs, enums, etc.
//==============================================================================
public:
	///---------------------------------------------------------------------
	/// The different types of token's possible in an L-System grammer.
	///---------------------------------------------------------------------
	enum Type {
		// Any capital characters from the english alphabet
		// not seperated by whitespace.
		PRODUCTION,
		// Any non-capital characters from the english alphabet
		// not seperated by whitespace.
		IDENTIFIER, 
		// An integer
		INT,
		// A Floating Point Number
		FLOAT,
		// +/-
		EXPRESSION_OP, 
		// *,/
		TERM_OP, 
		// '-' -> unary minus
		FACTOR_OP, 
		// (
		START_PARENS, 
		// )
		END_PARENS, 
		// #-line width,
		// {/,&,\} - rotation: x,y,z
		// {<,>,^} translate: x,y,z
		TRANSFORMATION,
		// EOF
		END_OF_FILE,
		// ;,
		PUNCTUATION,
		// All whitespace
		WHITESPACE
	};//enum TokenType


//==============================================================================
// Private Variables
//==============================================================================
private:

	Token::Type myType;
	std::string myAttribute;

//==============================================================================
// Public Methods
//==============================================================================
public:

	//----------------------------------------------------------------------
	// Constructors

	///---------------------------------------------------------------------
	/// Creates a new Token using default values.
	///---------------------------------------------------------------------
	Token( Token::Type type, std::string attrib )
		:
		myType(type),
		myAttribute(attrib)	
	{
	}

	///---------------------------------------------------------------------
	/// Copy constructor.
	///---------------------------------------------------------------------
	Token( const Token &source )
	{
		// Use assignment operator to do the copy
		*this = source;
	}


	///---------------------------------------------------------------------
	/// Assignment operator.
	///---------------------------------------------------------------------
	Token &operator= ( const Token &source )
	{
		// Copy fields from source class to this class here.
		myType = source.myType;
		myAttribute = source.myAttribute;
		// Return this object with new value
		return *this;
	}


	//----------------------------------------------------------------------
	// Destructor

	///---------------------------------------------------------------------
	/// Deletes a Token instance.
	///---------------------------------------------------------------------
	virtual ~Token ()
	{
	}


	//----------------------------------------------------------------------
	// Getters

	///---------------------------------------------------------------------
	/// Gets the value of Type of this Token
	///---------------------------------------------------------------------
	Token::Type getType() const
	{
		return myType;
	}

	///---------------------------------------------------------------------
	/// Gets the value of Attribute of this Token
	///---------------------------------------------------------------------
	std::string getAttribute() const
	{
		return myAttribute;
	}


	//----------------------------------------------------------------------
	// Setters

	///---------------------------------------------------------------------
	/// Sets the value of Type of this Token
	///---------------------------------------------------------------------
	void setType( Token::Type type )
	{
		myType = type;
	}

	///---------------------------------------------------------------------
	/// Sets the value of Attribute of this Token
	///---------------------------------------------------------------------
	void setAttribute( std::string attribute )
	{
		myAttribute = attribute;
	}
}; // End of Token

}; // End of LSystem namespace

#endif

