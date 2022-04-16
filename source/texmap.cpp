/*
 Copyright (C) 2003  Leonard T. Nooy
	
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
 * File: texmap.cpp
 * Purpose: This class loads tga file to be used as texture maps in the OpenGL environment
 * Date: October 11, 2003
 * Author: Leonard T. Nooy
 */

#include "texmap.h"
#include <GL/gl.h>
#include <GL/glu.h>
TexMap::TexMap()
{
	m_pixeldata = 0;
	m_alpha     = 0;

	m_id        = 0;
}

TexMap::~TexMap(){}

/*
 * Function: TexMap::loadfile
 * Purpose: The loadfile function is used, as its name indicates, to load a texture map file
 * Inputs: char * filename - The filename to load
 * Outputs: int - 1 = success
 *                0 = failure
 */
int TexMap::loadfile(char * filename)
{
	// load the tga file.
	FILE * file;
	file = fopen(filename, "rb");
	if(!file)
		return 0;

	// read the header from the file
	fread(&m_header, sizeof(TGAHEADER), 1, file);
  
	// Check for the right bit depth
	if(m_header.bits == 24) {
		m_alpha = 0;
	}
	else if(m_header.bits == 32) {
		m_alpha = 1;
	}
	else
		return 0;

	// allocate enough room for the pixel color data
	m_pixeldata = new unsigned char[m_header.width * m_header.height * m_header.bits];
	if(!m_pixeldata)
		return 0;

	// read the pixel color data from the file
	fread(m_pixeldata, sizeof(unsigned char), m_header.width * m_header.height * m_header.bits, file);
	fclose(file);

	// Create a new texture ID
	glGenTextures(1, &m_id);

	// Bind the Texture Pixels into Video Memory
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 
				 0, (m_alpha)?4:3,			// with or without alpha channel
				 m_header.width, 
				 m_header.height, 0, 
				 (m_alpha)?GL_BGRA_EXT:GL_BGR_EXT,   // with or without alpha channel
				 GL_UNSIGNED_BYTE, 
				 m_pixeldata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

	return 1;
}

/*
 * Function: TexMap::bindmap
 * Description: This function is called to switch between texture maps
 * Inputs: void
 * Outputs: int - 1 = success
 *                0 = failure
 */
int TexMap::bindmap(void)
{
	int cur_id;

	glGetIntegerv( GL_TEXTURE_BINDING_2D, &cur_id );
	if( (unsigned int)cur_id != m_id ) {
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
	return 1;
}

/*
 * This function cleans up after the loadfile function
 */
int TexMap::release(void)
{
	// delete the pixel data
	if(m_pixeldata) {
		delete [] m_pixeldata;
		m_pixeldata = 0;
	}

	// clean up the texture name, unless it is zero
	// I know that OpenGL should be able to handle deleting the zero 
	// texture, but some implementations don't handle it gracefully...
	if( m_id ) {
		glDeleteTextures(1, &m_id);
		m_alpha  = 0;
		m_id     = 0;
	}

	return 1;
}
