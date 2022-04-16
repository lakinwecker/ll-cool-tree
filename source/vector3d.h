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
 * File: vector3d.h
 * Purpose: This file is contains stucture and operator definitions
 *          for a 3D Vector type
 * Date: February 28, 2004
 * Author: Leonard T. Nooy
 */

#ifndef __VECTOR_3D__
#define __VECTOR_3D__

// Used by the turtle to track it's position
typedef struct __VECTOR3D__
{
	float x, y, z;
} Vector3D;


// a bunch of operators
Vector3D operator * ( const Vector3D & v, const float & f );
Vector3D operator + ( const Vector3D & v, const float & f );
Vector3D operator * ( const Vector3D & v, const Vector3D & w );
Vector3D operator + ( const Vector3D & v, const Vector3D & w );
Vector3D & operator += ( Vector3D & v, const float & f );
Vector3D & operator *= ( Vector3D & v, const float & f );
Vector3D & operator += ( Vector3D & v, const Vector3D & w );
Vector3D & operator *= ( Vector3D & v, const Vector3D & w );

float operator & ( const Vector3D & v, const Vector3D & w );
float Length( const Vector3D & v );


#endif

