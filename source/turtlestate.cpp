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
 * File: turtlestate.cpp
 * Purpose: This file contains the code for the 
 *	        TurtleState class used by the Turtle class
 * Date: February 26, 2004
 * Author: Leonard T. Nooy
 */

#include "turtle.h"

#include <cstring>

TurtleState::TurtleState()
{
	m_quat.identity();
    std::memset( &m_p, 0, sizeof(Vector3D) );
	m_width  = 1.0;
	m_length = 1.0;
	
	m_children     = 0;
	m_num_children = 0;
	m_parent       = 0;
}

TurtleState::~TurtleState() { }

/*
 * Function: TurtleState::AddChild
 * Purpose: This function adds a pointer to the children array that
 *          points to the given TurtleState
 * Inputs: TurtleState * ts - A pointer to the TurtleState to add
 * Outputs: void
 */
void TurtleState::AddChild( TurtleState * ts )
{
	TurtleState ** temp = 0;

	if( !ts )		// make sure the child is valid
		return;

	// allocate a new chunk of memory to hold the old children plus the new one
	temp = new TurtleState *[ m_num_children + 1 ];
	if(!temp) {
		return;
	}
    std::memset( temp, 0, sizeof(TurtleState *) );

	if( m_num_children ) {
		// copy the old children to the new memory
		memcpy( temp, m_children, sizeof(TurtleState *) * m_num_children );
		delete [] m_children;
	}
	
	// add the new child
	m_children = temp;
	m_children[ m_num_children ++ ] = ts;
}

/*
 * Function: TurtleState::ConvertLocal
 * Purpose: This function is important, because it converts the 
 *          absolute orientations given by the Turtle class into
 *          relative orientations with respect to each TurtleStates
 *			parent. This is needed by the LRenderer.
 * Inputs: void
 * Outputs: void
 */
void TurtleState::ConvertLocal( void )
{
	Quaternion quat_inv;
	int i;

	// for each of the children to this TurtleState, ask them 
	// to convert their children to local orientation before
	// we convert it.
	quat_inv = m_quat;
	quat_inv.inverse();

	for( i = 0; i < m_num_children; i++ ) {
		m_children[i]->ConvertLocal();

		// multiply the childs orientation by the inverse of the parents orientation
		m_children[i]->m_quat = quat_inv * m_children[i]->m_quat;		
		m_children[i]->m_cur_quat = m_children[i]->m_quat;
	}
}

/* simply sets the parent if p != 0 */
void TurtleState::SetParent( TurtleState * p )
{
	if( p ) {
		m_parent = p;
	}
}

void TurtleState::release( void )
{
	// loop through and call release for each of the children first
	int i;

	m_quat.identity();
    std::memset( &m_p, 0, sizeof(Vector3D) );
	m_width  = 1.0;
	m_length = 1.0;
	m_parent = 0;
	
	// release any memory associated with the children
	for( i = 0; i < m_num_children; i++ ) {
		m_children[i]->release();
		delete m_children[i];
	}

	if( m_children ) {
		delete [] m_children;
		m_children     = 0;
	}
	m_num_children = 0;
}

