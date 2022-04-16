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
 * File: Quaternion.cpp
 * Purpose: This class allows for gimble lock free rotation
 * Date: October 13, 2003
 * Author: Leonard T. Nooy
 */

#include <math.h>
#include "quaternion.h"

Quaternion::Quaternion()
{
	m_x = m_y = m_z = 0.0;		// unit quaternion
	m_w = 1.0;	
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

Quaternion::~Quaternion(){}

void Quaternion::identity(void)
{
	m_x = m_y = m_z = 0.0;
	m_w = 1.0;
}

void Quaternion::inverse( void )
{
	m_x = -m_x;
	m_y = -m_y;
	m_z = -m_z;
}

/*
 * Function: Quaternion::operator *
 * Purpose: This function overloads the multiplication operator
 *          for quaternions.
 * Inputs: const Quaternion & q - A pointer to the second quaternion to multiply by
 * Outputs: Quaternion - The product of (this*q);
 *
 * Note: Quaternion multiplication = qq´ = [ww´ - v · v´, v x v´ + wv´ + w´v]
 */
Quaternion Quaternion::operator * ( const Quaternion& q) const
{
	Quaternion a;

	a.m_w = (m_w * q.m_w) - (m_x * q.m_x + m_y * q.m_y + m_z * q.m_z);

	// cross product 'this' X q
	a.m_x = m_y * q.m_z - m_z * q.m_y;
	a.m_y = m_z * q.m_x - m_x * q.m_z;
	a.m_z = m_x * q.m_y - m_y * q.m_x;

	// add this.w * q
	a.m_x += m_w * q.m_x;
	a.m_y += m_w * q.m_y;
	a.m_z += m_w * q.m_z;

	// add q.w * this
	a.m_x += q.m_w * m_x;
	a.m_y += q.m_w * m_y;
	a.m_z += q.m_w * m_z;

	return a;
}

/*
 * Function: Quaternion::setRotationAxis
 * Purpose: This function is used to construct a Quaternion from a given
 *          axis of rotation and an angle.
 * Inputs: float theta - The amount to rotate about
 *         float x,y,z - Specify the axis of rotation
 * Outputs: void
 */
void Quaternion::setRotationAxis( float theta, float x, float y, float z )
{
	// rotate about an arbitrary axis.
	float length;

	// begin by normalizing the given axis of rotation
	length = (float)sqrt(x*x + y*y + z*z);
	if( length ) {
		x /= length;
		y /= length;
		z /= length;
	}

	// now set the quaternion scalers
	m_w = (float)cos( theta / 2.0 );

	m_x = x * (float)sin( theta / 2.0 );
	m_y = y * (float)sin( theta / 2.0 );
	m_z = z * (float)sin( theta / 2.0 );
}

/*
 * Function: Quaternion::setEulerAngles
 * Purpose: This function creates a quaternion based on angles around the 3 Euler axes
 * Inputs: float x - Rotation around the x axis
 *         float y - Rotation around the y axis
 *         float z - Rotation around the z axis
 * Outputs: void
 */
void Quaternion::setEulerAngles(float x, float y, float z)
{
	Quaternion Qx((float)cos(x/2.0), (float)sin(x/2.0), 0.0, 0.0);	// create a unit quaternion for each axis rotation
	Quaternion Qy((float)cos(y/2.0), 0.0, (float)sin(y/2.0), 0.0);
	Quaternion Qz((float)cos(z/2.0), 0.0, 0.0, (float)sin(z/2.0));
	Quaternion Qr;

	Qr = Qz * Qy * Qx;	// multiply them all together to get a composite rotation

	m_w = Qr.m_w;
	m_x = Qr.m_x;
	m_y = Qr.m_y;
	m_z = Qr.m_z;

	return;
}

/*
 * These functions are faster if only one axis must be set
 */
void Quaternion::setEulerX( float x )
{
	m_w = (float)cos(x / 2.0);
	m_x = (float)sin(x / 2.0);
	m_y = m_z = 0.0;
}

void Quaternion::setEulerY( float y )
{
	m_w = (float)cos(y / 2.0);
	m_y = (float)sin(y / 2.0);
	m_x = m_z = 0.0;
}

void Quaternion::setEulerZ( float z )
{
	m_w = (float)cos(z / 2.0);
	m_z = (float)sin(z / 2.0);
	m_x = m_y = 0.0;
}

/*
 * Function: Quaternion::toMatrix
 * Purpose: This function is used to convert a quaternion into a 4x4 matrix
 *          that can be applied to the OpenGL matrix stack
 * Inputs: float m[16] - Pointer to an array of floats to store the matrix
 * Outputs: void
 */
void Quaternion::toMatrix(float m[16])
{
	float y2, x2, z2, xx, yy, zz, xy, wz, xz, wy, yz, wx;
	
	x2 = m_x * 2.0f;
	y2 = m_y * 2.0f;
	z2 = m_z * 2.0f;

	yy = m_y * y2;	
	xx = m_x * x2;
	zz = m_z * z2;
	xy = m_x * y2;
	wz = m_w * z2;
	xz = m_x * z2;
	wy = m_w * y2;
	yz = m_y * z2;
	wx = m_w * x2;

	m[0] = 1.0f - (yy + zz);
	m[1] = xy + wz;
	m[2] = xz - wy;
	m[3] = 0.0;

	m[4] = xy - wz;
	m[5] = 1.0f - (xx + zz);
	m[6] = yz + wx;
	m[7] = 0.0;

	m[8] = xz + wy;
	m[9] = yz - wx;
	m[10] = 1.0f - xx - yy;
	m[11] = 0.0;

	m[12] = 0.0;
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
}

