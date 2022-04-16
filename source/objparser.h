/*
 * File: ObjParser.h
 * Purpose: This file contains the class definition for the OBJ parser class
 * Date: February 7, 2003
 * Author: Leonard T. Nooy
 */

#ifndef OBJPARSER__H
#define OBJPARSER__H

#include <stdio.h>
#include "texmap.h"

#define MAXTOKENLENGTH 128

// simple vertex structure
typedef struct __V__
{
	float u, v;
	float nx, ny, nz;
	float x, y, z;
} Vertex;

// simple triangle structure
typedef struct __Tr__
{
	int a,b,c;
} Triangle;

// encapsulates a single model
typedef struct __O__
{
	Vertex   * vertices;
	Triangle * faces;
	TexMap   * map;
	int nverts;
	int nfaces;
} Model;

// This class will load a properly formatted OBJ file.
// It's behaviour is undefined for corrupted OBJ files.
class OBJParser
{
	public:
		OBJParser();
		~OBJParser();
  
		int readmodel(char * filename, Model * model);

	private:
		int token(void);		// simple scanner functions
		int eatline(void);
		int eatwhitespace(void);

		int readvertex(Vertex * verts);		// loads a vertex
		int readtexcoord(Vertex * verts);	// loads a texture coordinate
		int readface(Triangle * faces);		// loads a face
		int numfacecoord(void);				// calculates how many triangles are needed to represent a polygon

		int calculatenormals(Vertex * vertices, Triangle * faces, int nvert, int nface);
		int allocate(Vertex ** vertices, Triangle ** faces);

		FILE * m_pStream;
		char   m_token[MAXTOKENLENGTH];

		int    m_nvert;
		int    m_nface;
		int    m_ntexc;
};


#endif
