/*
 Copyright (C) 2003  Leonard T. Nooy.
	
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
 * File: texmap.h
 * Purpose: This file contains the class defintion for a texture map
 * Date: February 7, 2003
 * Author: Leonard T. Nooy
 */

#ifndef TEXMAP__H
#define TEXMAP__H

#include <stdio.h>

/* This is the Header for TGA files
 * Note: There appears to be two different TGA header 
 *       definitions, one includes a padding byte after 
 *       imagetype, and the other does not. 
 *       I am using the one that does not.
 */
typedef struct __h__
{
	unsigned char identsize;
	unsigned char colormaptype;
	unsigned char imagetype;
  
	short colormapstart;
	short colormaplength;
 
	short xstart;
	short ystart;
	short width;
	short height;
	unsigned char bits;
	unsigned char descriptor;

} TGAHEADER;

/*
 * A very simple texture mapping class. To render a 
 * model with a specific texture map, load it from a
 * file and then call bindmap.
 */
class TexMap
{
	public:
		TexMap();
		~TexMap();

		int loadfile(char * filename);
		int bindmap(void);
		int release(void);

	protected:
		TGAHEADER       m_header;
		unsigned char * m_pixeldata;
		int             m_alpha;
		unsigned int    m_id;
};

#endif
