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
 * File: turtle.cpp
 * Purpose: This file contains the turtle code used to render an L-System
 * Date: February 14, 2004
 * Author: Leonard T. Nooy
 */

#include "turtle.h"

#include <cstring>

Turtle::Turtle()
{
	m_curpos = STACKSIZE - 1;
	
    std::memset( m_bstack, 0, sizeof(TurtleState *) * STACKSIZE );
	m_branch = &m_root;
}

Turtle::~Turtle() { }

void Turtle::release( void )
{
	// release the internal turtlestates
	m_state.release();
	m_root.release();

	// release the stack
	for(int i = 0; i < STACKSIZE; i++ ) {
		m_sstack[i].release();
	}

	// reset all of the variables
	m_curpos = STACKSIZE - 1;

    std::memset( m_bstack, 0, sizeof(TurtleState *) * STACKSIZE );
	m_branch = &m_root;
}

TurtleState * Turtle::Root( void )
{
	return &m_root;
}

/*
 * Function: Turtle::Push
 * Purpose: This function pushes the current state of the turtle onto a stack
 * Inputs: void
 * Outputs: void
 */
void Turtle::Push(void)
{
	if( m_curpos > 0 ) {
		m_sstack[ --m_curpos ] = m_state;
		m_bstack[ m_curpos ]   = m_branch;
	}
	else {
		return;		// stack overflow
	}
}

/*
 * Function: Turtle::Pop
 * Purpose: This function pops the state off from the top of the stack
 * Inputs: void
 * Outputs: void
 */
void Turtle::Pop(void)
{
	if( m_curpos < STACKSIZE - 1 ) {
		m_state  = m_sstack[ m_curpos ];
		m_branch = m_bstack[ m_curpos++ ];
	}
	else {
		m_state  = m_sstack[0];  // stack underflow, set to identity
		m_branch = m_bstack[0];
		return;
	}
}

/*
 * Function: Turtle::Move
 * Purpose: This function moves the turtle forward and changes the current
 *          state to reflect this movement
 * Inputs: float length - The length of the branch
 * Outputs: TurtleState - The state of the turtle before the move
 */
TurtleState Turtle::Move( float length )
{
	TurtleState * new_branch = 0;
	TurtleState rv;
	Vector3D heading;
	float mat[16];

	// obtain the state of the turtle before we do anything to it
	rv = m_state;

	// link the branches together
	m_state.m_length = length;
	new_branch = new TurtleState;
	if( new_branch ) {
		(*new_branch) = m_state;
		(*new_branch).m_cur_quat = (*new_branch).m_quat;
	}
	
	if( m_branch ) {
		m_branch->AddChild( new_branch );
		new_branch->SetParent( m_branch );
		m_branch = new_branch;
	}

	// apply the current rotation to the heading vector
	m_state.m_quat.toMatrix( mat );

	// This operation is equivalent to multiplying the rotation matrix by
	// [0,1,0] (The heading vector), and then moving the turtle along the
	// new heading vector "length" units
	heading.x = mat[4]; 
	heading.y = mat[5];
	heading.z = mat[6];
	heading *= length;
	m_state.m_p += heading;

	rv.m_length = length;
	return rv;
}

/*
 * Function: Turtle::RotateH
 * Purpose: This function rotates the turtle around it's heading vector (local y axis)
 * Inputs: float angle - The angle to rotate by.
 * Outputs: void
 */
void Turtle::RotateH( float angle )
{
	Quaternion q;

	q.setEulerY( DEG2RAD * angle );
	m_state.m_quat = m_state.m_quat * q;   // this will rotate around the turtles local y axis
}

/*
 * Function: Turtle::RotateL
 * Purpose: This function rotates the turtle around it's Left vector (local x axis)
 * Inputs: float angle - The angle to rotate by.
 * Outputs: void
 */
void Turtle::RotateL( float angle )
{
	Quaternion q;

	q.setEulerX( DEG2RAD * angle );
	m_state.m_quat = m_state.m_quat * q;   // this will rotate around the turtles local x axis
}

/*
 * Function: Turtle::RotateU
 * Purpose: This function rotates the turtle around it's up vector (local z axis)
 * Inputs: float angle - The angle to rotate by.
 * Outputs: void
 */
void Turtle::RotateU( float angle )
{
	Quaternion q;

	q.setEulerZ( DEG2RAD * angle );
	m_state.m_quat = m_state.m_quat * q;   // this will rotate around the turtles local z axis
}

/*
 * Function: Turtle::Width
 * Purpose: The Width function is used to increase, decrease, or set absolutely
 *          the width of the branch.
 * Inputs: float width - The width of the branch
 *         bool inc - Flag to indicate if width is an incremental value
 *                    or an absolute value
 * Ouputs: void
 */
void Turtle::Width( float width, bool inc )
{
	if(!inc) {
		m_state.m_width = width;
	}
	else {
		m_state.m_width += width;
	}
}
