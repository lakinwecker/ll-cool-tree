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
 * File: ObjParser.cpp
 * Purpose: This class is used to parse an OBJ file and stores the results
 *          in specified pointers.
 * Date: February 7, 2004
 * Author: Leonard T. Nooy
 */

#include <cstring>
#include <cmath>
#include <cstdlib>
#include "objparser.h"

OBJParser::OBJParser()
{
	memset(m_token, 0, MAXTOKENLENGTH);
	m_pStream  = 0;

	m_nvert    = 0;
	m_nface    = 0;
	m_ntexc    = 0;
}

OBJParser::~OBJParser()
{
	memset(m_token, 0, MAXTOKENLENGTH);
	m_pStream  = 0;

	m_nvert    = 0;
	m_nface    = 0;
	m_ntexc    = 0;
}

/*
 * Function: OBJParser::readmodel
 * Description: This function is called to load an OBJ mesh into v and f.
 * Inputs: char * filename - The file to load
 *         Model * model - Pointer to a model object to store the information.
 * Outputs: int - 1 = success
 *                0 = failure
 */
int OBJParser::readmodel(char * filename, Model * model)
{
	if(!model)
		return 0;

	m_pStream = fopen(filename, "rb");
	if(!m_pStream)
		return 0;

	// PASS 1: do an inital pass on the file and count the number of vertices and faces
	do {
		token();   // get a new token
		if(!strcmp(m_token, "#")) {			// a comment 
			eatline();
		}	
		else if(!strcmp(m_token, "v")) {	// a vertex
			eatline();
			m_nvert++;
		}
		else if(!strcmp(m_token, "f")) {	// a face
			m_nface += numfacecoord() - 2;	// calculate the number of triangles, not faces
			eatline();
		}
		else if(!strcmp(m_token, "vt")) {	// a tex coord
			eatline();
			m_ntexc++;
		}
		else { // ignore everything else
			eatline();
		}
	} while(!feof(m_pStream));

  	fseek(m_pStream, 0, SEEK_SET); // put the pointer back at the top of the file.

	// create the storage for the mesh
	if(!allocate(&model->vertices, &model->faces)) {
		return 0;
	}
	
	m_nvert = 0;
	m_nface = 0;
	m_ntexc = 0;

	// PASS 2
	do {
		token();
		if(!strcmp(m_token, "#")) {			// eat the comment lines
			eatline();
		}
		else if(!strcmp(m_token, "v")) {	// vertex coords
			readvertex(model->vertices); 
		}
		else if(!strcmp(m_token, "f")) {	// faces
			readface(model->faces);
		}
		else if(!strcmp(m_token, "vt")) {	// tex coords
			readtexcoord(model->vertices);
		}
		else {  // ignore everything else
			eatline();
		}
	} while(!feof(m_pStream));

	// figure out the normals for all of the points
	if(!calculatenormals(model->vertices, model->faces, m_nvert, m_nface))
		return 0;

	model->nverts = m_nvert;
	model->nfaces = m_nface;

	fclose(m_pStream);

	return 1;
}

/*
 * Function: OBJParser::token
 * Purpose: This function reads a whitespace delimited token
 * Inputs: void
 * Outputs: int - 1 = success
 *                0 = failure
 */
int OBJParser::token(void)
{
	memset(m_token, 0, MAXTOKENLENGTH);

	if(m_pStream)
		fscanf(m_pStream, "%s", m_token);

	return 1;
}

/*
 * Function: OBJParser::eatline
 * Purpose: This function will eat characters until it gets to a '\n'
 * Inputs: void
 * Outputs: int - 1 = success
 *                0 = failure
 */
int OBJParser::eatline(void)
{
	char line[512];  // a line of 512 characters is the maximum
  
	memset(line, 0, 512);

	if(m_pStream)
		fscanf(m_pStream, "%[^\n]", line);

	return 1;
}

/*
 * Function: OBJParser::allocate
 * Description: This file allocates local storage for a new mesh
 * Inputs: Vertex ** vertices - Pointer to vertex storage
 *         Triangle ** faces - Pointer to face storage
 * Outputs: int - 1 = success
 *                0 = failure
 */
int OBJParser::allocate(Vertex ** vertices, Triangle ** faces)
{
	if(!vertices || !faces)
		return 0;
 
	(*vertices) = new Vertex[m_nvert];
	if(!(*vertices)) {
		return 0;
	}
	memset((*vertices), 0, sizeof(Vertex) * m_nvert);

	(*faces) = new Triangle[m_nface];
	if(!(*faces)) {
		return 0;
	}	
	memset((*faces), 0, sizeof(Triangle) * m_nface);

	return 1;
}

/*
 * Function: OBJParser::readvertex 
 * Description: When a 'v' token is found, this function is called to properly load a vertex
 * Inputs: Vertex * vertices - Vertex array to add too.
 * Outputs: int - 1 = success;
 *                0 = failure;
 */
int OBJParser::readvertex(Vertex * vertices)
{
	if(!vertices)
		return 0;

	fscanf(m_pStream, "%f", &vertices[m_nvert].x); // x coord
	fscanf(m_pStream, "%f", &vertices[m_nvert].y); // y coord
	fscanf(m_pStream, "%f", &vertices[m_nvert].z); // z coord

	vertices[m_nvert].x /= 10.0;
	vertices[m_nvert].y /= 10.0;
	vertices[m_nvert].z /= 10.0;

	m_nvert++;

	return 1;
}

/*
 * Function: OBJParser::readtexcoord
 * Description: When a 'vt' token is found, this function is called to properly load texture coords
 * Inputs: Vertex * vertices - Vertex array to add too.
 * Outputs: int - 1 = success;
 *                0 = failure;
 */
int OBJParser::readtexcoord(Vertex * vertices)
{
	if(!vertices)
		return 0;

	fscanf(m_pStream, "%f", &vertices[m_ntexc].u);
	fscanf(m_pStream, "%f", &vertices[m_ntexc].v);

	m_ntexc++;
  
	return 1;
}

/*
 * Function: OBJParser::readface
 * Description: When an 'f' token is found, this function is called to properly load a face
 * Inputs: Triangle faces - Triangle array to add too
 * Outputs: int - 1 = success;
 *                0 = failure;
 */
int OBJParser::readface(Triangle * faces)
{
	int i,x;
	char coord[32];
	int indices[64];  // we can store a face of up to 64 indices.

	if(!faces)
		return 0;

	// read all of the indices;
	i = 0;
	while(i < 64) {
		fscanf(m_pStream, "%s", (char*)&coord);		
		indices[i++] = atoi(coord);
		fscanf(m_pStream, "%[ \t]", (char*)&coord);		    // eat any white space
		if(fscanf(m_pStream, "%[\r\n]", (char*)&coord))		// attempt to find the newline or return character
			break;
	}

	// split the resulting polygon into triangles.
	x = 0;
	while(x < (i - 2)) {
		faces[m_nface].a = indices[0] - 1;
		faces[m_nface].b = indices[x + 1] - 1;
		faces[m_nface].c = indices[x + 2] - 1;
		m_nface++;
		x++;
	}

	return 1;
}

// this function figures out how many indices a particular face has
// and is used in calculations in the first pass over the file.
int OBJParser::numfacecoord(void)
{
	int i;
	long fpointer;
	char coord[32];

	fpointer = ftell(m_pStream);
	
	i = 0;
	while(1) {
		fscanf(m_pStream, "%s", (char*)&coord);		
		i++;
		fscanf(m_pStream, "%[ \t]", (char*)&coord);		// eat an white space
		if(fscanf(m_pStream, "%[\r\n]", (char*)&coord))		// attempt to find the newline or return character
			break;
	}

	fseek(m_pStream, fpointer, SEEK_SET); 

	fpointer = ftell(m_pStream);

	return i;
}

/*
 * Function: OBJParser::calculatenormals
 * Description: This function is called to calculate face normals and assign them to all
 *              of the vertices
 * Inputs: Vertex * vertices - The vertex array to work on.
 *         Triangle * faces - The face array to work on.
 *         int nvert - The number of vertices
 *         itn nface - The number of faces
 * Outputs: int - 1 = success
 *                0 = failure
 */
int OBJParser::calculatenormals(Vertex * vertices, Triangle * faces, int nvert, int nface)
{
	int i;
	float length;
	Vertex v1, v2;

	if(!vertices || ! faces)
		return 0;

	i = 0;
	while(i < nface) {
		// create two vectors from the triangles 3 points
		v1.x = vertices[faces[i].a].x - vertices[faces[i].b].x;
		v1.y = vertices[faces[i].a].y - vertices[faces[i].b].y;
		v1.z = vertices[faces[i].a].z - vertices[faces[i].b].z;

		v2.x = vertices[faces[i].c].x - vertices[faces[i].b].x;
		v2.y = vertices[faces[i].c].y - vertices[faces[i].b].y;
		v2.z = vertices[faces[i].c].z - vertices[faces[i].b].z;

		// calculate the cross product.
		v1.nx = v1.y * v2.z - v1.z * v2.y;
		v1.ny = v1.z * v2.x - v1.x * v2.z;
		v1.nz = v1.x * v2.y - v1.y * v2.x;

		// add this normal to all vertices on this face
		vertices[faces[i].a].nx += v1.nx;
		vertices[faces[i].a].ny += v1.ny;
		vertices[faces[i].a].nz += v1.nz;

		vertices[faces[i].b].nx += v1.nx;
		vertices[faces[i].b].ny += v1.ny;
		vertices[faces[i].b].nz += v1.nz;

		vertices[faces[i].c].nx += v1.nx;
		vertices[faces[i].c].ny += v1.ny;
		vertices[faces[i].c].nz += v1.nz;

		i++;
	}

	// now normalize all of the normals.
	i = 0;
	while(i < nvert) {
		length = (float)sqrt((vertices[i].nx * vertices[i].nx) +
						     (vertices[i].ny * vertices[i].ny) +
							 (vertices[i].nz * vertices[i].nz));

		if(length) {
			vertices[i].nx /= -length;
			vertices[i].ny /= -length;
			vertices[i].nz /= -length;
		}

		i++;
	}
	
	return 1;
}

