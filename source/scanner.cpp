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
#include "scanner.h"

#include <cctype>
#include <cassert>


using namespace LSystem;

Token *Scanner::lex () {

	//----------------------------------------------------------------------
	// Return the last token unlexed.
	if( myCurrentToken ) {
		Token *tok = myCurrentToken;
		myCurrentToken = NULL;
		return tok;
	}


	//----------------------------------------------------------------------
	//Grab the next character.
	int curChar = myInputStream.get();


	//----------------------------------------------------------------------
	//Skip whitespace
	while( isspace( curChar ) ) {
		if( curChar == (int)'\n' ) {
			++myCurrentLine;
			myCurrentCol = 1;
		}
		curChar = myInputStream.get();
	}


	//----------------------------------------------------------------------
	// Recognize tokens
	if( curChar == EOF ) {
		return new Token( Token::END_OF_FILE, "EOF" );
	} else if( isdigit( curChar ) ) {
		std::string theFloat( 1, (char)curChar ) ;
		curChar = myInputStream.get();
		while( isdigit( curChar ) ) {
			theFloat += curChar;
			curChar = myInputStream.get();
		}
		if( curChar != (int)'.' ) {
			myInputStream.putback((char)curChar);
			return new Token( Token::INT, theFloat );
		}
		theFloat += curChar;
		curChar = myInputStream.get();
		while( isdigit( curChar ) ) {
			theFloat += curChar;
			curChar = myInputStream.get();
		}
		myInputStream.putback((char)curChar);
		return new Token( Token::FLOAT, theFloat );
	} else if( curChar == (int)'(' ) {
		return new Token( Token::START_PARENS, "(" );
	} else if( curChar == (int)')' ) {
		return new Token( Token::END_PARENS, ")" );
	} else if( curChar == (int)'+' || 
			   curChar == (int)'-' ||
			   curChar == (int)'&' ||
			   curChar == (int)'\\' ||
			   curChar == (int)'/' ||
			   curChar == (int)'*' ||
			   curChar == (int)'^' ||
			   curChar == (int)'|' ||
			   curChar == (int)'{' ||
			   curChar == (int)'}' ||
			   curChar == (int)'!' ||
			   curChar == (int)'[' ||
			   curChar == (int)']' ||
			   isupper( curChar ) ) {
		std::string production ( 1, (char)curChar );
		return new Token( Token::PRODUCTION, production );
	} else if( curChar == (int)';' ||
			   curChar == (int)':' ||
			   curChar == (int)'\''||
			   curChar == (int)',' ) {
		return new Token( Token::PUNCTUATION, std::string( 1, (char)curChar ) );
	} else if( curChar == (int)'=' ) {
		char next = myInputStream.get();
		if( next != (int)'>') {
			throw createError("Error: '=' expects a '>' directly afterwards!\n",__FILE__,__LINE__);
		}
		return new Token( Token::PUNCTUATION, "=>" );
	} else if( islower( curChar ) ) {
		std::string ident( 1, (char)curChar );
		curChar = myInputStream.get();
		while( islower( curChar ) ) {
			ident += curChar;
			curChar = myInputStream.get();
		}
		myInputStream.putback((char)curChar);
		return new Token( Token::IDENTIFIER, ident );
	} else if( curChar == (int)'\n' ) {
		++myCurrentLine;
		myCurrentCol = 1;
	}
	throw createError("Invalid token: " + (char)curChar, __FILE__,__LINE__);
	return NULL;
}


void Scanner::unlex (Token *tok) {
	//Can't unlex a null token.
	assert( tok ); 

	//Can't unlex two tokens until the first
	//token is consumed.
	if( myCurrentToken ) {
		throw createError("Can only unlex one token at a time.", __FILE__, __LINE__);
	}
	myCurrentToken = tok;
}

Error *Scanner::createError( std::string msg, std::string file, int line ) {
	return new Error(msg, myCurrentLine, myCurrentCol, file, line);
}

