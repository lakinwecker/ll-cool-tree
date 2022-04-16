/*
 Copyright (C) 2003 Leonard T. Nooy
	
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
 * File: quaternion.cpp
 * Purpose: This file contains the quaternion class which is
 *          used by the viewer to provide gimble lock free rotation.
 * Date: October 13, 2003
 * Author: Leonard T. Nooy
 */

#ifndef QUATERNION__H
#define QUATERNION__H

// This is not a complete quaternion class, it only contains the basic
// functions needed by this program
class Quaternion
{
	public:
		Quaternion();
		Quaternion(float w, float x, float y, float z);
		~Quaternion();

		Quaternion operator * ( const Quaternion& ) const;  // quaternion multiplication

		void setRotationAxis( float theta, float x, float y, float z );
		void setEulerAngles( float x, float y, float z );	// Euler -> Quaternion
		void setEulerX( float x );							// Individual axes -> Quaternion
		void setEulerY( float y );
		void setEulerZ( float z );
		
		void toMatrix( float m[16] );						// Quaternion -> Matrix
		void toHeading( float v[3] );						// Quaternion * [0,1,0]
		
		void identity(void);								// sets the quaternion to the identity
		void inverse(void);									// sets the quaternion to it's inverse

	protected:
		float m_w, m_x, m_y, m_z;
};

#endif

