/*
 Copyright (C) 2004 Leonard T. Nooy.
	
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details. 

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */



/*
 * File: renderer.h
 * Purpose: This file contains the class definitions for the L-System
 *          renderer, and data structures
 * Date: February 14, 2004
 * Author: Leonard T. Nooy
 */

#ifndef RENDERER__H
#define RENDERER__H

#include <vector>
#include "module.h"
#include "turtle.h"
#include "objparser.h"

#define NUMBRANCHES  4
#define NUMLEAVES    4

#define ANGLE 22.5

#define RENDERSTATIC  0x0A
#define RENDERDYNAMIC 0x0B

#define FreePointer(r) if((r)) { delete [] (r); (r) = 0; }

class LRenderer
{
	public:
		LRenderer();
		~LRenderer();

		int  loadmodels( void );
		int  setinput( std::vector<LSystem::Module> * s );
		void release( void );
		void reset( void );

		void render( const int & btype, const int & ltype, const int & rtype );

	protected:
				
		void compile( void );
		void do_operator( LSystem::Module * m );
		void do_identifier( LSystem::Module * m, bool branch );
		void do_push( void );
		void do_pop( void );

		void updatebranch( TurtleState * branch, 
						   const TurtleState & parent,
						   const int & btype,
						   const int & ltype );

		// stores the dervation to render.
		std::vector<LSystem::Module> * m_derv;
		LSystem::Module    * m_prevmodule;
		
		Turtle      m_turtle;
		std::vector<TurtleState> m_branches;
		std::vector<TurtleState> m_leaves;

		// stores the models for the branches and leaves
		OBJParser   m_objparser;
		Model     * m_branchobj;
		int         m_nbranch;
		Model     * m_leaveobj;
		int         m_nleave;
};			

#endif

