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
 * File: vector3d.cpp
 * Purpose: This file contains the overloaded operators
 *          used to do vector math
 * Date: February 28, 2004
 * Author: Leonard T. Nooy
 */

#include <math.h>
#include "vector3d.h"

// Scalar Multiplication
Vector3D operator * ( const Vector3D & v, const float & f )
{
	Vector3D a;

	a.x = v.x * f;
	a.y = v.y * f;
	a.z = v.z * f;

	return a;
}

// Scalar Addition
Vector3D operator + ( const Vector3D & v, const float & f )
{
	Vector3D a;

	a.x = v.x + f;
	a.y = v.y + f;
	a.z = v.z + f;

	return a;
}

// Vector Cross Product
Vector3D operator * ( const Vector3D & v, const Vector3D & w )
{
	Vector3D a;

	a.x = v.y * w.z - v.z * w.y;		// cross product
	a.y = v.z * w.x - v.x * w.z;
	a.z = v.x * w.y - v.y * w.x;

	return a;
}

// Vector Addition
Vector3D operator + ( const Vector3D & v, const Vector3D & w )
{
	Vector3D a;

	a.x = v.x + w.x;
	a.y = v.y + w.y;
	a.z = v.z + w.z;

	return a;
}

// Scalar Addition
Vector3D & operator += ( Vector3D & v, const float & f )
{
	v.x += f;
	v.y += f;
	v.z += f;

	return v;
}

// Scalar Multiplication
Vector3D & operator *= ( Vector3D & v, const float & f )
{
	v.x *= f;
	v.y *= f;
	v.z *= f;

	return v;
}

// Vector Addition
Vector3D & operator += ( Vector3D & v, const Vector3D & w )
{
	v.x += w.x;
	v.y += w.y;
	v.z += w.z;

	return v;
}

// Vector Cross Product
Vector3D & operator *= ( Vector3D & v, const Vector3D & w )
{
	Vector3D a;

	a = v * w;
	v = a;
	
	return v;
}

// Vector Dot Product
float operator & ( const Vector3D & v, const Vector3D & w )
{
	return (( v.x * w.x ) + ( v.y * w.y ) + ( v.z * w.z ));
}

// sqrt( v.v )
float Length( const Vector3D & v )
{
	return (float)sqrt( v & v );
}

