/*
 Copyright (C) 2004  Leonard T. Nooy
	
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
 * File: renderer.cpp
 * Purpose: This class is used to interpret a L-System dervation and 
 *          produce a visualization.
 * Date: February 14, 2004
 * Author: Leonard T. Nooy
 */


#include <cmath>
#include <cstring>
#include <GL/gl.h>
#include <GL/glu.h>
#include "renderer.h"


LRenderer::LRenderer()
{
	m_derv      = 0;
	m_branchobj = 0;
	m_nbranch   = 0;
	m_leaveobj  = 0;
	m_nleave    = 0;
}

LRenderer::~LRenderer(){}

/*
 * Function: LRenderer::setinput
 * Purpose: This function sets the dervation that the renderer should draw
 * Inputs: std::list<LSystem::Module> * s  - A generated module string
 * Outputs: int - 1 = success
 *                0 = failure
 */
int LRenderer::setinput( std::vector<LSystem::Module> * s )
{
	if(!s)
	  return 0;

	m_derv = s;
	
	compile();
	return 1;
}

/*
 * Function: LRenderer::loadmodels
 * Purpose: This function loads the static branch and leaf models used to
 *          render the trees in 3D
 * Inputs: void
 * Outputs: int - 1 = success
 *                0 = failure
 */
int LRenderer::loadmodels( void )
{
	int i;
	char filename[128];

	// Branches
	m_branchobj = new Model[NUMBRANCHES];
	if(!m_branchobj)
		return 0;
    std::memset( m_branchobj, 0, sizeof(Model) );

	i = 0;
	while( i < NUMBRANCHES ) {
		sprintf( filename, "branch%d.obj", i+1);
		if(!m_objparser.readmodel(filename , &m_branchobj[i])) {
			FreePointer( m_branchobj );
			return 0;
		}
		
		m_branchobj[i].map = new TexMap[1];
		if(!m_branchobj[i].map)
			return 0;
		sprintf( filename, "bark%d.tga", i+1);
		if(!m_branchobj[i].map->loadfile( filename )) {
			FreePointer( m_branchobj[i].map );
			return 0;
		}
		i++;
	}
	
	// Leaves
	m_leaveobj = new Model[NUMLEAVES];
	if(!m_leaveobj)
		return 0;
    std::memset( m_leaveobj, 0, sizeof(Model) );
	
	i = 0;
	while( i < NUMLEAVES ) {
		sprintf( filename, "leaf%d.obj", i+1);
		if(!m_objparser.readmodel( filename, &m_leaveobj[i])) {
			FreePointer( m_leaveobj );
			return 0;
		}
		
		m_leaveobj[i].map = new TexMap[1];
		if(!m_leaveobj[i].map)
			return 0;
		sprintf( filename, "leave%d.tga", i+1);
		if(!m_leaveobj[i].map->loadfile( filename )) {
			FreePointer( m_leaveobj[i].map );
			return 0;
		}
		i++;
	}

	return 1;
}

/*
 * Function: LRenderer::release
 * Purpose: This function is used to release any memory allocated during 
 *          it's execution
 * Inputs: void
 * Outputs: void
 */
void LRenderer::release( void )
{
	int x;

	// release any memory used by the turtle.
	m_turtle.release();

	// release the branch models
	if( m_branchobj ) {
		for( x = 0; x < NUMBRANCHES; x++ ) {
			if( m_branchobj[x].map ) {
				m_branchobj[x].map->release();
				FreePointer( m_branchobj[x].map );
			}

			// now axe any memory allocated for vertices and faces
			if( m_branchobj[x].vertices ) {
				FreePointer( m_branchobj[x].vertices );
				m_branchobj[x].nverts = 0;
			}

			if( m_branchobj[x].faces ) {
				FreePointer( m_branchobj[x].faces );
				m_branchobj[x].nfaces = 0;
			}
		}
		FreePointer( m_branchobj );
	}

	// release the leave models
	if( m_leaveobj ) {
		for( x = 0; x < NUMLEAVES; x++ ) {
			if( m_leaveobj[x].map ) {
				m_leaveobj[x].map->release();
				FreePointer( m_leaveobj[x].map );
			}
	
			if( m_leaveobj[x].vertices ) {
				FreePointer( m_leaveobj[x].vertices );
				m_leaveobj[x].nverts = 0;
			}

			if( m_leaveobj[x].faces ) {
				FreePointer( m_leaveobj[x].faces );
				m_leaveobj[x].nfaces = 0;
			}
		}
		FreePointer( m_leaveobj );
	}

	// reset the dervation pointer.
	m_derv      = 0;

	// clear the branch and leave vectors
	m_branches.clear();
	m_leaves.clear();
}

/*
 * Function: LRenderer::render
 * Purpose: When an L-System needs to be drawn this function is called
 * Inputs: const int & btype - The type of branch to draw
 *         const int & ltype - The type of leaf to draw
 *         const int & rtype - The type of rendering to use, either dynamic - RENDERDYNAMIC
 *                                                               or static  - RENDERSTAIC
 * Outputs: void
 */
void LRenderer::render( const int & btype, const int & ltype, const int & rtype )
{
	unsigned int i;
	Model * model;
	TurtleState t;
	float m[16];

	if( rtype == RENDERDYNAMIC ) {
		for( i = 0; i < (unsigned int)m_turtle.Root()->m_num_children; i++ ) {
			updatebranch( m_turtle.Root()->m_children[i], 
						  *m_turtle.Root(), 
						  btype, 
						  ltype );
		}
	}
	else if( rtype == RENDERSTATIC ) {
		// Begin by rendering the branches.
		model = 0;
		if( m_branchobj && btype ) {
			model = &m_branchobj[ btype - 1 ];

			// set the texturemap and vertex array's only once
			if( model->map ) {
				model->map->bindmap();
			}
			glInterleavedArrays(GL_T2F_N3F_V3F, 0, model->vertices); 

			for( i = 0; i < m_branches.size(); i++ ) {
				t = m_branches[i];

				// get the matrix representation of the quaternion
				t.m_quat.toMatrix( m );

				glPushMatrix();
					glTranslatef( t.m_p.x, 
								  t.m_p.y,
								  t.m_p.z );
					glMultMatrixf( m );
					glScalef( t.m_width, t.m_length, t.m_width );

					glDrawElements( GL_TRIANGLES,
									model->nfaces * 3,
									GL_UNSIGNED_INT,
									model->faces );
				glPopMatrix();
			}
		}
		else {
			// render the tree using simple lines.
			glDisable( GL_TEXTURE_2D );
			glDisable( GL_LIGHTING );

			glColor3f( 1.0, 1.0, 1.0 );
			for( i = 0; i < m_branches.size(); i++ ) {
				t = m_branches[i];
			
				t.m_quat.toMatrix( m );
				glPushMatrix();
					glTranslatef( t.m_p.x,
								  t.m_p.y,
								  t.m_p.z );
					glMultMatrixf( m );
					glScalef( t.m_width, t.m_length, t.m_width );
					glLineWidth( t.m_width );

					glBegin( GL_LINES );
						glVertex3f( 0.0, 0.0, 0.0 );
						glVertex3f( 0.0, 1.0, 0.0 );
					glEnd();
				glPopMatrix();
			}

			glEnable( GL_LIGHTING );
			glEnable( GL_TEXTURE_2D );
			glLineWidth( 1.0 );
		}

		// now render the leaves
		model = 0;
		if( m_leaveobj && ltype ) {
			model = &m_leaveobj[ ltype - 1 ];

			// bind the texture map and the vertex array only once
			if( model->map ) {
				model->map->bindmap();
			}
			glInterleavedArrays(GL_T2F_N3F_V3F, 0, model->vertices);
			
			for( i = 0; i < m_leaves.size(); i++ ) {
				t = m_leaves[i];

				// get the matrix representation of the quaternion
				t.m_quat.toMatrix( m );

				glPushMatrix();
					glTranslatef( t.m_p.x, 
								  t.m_p.y,
								  t.m_p.z );
					glMultMatrixf( m );
					glScalef( t.m_width, t.m_length, t.m_width );

					glDrawElements( GL_TRIANGLES,
									model->nfaces * 3,
									GL_UNSIGNED_INT,
									model->faces );
				glPopMatrix();
			}
		}
		else {
			glDisable( GL_TEXTURE_2D );
			glDisable( GL_LIGHTING );

			glColor3f( 1.0, 1.0, 1.0 );
			for( i = 0; i < m_leaves.size(); i++ ) {
				t = m_leaves[i];
			
				t.m_quat.toMatrix( m );
				glPushMatrix();
					glTranslatef( t.m_p.x,
								  t.m_p.y,
								  t.m_p.z );
					glMultMatrixf( m );
					glScalef( t.m_width, t.m_length, t.m_width );
					glLineWidth( t.m_width );

					glBegin( GL_LINES );
						glVertex3f( 0.0, 0.0, 0.0 );
						glVertex3f( 0.0, 1.0, 0.0 );
					glEnd();
				glPopMatrix();
			}

			glEnable( GL_LIGHTING );
			glEnable( GL_TEXTURE_2D );
			glLineWidth( 1.0 );
		}
	}
}

/*
 * Function: LRenderer::updatebranch
 * Purpose: This function is used to recursively render and update
 *          a branches rotation and position.
 * Inputs: TurtleState * branch - The branch to render/update
 *		   const TurtleState & parent - The pos/rotation of the parent to the branch
 *         const int & btype - The type of branch to render
 *         const int & ltype - The type of leaf to render
 * Outputs: void
 */
void LRenderer::updatebranch( TurtleState * branch, 
							  const TurtleState & parent, 
							  const int & btype, 
							  const int & ltype)
{
	int i;
	Model * model = 0;
	float   m[16] = {0.0};
	
	TurtleState advance;
	Quaternion quat;
	Vector3D heading;

	// get the branches absolute orientation...
	quat = parent.m_cur_quat * branch->m_cur_quat;
	quat.toMatrix( m );

	heading.x = m[4];
	heading.y = m[5];
	heading.z = m[6];

	// render the branch
	glPushMatrix();
		glTranslatef( parent.m_p.x,
					  parent.m_p.y,
					  parent.m_p.z );
		glMultMatrixf( m );
		glScalef( branch->m_width, branch->m_length, branch->m_width );
		
		// decide which model to render the branch based on 
		// how many children this branch has.
		if( branch->m_num_children ) {
			if( m_branchobj && btype ) {
				model = &m_branchobj[ btype - 1 ];

				// set the texturemap and vertex array's only once
				if( model->map ) {
					model->map->bindmap();
				}
				glInterleavedArrays(GL_T2F_N3F_V3F, 0, model->vertices); 

				// loop through all of the branches and render them
				glDrawElements( GL_TRIANGLES,
								model->nfaces * 3,
								GL_UNSIGNED_INT,
								model->faces );
			}
			else {
				// render the tree using simple lines.
				glDisable( GL_TEXTURE_2D );
				glDisable( GL_LIGHTING );

				glColor3f( 1.0, 1.0, 1.0 );
				glLineWidth( branch->m_width );
				glBegin( GL_LINES );
					glVertex3f( 0.0, 0.0, 0.0 );
					glVertex3f( 0.0, 1.0, 0.0 );
				glEnd();
			
				glEnable( GL_LIGHTING );
				glEnable( GL_TEXTURE_2D );
				glLineWidth( 1.0 );
			}
		}
		else {		// render with leaf model
			if( m_leaveobj && ltype ) {
				model = &m_leaveobj[ ltype - 1 ];

				// set the texturemap and vertex array's only once
				if( model->map ) {
					model->map->bindmap();
				}
				glInterleavedArrays(GL_T2F_N3F_V3F, 0, model->vertices); 

				// loop through all of the branches and render them
				glDrawElements( GL_TRIANGLES,
								model->nfaces * 3,
								GL_UNSIGNED_INT,
								model->faces );
			}
			else {
				// render the tree using simple lines.
				glDisable( GL_TEXTURE_2D );
				glDisable( GL_LIGHTING );

				glColor3f( 1.0, 1.0, 1.0 );
				glLineWidth( branch->m_width );
				glBegin( GL_LINES );
					glVertex3f( 0.0, 0.0, 0.0 );
					glVertex3f( 0.0, 1.0, 0.0 );
				glEnd();

				glEnable( GL_LIGHTING );
				glEnable( GL_TEXTURE_2D );
				glLineWidth( 1.0 );
			}
		}

	glPopMatrix();

	if( branch->m_num_children ) {
		// move the turtle forward and pass this new turtle state as
		// the childrens parent
		advance.m_cur_quat = quat;

		heading *= branch->m_length;
		advance.m_p = parent.m_p + heading;

		// render/update the children of this branch
		for( i = 0; i < branch->m_num_children; i++ ) {
			updatebranch( branch->m_children[i], 
						  advance, 
						  btype, 
						  ltype );
		}
	}

	return;
}


/*
 * Function: LRenderer::compile
 * Purpose: This function takes a dervation and creates an internal 
 *          structure that can be rendered very quickly
 * Inputs: void
 * Outputs: void
 */
void LRenderer::compile( void )
{
	unsigned int x;
	bool doprev = false;
	bool branchtype = false;

	// This loop works by performing modules one step behind.
	// checking to see if the next modules is valid before performing the previous one.
	m_prevmodule = 0;
	if( m_derv ) {
		m_prevmodule = &((*m_derv)[0]);

		x = 1;
		while( x < m_derv->size()) {
		    doprev = false;
		    branchtype = true;
		  
			// look at the next module to determine what to do next
		    switch( (*m_derv)[x].name ) {
				case 'F':                // identifiers
					doprev = true;
					break;
		      
				case ']':				// pop operation
					branchtype = false;
		      
				case '[':				// Push
				case '+':				// all valid operators
				case '-':
				case '/':
				case '\\':
				case '^':
				case '&':
				case '!':
				case '#':
					doprev = true;
					break;
				
				default:
					break;
		    }
			
			// if the current module is valid, perform
			// the previous module
		    if( doprev ) {
				switch( m_prevmodule->name ) {
					case 'F':		// identifiers
						do_identifier( m_prevmodule, branchtype );
						break;
			
					case ']':		// pop operation
						do_pop();
						break;
			
					case '[':		// push operation
						do_push();
						break;
			
					case '+':		// operators
					case '-':
					case '/':
					case '\\':
					case '^':
					case '&':
					case '!':
					case '#':
						do_operator( m_prevmodule );
						break;
			

					default:
						break;
				}
				m_prevmodule = &((*m_derv)[x]);
		    }
		    x++;
		}

		// since we are executing modules one behind, we 
		// need to manually execute the very last module
		// to complete the renderer.
		if( m_prevmodule) {
			switch( m_prevmodule->name ) {
				case 'F':
					do_identifier( m_prevmodule, true );
					break;
				
				case ']':
					do_pop();
					break;
				
				case '[':
					do_push();
					break;
				
				case '+':
				case '-':
				case '/':
				case '\\':
				case '^':
				case '&':
				case '!':
				case '#':
					do_operator( m_prevmodule );
					break;
			
				default:
					break;
			}
		}
	}

	// To use dynamic renderering this function must be called here
	//m_turtle.Root()->ConvertLocal();
}

/*
 * Function: LRenderer::do_operator
 * Purpose: This function is called when an operator is found
 *          in the given dervation
 * Inputs: LSystem::Module * m - Pointer to the operator module
 * Outputs: void
 */
void LRenderer::do_operator( LSystem::Module * m )
{
	float angle = ANGLE;

	if( !m )
		return;

	if( m->parameters.size()) {
		angle = m->parameters[0];
	}

	switch( m->name ) {
		case '+':
			m_turtle.RotateU(angle);	
			break;

		case '-':
			m_turtle.RotateU(-angle);
			break;

		case '\\':
			m_turtle.RotateH(angle);
			break;

		case '/':
			m_turtle.RotateH(-angle);
			break;

		case '&':
			m_turtle.RotateL(-angle);
			break;

		case '^':
			m_turtle.RotateL(angle);
			break;

		case '!':
		    if( m->parameters.size()) {
				m_turtle.Width( m->parameters[0], false );
			}
			else {
				m_turtle.Width( -0.1f, true );
			}
			break;

		case '#':
			if( m->parameters.size() ) {
				m_turtle.Width( m->parameters[0], false );
			}
			else {
				m_turtle.Width( 0.1f, true );
			}
			break;

		default:
			return;
	}
}

/*
 * Function: LRenderer::do_identifier
 * Purpose: This function is called whenever an identifier is
 *          found in the given dervation (Currently only identifiers
 *          named 'F' do anything, everything else is ignored).
 * Inputs: LSystem::Module * m - The identifier module
 * Outputs: void
 */
void LRenderer::do_identifier( LSystem::Module * m, bool branch )
{
	float length = 1.0;

	if( !m )
		return;

	switch( m->name ) {
		case 'F':
			// get the value of the first parameter and use it as a length
			if( m->parameters.size() ) { 
				length = m->parameters[0];
			}
			
			if( branch ) {
				m_branches.push_back( m_turtle.Move( length ) );
			}
			else {
				m_leaves.push_back( m_turtle.Move( length ) );
			}
			break;

		default:
			return;
	}
}

/* simple push and pop operations */
void LRenderer::do_push( void )
{
	m_turtle.Push();
}

void LRenderer::do_pop( void )
{
	m_turtle.Pop();
}

void LRenderer::reset( void ) {
	// release any memory used by the turtle.
	m_turtle.release();

	// reset the dervation pointer.
	m_derv      = 0;

	// clear the branch and leave vectors
	m_branches.clear();
	m_leaves.clear();

}
