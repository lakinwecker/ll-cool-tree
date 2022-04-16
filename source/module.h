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

#ifndef MODULE_H
#define MODULE_H

#include <vector>

namespace LSystem {

///-----------------------------------------------------------------------------
/// A Module represents one part of the final sequence for an LSystem.
///
/// @TODO: I think that the module names should be strings, not characters
///  as this would allow L-Systems to be written in a more meaningful way.
///
/// @author Lakin Wecker aka nikal@nucleus.com
///-----------------------------------------------------------------------------
class Module {

//==============================================================================
// Public Members
//==============================================================================
public:
	std::vector<double> parameters;
	char name;

//==============================================================================
// Public Methods
//==============================================================================
public:

	//----------------------------------------------------------------------
	// Constructors

	///---------------------------------------------------------------------
	/// Creates a new Module using default values.
	///---------------------------------------------------------------------
	Module()
	{}

	///---------------------------------------------------------------------
	/// Copy constructor.
	///---------------------------------------------------------------------
	Module( const Module &source )
	{
		// Use assignment operator to do the copy
		*this = source;
	}


	///---------------------------------------------------------------------
	/// Assignment operator.
	///---------------------------------------------------------------------
	Module &operator= ( const Module &source )
	{
		// Copy fields from source class to this class here.
		name = source.name;
		parameters = source.parameters;
		// Return this object with new value
		return *this;
	}

	//----------------------------------------------------------------------
	// Destructor

	///---------------------------------------------------------------------
	/// Deletes a Module instance.
	///---------------------------------------------------------------------
	virtual ~Module ()
	{
	}
}; // End of Module

} // End of LSystem namespace

#endif
