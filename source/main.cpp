///-----------------------------------------------------------------------------
/// This file is part of tree.
///
/// tree is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
///
/// tree is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with tree; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
/// Copyright (c) 2004 
/// Lakin Wecker aka nikal@nucleus.com
/// - Based heavily on the example program from the gtkmm programming book.
///-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Project includes.
#include "tree.h"


//-----------------------------------------------------------------------------
// gtkmm includes.
#include <gtkmm/main.h>
#include <gtkglmm.h>


//-----------------------------------------------------------------------------
// std c++ includes.
#include <iostream>



///-----------------------------------------------------------------------------
/// The main implementation for the Tree program.
/// 
/// It simply runs the program ensuring that there is enough memory to be
/// allocated.
///-----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	//----------------------------------------------------------------------
	// Create and make sure that we have an instance of Gtk's Main.
	Gtk::Main *kit = new Gtk::Main(argc, argv);
	if( !kit ) {
		std::cerr << "Couldn't allocate enough memory." << std::endl;
		std::exit(-1);
	}

	Gtk::GL::init(argc, argv);
	//----------------------------------------------------------------------
	// Create and make sure that we have an instance of Tree
	Tree *tree = new Tree();
	if( !tree ) {
		std::cerr << "Couldn't allocate enough memory." << std::endl;
		std::exit(-1);
	}


	//----------------------------------------------------------------------
	// Run the program.
	Gtk::Main::run(*tree);

	//----------------------------------------------------------------------
	// The window has been closed, clean up the memory
	delete tree;
	delete kit;

	//----------------------------------------------------------------------
	// And exit.
	return 0;
}


