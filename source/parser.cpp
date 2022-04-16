/************************************************************************
 * Copyright (C) Sun Feb 15 2004  nikal
 *
 * parser.cpp: Started at Sun Feb 15 2004 - 16:47:42
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 **************************************************************************/

#include "parser.h"
#include <sstream>

using namespace LSystem;


#ifdef SCANNER_ERROR
#undef SCANNER_ERROR
#endif
#define SCANNER_ERROR(x) myScanner.createError(x, __FILE__,__LINE__)
/**
 * Forward Declaration
 */
void seedrand( );

/**
 * Operator for token type
 */
std::ostream &operator<<( std::ostream &out, Token::Type t ) {
	switch( t ) {
		case Token::PRODUCTION:
			out << "PRODUCTION";
			return out;
		case Token::IDENTIFIER:
			out << "IDENTIFIER";
			return out;
		case Token::INT:
			out << "INT";
			return out;
		case Token::FLOAT:
			out << "FLOAT";
			return out;
		case Token::EXPRESSION_OP:
			out << "EXPRESSION_OP";
			return out;
		case Token::TERM_OP:
			out << "TERM_OP";
			return out;
		case Token::FACTOR_OP:
			out << "FACTOR_OP";
			return out;
		case Token::START_PARENS:
			out << "START_PARENS";
			return out;
		case Token::END_PARENS:
			out << "END_PARENS";
			return out;
		case Token::TRANSFORMATION:
			out << "TRANSFORMATION";
			return out;
		case Token::END_OF_FILE:
			out << "END_OF_FILE";
			return out;
		case Token::PUNCTUATION:
			out << "PUNCTUATION";
			return out;
		case Token::WHITESPACE:
			out << "WHITESPACE";
			return out;
	}
	return out;
}

ModuleVec Parser::evaluateSystem() {
	//Start the random number generator
	seedrand();
	//Temp Work Vectors
	ModuleVec work1Vector = myStartList;
	ModuleVec work2Vector;

	//References
	ModuleVec *currentVector = &work1Vector;
	ModuleVec *tempVector = &work2Vector;
	ModuleVec *newVector = &work2Vector;
	//Do this as many times as required.
	for( int j = 0; j < myIterations; ++j ) {
		//Go through each of the modules in the current working
		//module vector replacing them with their productions.
		for( ModuleVec::size_type i = 0; i < currentVector->size(); ++i ) {
			ModuleVec tmp = myProductionSet.evaluate( (*currentVector)[i], myGlobals );
			newVector->insert( newVector->end(), tmp.begin(), tmp.end() );
		}
		currentVector->clear();

		//Swap References
		tempVector = newVector;
		newVector = currentVector;
		currentVector = tempVector;
	}
	///////////////////////////////////////////////////////////////////////////////////////
	// Print out the new list of modules
	LDEBUG( 
	for( ModuleVec::size_type i = 0; i < currentVector->size(); ++i ) {
		std::cout << (*currentVector)[i].name;
		std::cout << "(";
		for( NumberVec::size_type j = 0; j < (*currentVector)[i].parameters.size(); ++j ) {
			std::cout << (*currentVector)[i].parameters[j] << ",";
		}
		std::cout << ")";
	} )
	return *currentVector;
}


//L-System => StartState Production { Production } EndOfFile
void Parser::parseLSystem() {
	////////////////////////////////////
	//Start anew
	myProductionSet.clear();
	myStartList.clear();
	myIterations = 0;

	////////////////////////////////////
	// Parse the initial states.
	if( !parseStartState() ) {
		throw SCANNER_ERROR("Lush requires there to be a start state.");
	} 
	if( !parseProduction() ) {
		throw SCANNER_ERROR("Requires at least one production.");
	}
	
	//////////////////////////////////////////////////////////////
	// Parse the rest of the productions until there are no more. 
	// We don't catch any errors here, parseProduction will return 
	// false if there is just no production.
	while(  parseProduction() ) {  }

	/////////////////////////////////////////////////////////////
	// Now we should be at the end of the l-system
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::END_OF_FILE ) {
		std::string a = tok->getAttribute();
		delete tok;
		throw SCANNER_ERROR( "Error: Expected end of file, got: " + a );
	}
	delete tok;
	
	////////////////////////////////////////////////////////////
	// Normalize the probabilities
	myProductionSet.normalize();
}

//StartState => { (Globals | ModelMaps) } Iterations ';' StartModules ';'
bool Parser::parseStartState() {
	LDEBUG( std::cout << "In start state"; )
	Token *tok;
	while( !parseIterations() ) {
		tok = myScanner.lex();	
		if( tok->getType() == Token::IDENTIFIER ) {
			std::string globalIdent = tok->getAttribute();
			delete tok;
			tok = myScanner.lex();
			if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ":" ) {
				delete tok;
				throw SCANNER_ERROR("Invalid global assignment. Looks like you forgot the ':'.");
			}
			delete tok;
			Expression *e = Expression::parseExpression( myScanner );
			myGlobals[ globalIdent ] = e->evaluateExpression( myGlobals );
			delete e;
			tok = myScanner.lex();
			if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ";" ) {
				delete tok;
				throw SCANNER_ERROR("Invalid global assigment. Looks like you forgot a ';' somewhere.");
			}
			delete tok;	
		} else if( tok->getType() == Token::PRODUCTION ) {
			char model = tok->getAttribute()[0];
			delete tok;
			tok = myScanner.lex();
			if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ":" ) {
				delete tok;
				throw SCANNER_ERROR("Invalid model assignment. Looks like you forgot the ':'.");
			}
			delete tok;
			tok = myScanner.lex();
			if( tok->getType() != Token::INT ) {
				delete tok;
				throw SCANNER_ERROR("Invalid model assignment. You need an INTEGER after the ':'.");
			}
			int value = atoi( tok->getAttribute().c_str() );
			delete tok;
			myModels[ model ] = value;
			tok = myScanner.lex();
			if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ";" ) {
				delete tok;
				throw SCANNER_ERROR("Invalid model assignment.  Looks like you forgot a ';' somwhere.");
			}
			delete tok;
		} else {
			throw SCANNER_ERROR("An lsystem needs to start with: 'iterations: <INT>;'");
		}
	}
	tok = myScanner.lex();
	if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ";" ) {
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: Expected a ';' but got '" + a + "' instead.");
	}
	delete tok;
	if( !parseStartModules() ) {
		throw SCANNER_ERROR("An lsystem needs to have a list of start modules following the iterations line.");
	}

	tok = myScanner.lex();
	if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ";" ) {
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: Expected a ';' but got '" + a + "' instead.");
	}
	LDEBUG( std::cout << "Finished start state"; )
	delete tok;

	return true;
}

//Iterations => 'iterations:' Integer
bool Parser::parseIterations() {
	
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::IDENTIFIER || tok->getAttribute() != "iterations" ) {
		/////////////////////////////////////////////////////////////////////////////
		// Not an interations line.
		myScanner.unlex( tok );
		return false;
	}
	delete tok;

	tok = myScanner.lex();

	if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ":" ) {
		////////////////////////////////////////////////////////////////////////////
		// Invalid Iterations line.
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: Expected ':' after 'iterations' but got '" + a + "' instead.");
	}
	delete tok;
	tok = myScanner.lex();

	if( tok->getType() != Token::INT ) {
		////////////////////////////////////////////////////////////////////////////
		// Invalid Iterations line.
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: Expected an integer after 'iterations:' but got '" + a + "' instead.");
	}
	myIterations = atoi( tok->getAttribute().c_str() );
	
	LDEBUG( std::cout << "Iterations read as: " << tok->getAttribute(); )
	LDEBUG( std::cout << "Iterations set to: " << myIterations; )
	
	delete tok;
	return true;
}

//StartRule => Module { Module } 
bool Parser::parseStartModules() {
	Token *tok = myScanner.lex();
	std::string resultName = tok->getAttribute();
	if( tok->getType() != Token::PRODUCTION  && 
		///////////////////////////////////////////////////////////////////////////
		// Not a start rule.
		tok->getType() != Token::TRANSFORMATION ) {
		myScanner.unlex( tok );
		return false;
	}
	delete tok;

	//////////////////////////////////////////////////////////////////////////////
	// Parse the ExpressionList.
	ExpressionPtrVec myEV;
	parseExpressionList( myEV );
	Module mod;

	mod.name = resultName[0];
	for( ExpressionPtrVec::size_type i = 0; i < myEV.size(); ++i) {
		double d = myEV[i]->evaluateExpression( myGlobals );
		delete myEV[i];
		mod.parameters.push_back(d);
	}
	myStartList.push_back( mod );
	myEV.clear();
	

	/////////////////////////////////////////////////////////////////////////////
	// TODO: Does this really need to be a while?  (I don't think so.)
	while( parseStartModules() ) { }

	return true;
}


//NumberList => '(' Number { ',' Number } ')'
bool Parser::parseNumberList( NumberVec &numberList ) {
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::START_PARENS ) {
		////////////////////////////////////////////////////////////////////////
		// Not a Number List
		myScanner.unlex( tok );
		return false;
	}
	delete tok;
	tok = myScanner.lex();

	/////////////////////////////////////////////////////////////////////////////
	// We got a start parens, let's look for all the number seperated by commas
	// that we can find.  booya.  End it on an end parenthesis.
	while( tok->getType() == Token::FLOAT || tok->getType() == Token::INT  ) { 
		numberList.push_back( tok->getAttribute() );
		delete tok;
		tok = myScanner.lex();
		if( tok->getType() == Token::PUNCTUATION && tok->getAttribute() == "," ) {
			delete tok;
			tok = myScanner.lex();
			continue;
		} else if( tok->getType() == Token::END_PARENS ) {
			delete tok;
			break;
		} else {
			//////////////////////////////////////////////////////////////////////
			// Error within a number list.
			std::string a = tok->getAttribute();
			myScanner.unlex( tok );
			throw SCANNER_ERROR("Error: expected either a number or an end parens, got: '"+a+"' instead.");
		}
	}
	return true;
}

//ExpressionList => '(' Expression { ',' Expression } ')'
bool Parser::parseExpressionList( ExpressionPtrVec &exprList ) {
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::START_PARENS ) {
		/////////////////////////////////////////////////////////////////////////
		// Not an Expression List
		myScanner.unlex( tok );
		return false;
	}
	delete tok;
	
	/////////////////////////////////////////////////////////////////////////////
	// Must have at least one Expression.
	Expression *expression = Expression::parseExpression( myScanner );
	LDEBUG( std::cout << "Expression: " << expression; )
	exprList.push_back( expression );
	tok = myScanner.lex();
	
	/////////////////////////////////////////////////////////////////////////////
	// Let's find all the expressions that we can find followed by commas.  booya
	while( tok->getType() == Token::PUNCTUATION && tok->getAttribute() == "," ) {
		delete tok;
		expression = Expression::parseExpression( myScanner );
		exprList.push_back( expression );	
		tok = myScanner.lex();
	}
	
	/////////////////////////////////////////////////////////////////////////////
	// End it with an end parens.
	if( tok->getType() != Token::END_PARENS ) {
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: Expected a ')' but got: '"+a+"' instead.");
	}
	delete tok;

	return true;
}

//Production => Predecessor ':' Float '=>' Successor { Successor } ';'
bool Parser::parseProduction() {
	std::string rulename;
	IdentVec identList;

	if( !parsePredecessor( rulename, identList ) ) {
		////////////////////////////////////////////////////////////////////////
		// Not a production.
		return false;
	}
	
	Token *tok = myScanner.lex();
	std::string fl = "1.0";
	////////////////////////////////////////////////////////////////////////////
	// Grab the probability if it's there.
	if( tok->getType() == Token::PUNCTUATION && tok->getAttribute() == ":" ) {
		delete tok;
		tok = myScanner.lex();
		if( tok->getType() != Token::FLOAT ) {
			std::string a = tok->getAttribute();
			myScanner.unlex( tok );
			throw SCANNER_ERROR( "Error: did not include a probability with rule: "+rulename+".");
		}
		fl = tok->getAttribute();
		delete tok;
		tok = myScanner.lex();
	}  
	///////////////////////////////////////////////////////////////////////////
	// Match the '=>' token.
	if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != "=>" ) {
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: rule: "+rulename+" missing '=>' got '"+a+"'.");
	} 
	delete tok;
	
	SuccessorVec successorList;
	if( !parseSuccessorList( successorList ) ) {
		throw SCANNER_ERROR( "A production must have successor's.");
	}

	LDEBUG( std::cout << successorList.size(); )

	Production prod( rulename, atof(fl.c_str()) );
	prod.setSuccessorVec( successorList );
	prod.setIdentVec( identList );

	tok = myScanner.lex();
	if( tok->getType() != Token::PUNCTUATION || tok->getAttribute() != ";" ) {
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR("Error: expecting a ';' got '"+a+"' instead.");
	}
	std::ostringstream out;
	out << rulename << identList.size();
	myProductionSet.push_back( prod );
	LDEBUG( std::cout << "Adding rule[" << rulename << "] with probability[" << fl << "]"; )
	delete tok;	
	return true;
}

//Predecessor => Module IdentifierList
bool Parser::parsePredecessor( std::string &name, IdentVec &identList ) {
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::PRODUCTION ) {
		//////////////////////////////////////////////////////////////////////////////
		// Not a Predecessor.
		myScanner.unlex( tok );
		return false;
	}
	name = tok->getAttribute();
	delete tok;
	
	parseIdentifierList( identList );
	return true;
}

//SuccessorList => Successor { Successor } 
bool Parser::parseSuccessorList( SuccessorVec &successorList ) {
	ExpressionPtrVec expressionList;
	std::string successorName;
	while ( parseSuccessor( successorName, expressionList) ) {	

		Successor res( successorName, expressionList );
		successorList.push_back( res );
		successorName = "";
		expressionList.clear();

		LDEBUG( std::cout << "Adding successor[" << successorName << "]"; );
	}
	return true;
}

//Successor => ModuleName ExpressionList
bool Parser::parseSuccessor( std::string &name, ExpressionPtrVec &expressionList ) {
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::PRODUCTION ) {
		/////////////////////////////////////////////////////////////////////////////////////
		// Not a Successor.
		myScanner.unlex( tok );
		return false;
	}

	name = tok->getAttribute();
	delete tok;

	parseExpressionList( expressionList );
	
	return true;
}

//IdentifierList => '(' Identifier { ',' Identifier } ')'
bool Parser::parseIdentifierList( IdentVec &identList ) {
	Token *tok = myScanner.lex();
	if( tok->getType() != Token::START_PARENS ) {
		/////////////////////////////////////////////////////////////////////
		// Not an IdentifierList.
		myScanner.unlex(tok);
		return false;
	}
	delete tok;
	tok = myScanner.lex();
	if( tok->getType() != Token::IDENTIFIER ) {
		std::string err = "Error: Expected an identifier for the identifier list got '"+tok->getAttribute()+"'.";
		myScanner.unlex(tok);
		throw SCANNER_ERROR( err );
	}
	identList.push_back( tok->getAttribute() );
	delete tok;
	tok = myScanner.lex();
	while( tok->getType() == Token::PUNCTUATION && tok->getAttribute() == "," ) {
		delete tok;
		tok = myScanner.lex();
		if( tok->getType() != Token::IDENTIFIER ) {
			std::string err = "Error: Expected an identifier for the identifier list got '"+tok->getAttribute()+"'.";
			myScanner.unlex(tok);
			throw SCANNER_ERROR( err );
		}

		identList.push_back( tok->getAttribute() );	
		delete tok;
		tok = myScanner.lex();
	}
	if( tok->getType() != Token::END_PARENS ) {
		std::string a = tok->getAttribute();
		myScanner.unlex( tok );
		throw SCANNER_ERROR( "Error: Expected a ')' but got: '"+a+"' instead.");
	}
	delete tok;
	return true;
}
