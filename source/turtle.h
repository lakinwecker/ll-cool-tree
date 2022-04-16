/*
 Copyright (C) 2004 Leonard T. Nooy
	
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
 * File: turtle.h
 * Purpose: This file contains the class definition for the turtle
 *          class.
 * Date: February 19, 2004
 * Author: Leonard T. Nooy
 */

#ifndef TURTLE__H
#define TURTLE__H

#include <vector>
#include "vector3d.h"
#include "objparser.h"
#include "quaternion.h"

#define DEG2RAD 0.017453292		// PI/180
#define STACKSIZE 64

class TurtleState;
class TurtleState
{
	public:
		TurtleState();
		~TurtleState();

		void AddChild( TurtleState * ts );
		void SetParent( TurtleState * p );
		void ConvertLocal( void );

		void release( void );

	public:
		Quaternion m_quat;		// the rotation at rest
		Quaternion m_cur_quat;  // the current rotation
		
		Vector3D   m_p;			// the position
		float	   m_width;		
		float      m_length;	

		TurtleState ** m_children;
		int			   m_num_children;

		TurtleState * m_parent;
};


class Turtle
{
	public:
		Turtle();
		~Turtle();

		TurtleState * Root( void );
		
		TurtleState Move( float length );
		void Push( void );
		void Pop( void );
		void RotateH( float angle );
		void RotateL( float angle );
		void RotateU( float angle );
		void Width( float angle, bool inc );

		void release( void );

	protected:
		TurtleState m_state;		// the turtles current state
		
		TurtleState m_root;			// a concrete root to ground the tree
		TurtleState * m_branch;		// the trees current branch

		TurtleState m_sstack[STACKSIZE];	// an internal stack of states, and branches
		TurtleState * m_bstack[STACKSIZE];  // a stack of TurtleState pointers representing the
											// last created branch.
		int m_curpos;
};

#endif
