//------------------------------------------------------------------------------
// Copyright (C) 2004  nikal
//
// scanner.h: Started at Sun Feb 15 2004 - 16:45:24
//
// tree is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//------------------------------------------------------------------------------

#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "error.h"

#include <string>
#include <iostream>

namespace LSystem {
	
///-----------------------------------------------------------------------------
/// A basic scanner implementation that allows you to lex and unlex a character
/// at a time, it also keeps track of the current line number and column number.
///
/// @see LSystem::Token
/// @see LSystem::Error
///-----------------------------------------------------------------------------
class Scanner {

//==============================================================================
// Public Methods
//==============================================================================
public:
	//----------------------------------------------------------------------
	// Constructors
	

	///---------------------------------------------------------------------
	/// Standard constructor.
	///	
	/// @param in The input stream to read characters from.
	///---------------------------------------------------------------------
	Scanner(std::istream &in)
		: 
		myInputStream(in),
		myCurrentToken(NULL),
		myCurrentLine(1),
		myCurrentCol(1)
	{
	}

	///---------------------------------------------------------------------
	/// Standard destructor
	///---------------------------------------------------------------------
	~Scanner() 
	{
		if( myCurrentToken ) {
			delete myCurrentToken;
		}
	}


	///---------------------------------------------------------------------
	/// Grabs the next recognizable Token from the string.
	/// The calling code is responsible for deleting the
	/// token when it's done with it.
	/// Throws an error if it find an unrecognized symbol.
	///---------------------------------------------------------------------
	Token *lex ();


	///---------------------------------------------------------------------
	/// Puts the token back onto the front of the list, the next
	/// call to lex will return a pointer to this token.
	/// the Scanner class will assume responsibility for any token
	/// unlexed this way and will delete it with it's dtor.
	/// The only error for this function is if we already 
	/// have a token that's been unlexed.  The function will throw
	/// on this occasion.
	/// 
	///
	/// @param tok The token to restore to the front of the list.
	///---------------------------------------------------------------------
	void unlex (Token *tok);

	///---------------------------------------------------------------------
	/// Returns a pointer to an error object which can be used
	/// to detail the error state of the scanner. This details
	/// the line number and column number in the input. Optional
	/// Arguments are an Error message explaining the error and
	/// the line number and filename of the code which found the 
	/// error.
	///---------------------------------------------------------------------
	Error *createError(std::string msg, std::string file , int line  );

	//----------------------------------------------------------------------
	// Getters


	///---------------------------------------------------------------------
	/// Returns the current line.
	/// 
	/// @return an Int representing the line number.
	///---------------------------------------------------------------------
	int getCurrentLine() const {
		return myCurrentLine;
	}


//==============================================================================
// Private Variables
//==============================================================================
private:

	std::istream &myInputStream;
	Token *myCurrentToken;
	int myCurrentLine;
	int myCurrentCol;

//==============================================================================
// Disabled constructors and operators
//==============================================================================


	///---------------------------------------------------------------------
	/// Disabled copy constructor
	///---------------------------------------------------------------------
	Scanner( const Scanner &source );


	///---------------------------------------------------------------------
	/// Disabled Assignment Operator
	///---------------------------------------------------------------------
	Scanner &operator= ( const Scanner &source );

};//class Scanner
};//namespace LSystem
#endif //SCANNER_H

