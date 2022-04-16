///-----------------------------------------------------------------------------
/// This file is part of Area17.
///
/// Area17 is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
///
/// Area17 is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with Area17; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
///
/// Copyright (c) 2004 
/// Lakin Wecker aka nikal@nucleus.com
///-----------------------------------------------------------------------------
#include "treescene.h"

#include <cstdio>
#include <cmath>

TreeScene::TreeScene(bool is_sync)
{

	// Configure OpenGL-capable visual.
	Glib::RefPtr<Gdk::GL::Config> glconfig;

	// Try double-buffered visual
	glconfig = Gdk::GL::Config::create(
			Gdk::GL::MODE_RGB |
			Gdk::GL::MODE_DEPTH |
			Gdk::GL::MODE_DOUBLE );

	if (!glconfig)
	{
		std::cerr << "*** Cannot find the double-buffered visual.\n"
			<< "*** Trying single-buffered visual.\n";

		// Try single-buffered visual
		glconfig = Gdk::GL::Config::create(
				Gdk::GL::MODE_RGB | Gdk::GL::MODE_DEPTH );
		if (!glconfig)
		{
			std::cerr << "*** Cannot find any OpenGL-capable visual.\n";
			std::exit(1);
		}
	}

	// print frame buffer attributes.
	// TODO: Get something similar set up for me.
	// GLConfigUtil::examine_gl_attrib(glconfig);

	add_events(
		Gdk::BUTTON1_MOTION_MASK |
		Gdk::BUTTON_PRESS_MASK |
		Gdk::BUTTON_RELEASE_MASK |
		Gdk::SCROLL_MASK );

	// Set OpenGL-capability to the widget.
	set_gl_capability(glconfig);

	m_flags      = 0;
	m_branchtype = 0;
	m_leavetype  = 0;
	memset( &m_wininfo, 0, sizeof(WindowInfo) );

	
}

TreeScene::~TreeScene() {
	m_renderer.release();
}
// button connections
void TreeScene::startzooming(void)
{
	m_flags &= ZOOMING;		// mask out all other bits
	m_flags ^= ZOOMING;		// invert the zooming bit
	
	update();
}

void TreeScene::startpanning(void)
{
	m_flags &= PANNING;		// mask out all other bits
	m_flags ^= PANNING;		// invert the panning bit

	update();
}

void TreeScene::startarcing(void)
{
	m_flags &= ARCING;		// mask out all other bits
	m_flags ^= ARCING;		// invert the arcing bit

	update();
}

// Public slots
void TreeScene::setbranchtype( int type )
{
	if( type >= 0 && type <= NUMBRANCHES ) {
		m_branchtype = type;
		update();
	}
}

void TreeScene::setleavetype( int type )
{
	if( type >= 0 && type <= NUMLEAVES ) {
		m_leavetype = type;
		update();
	}
}



void TreeScene::on_realize()
{
	// We need to call the base on_realize()
	Gtk::DrawingArea::on_realize();

	// Get GL::Drawable.
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	// *** OpenGL BEGIN ***
	if (!gldrawable->gl_begin(get_gl_context())) return;


	float matdiffuse[4] = {0.6f, 0.6f, 0.6f, 1.0};
	float matambient[4] = {0.5f, 0.5f, 0.5f, 1.0};
	float litambient[4] = {0.4f, 0.4f, 0.4f, 1.0};

	glClearColor(0.50f, 0.50f, 0.50f, 1.0);	  // set the clear color to grey

	glEnableClientState(GL_VERTEX_ARRAY);	  // enable vertex arrays to be used, very fast.
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glEnable(GL_DEPTH_TEST);		// enable depth buffer test
	glEnable(GL_TEXTURE_2D);		// enable texture mapping

	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);
	
	glShadeModel(GL_SMOOTH);		// enable gaurad shading
	glEnable(GL_LIGHTING);			// enable lighting
	glEnable(GL_LIGHT0);			// enable the light 0
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, litambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, matdiffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, matambient );
	
	glEnable(GL_NORMALIZE);		    // because we are using glScale allow OpenGL deal with scaling normals

	m_renderer.loadmodels();



	

	gldrawable->gl_end();
	// *** OpenGL END ***
}

bool TreeScene::on_configure_event( GdkEventConfigure* event )
{
	// Get GL::Drawable.
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

	// *** OpenGL BEGIN ***
	if (!gldrawable->gl_begin(get_gl_context()))
		return false;

	// store the new window size
	m_wininfo.m_width = event->width;
	m_wininfo.m_height = event->height;
	
	glViewport(
		0, 
		0, 
		m_wininfo.m_width, 
		m_wininfo.m_height );
	setperspective();


	gldrawable->gl_end();
	// *** OpenGL END ***

	return true;
}

void TreeScene::setperspective(void)
{
	float aspect = (float)m_wininfo.m_width / (float)m_wininfo.m_height;
	
	// create a simple perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
		glFrustum( -0.25 * aspect, 0.25 * aspect, -0.25, 0.25, 0.8, 100.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0, -4.5 );

	update();
}


bool TreeScene::on_expose_event(GdkEventExpose* event)
{
	//---------------------------------------------------------------------
	// Get GL::Drawable.
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
	if (!gldrawable->gl_begin(get_gl_context()))
		return false;


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
		glColor3f( 1.0, 1.0, 1.0 );

		// This sets the camera
		glTranslatef( m_wininfo.m_panx, 
					  m_wininfo.m_pany, 
					  m_wininfo.m_zoom );
				
		glRotatef( m_wininfo.m_viewrotz, 0.0, 0.0, 1.0 );
		glRotatef( m_wininfo.m_viewroty, 1.0, 0.0, 0.0 );
		glRotatef( m_wininfo.m_viewrotx, 0.0, 1.0, 0.0 );
		
		// render the current L-System
		glPushMatrix();
			m_renderer.render( m_branchtype, 
							   m_leavetype, 
							   RENDERSTATIC );
		glPopMatrix();

	glPopMatrix();


	// Swap buffers.
	if (gldrawable->is_double_buffered())
		gldrawable->swap_buffers();
	else
		glFlush();

	gldrawable->gl_end();
	// *** OpenGL END ***

	return true;
}

bool TreeScene::on_button_press_event( GdkEventButton *event ) {
	m_wininfo.m_lastmousex = (int)event->x;
	m_wininfo.m_lastmousey = (int)event->y;


	invalidate();
	return true;
}

bool TreeScene::on_button_release_event( GdkEventButton *event ) {
	invalidate();
	return true;
}

bool TreeScene::on_motion_notify_event( GdkEventMotion *event ) {

	switch(m_flags) {
	case ARCING:		// view rotation
		m_wininfo.m_viewrotx -=
			(int)(( m_wininfo.m_lastmousex - event->x)
			      *AXIS_ROT_SCALE);
		if( m_wininfo.m_viewrotx > 180.0 ) 
			m_wininfo.m_viewrotx -= 360.0;
		else if( m_wininfo.m_viewrotx < -180.0 )
			m_wininfo.m_viewrotx += 360.0;

		m_wininfo.m_viewroty -=
			(int)(( m_wininfo.m_lastmousey - event->y)
			      * AXIS_ROT_SCALE);
		if( m_wininfo.m_viewroty > 180.0 )
			m_wininfo.m_viewroty -= 360.0;
		else if( m_wininfo.m_viewroty < -180.0 )
			m_wininfo.m_viewroty += 360.0;

		break;

	case PANNING:		// view translation
		m_wininfo.m_panx -=
			( m_wininfo.m_lastmousex - event->x )
			      * AXIS_PAN_SCALE;
		m_wininfo.m_pany +=
			( m_wininfo.m_lastmousey - event->y )
			      * AXIS_PAN_SCALE;
		break;

	case ZOOMING:		// view translation
		m_wininfo.m_zoom +=
			(int)(( m_wininfo.m_lastmousey - event->y )
			* AXIS_ZOOM_SCALE);
		break;

	default:
		break;
	}
	
	m_wininfo.m_lastmousex = (int)event->x;
	m_wininfo.m_lastmousey = (int)event->y;

	invalidate();
	return true;
}

bool TreeScene::on_scroll_event( GdkEventScroll *event ) {
	double delta = 0.0;
	if( event->direction == GDK_SCROLL_UP ) {
		delta = 1.0;
	} else if( event->direction == GDK_SCROLL_DOWN ) {
		delta = -1.0;
	}

	m_wininfo.m_zoom += ( delta * AXIS_ZOOM_WHEEL_SCALE );

	invalidate();
	return true;
}

//Set's the modules to our list.
void TreeScene::setmodules( std::vector<LSystem::Module> m )
{
	m_v.clear();
	m_v = m;
	m_renderer.reset();
	m_renderer.setinput( &m_v );
}
