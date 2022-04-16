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
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with Area17; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
/// Copyright (c) 2004 
/// Lakin Wecker aka nikal@nucleus.com
///-----------------------------------------------------------------------------
#ifndef TREESCENE_H
#define TREESCENE_H

///-----------------------------------------------------------------------------
/// Project Includes
#include "renderer.h"
#include "parser.h"

///-----------------------------------------------------------------------------
/// C++ includes
#include <iostream>
#include <cstdlib>
#include <cmath>


///-----------------------------------------------------------------------------
/// Windowing Toolkit includes.
#include <gtkmm.h>
#include <gtkglmm.h>


///-----------------------------------------------------------------------------
/// Some windows includes for when runnning on windows.
#ifdef G_OS_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif

///-----------------------------------------------------------------------------
/// OpenGL Includes.
#include <GL/gl.h>
#include <GL/glu.h>

///-----------------------------------------------------------------------------
/// The TreeScene class which holds the objects and information to render
/// the terrain.
///-----------------------------------------------------------------------------
class TreeScene : public Gtk::GL::DrawingArea {
public:
	
	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	explicit TreeScene( bool is_sync = true );

	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	virtual ~TreeScene();
	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void startpanning( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void startzooming( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void startarcing( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void setbranchtype( int type );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void setleavetype( int type );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void setmodules( std::vector<LSystem::Module> m );


protected:
	// signal handlers:
	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	virtual void on_realize();

	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	virtual bool on_configure_event( GdkEventConfigure* event );

	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	virtual bool on_expose_event( GdkEventExpose* event );

	virtual bool on_button_press_event( GdkEventButton *event );
	virtual bool on_button_release_event( GdkEventButton *event );
	virtual bool on_motion_notify_event( GdkEventMotion *event );
	virtual bool on_scroll_event( GdkEventScroll *event );

	void setperspective(void);

public:
	// Invalidate whole window.
	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	void invalidate( ) {
		Gtk::Allocation allocation = get_allocation( );
		get_window()->invalidate_rect( allocation, false );
	}

	// Update window synchronously (fast).
	///---------------------------------------------------------------------
	///---------------------------------------------------------------------
	void update( ) {
		get_window()->process_updates(false);
	}
///-----------------------------------------------------------------------------
/// Some internal structures used to store information.
///-----------------------------------------------------------------------------
protected:
	typedef struct __windowinfo__
	{
		// stores the size of the window
		int m_width;
		int m_height;
		
		// used to track the mouse.
		int m_lastmousex;
		int m_lastmousey;

		// rotation angles
		float m_viewrotx;
		float m_viewroty;
		float m_viewrotz;

		// zoom factor.
		float m_zoom;

		// panning information.
		float m_panx;
		float m_pany;

	} WindowInfo;

///-----------------------------------------------------------------------------
/// Member variables.
///-----------------------------------------------------------------------------
protected:
	unsigned int m_flags;
	unsigned int m_branchtype;
	unsigned int m_leavetype;

	WindowInfo m_wininfo;

	LRenderer m_renderer;
	std::vector<LSystem::Module> m_v;
	
///-----------------------------------------------------------------------------
/// Member constants.
///-----------------------------------------------------------------------------
protected:

	//----------------------------------------------------------------------
	const static int ZOOMING = 0x01;
	const static int ARCING = 0x02;
	const static int PANNING = 0x04;

	//----------------------------------------------------------------------
	constexpr const static double TWOPI = 6.283185307;
	constexpr const static double DEGTORAD = 0.017453292;
	//----------------------------------------------------------------------
	// reduces the amount of rotation per pixel
	constexpr const static double AXIS_ROT_SCALE = 0.45;

	//----------------------------------------------------------------------
	// reduces pan per pixel
	constexpr const static double AXIS_PAN_SCALE = 0.0625;

	//----------------------------------------------------------------------
	// reduces zoom per pixel
	constexpr const static double AXIS_ZOOM_SCALE = 0.025;

	//----------------------------------------------------------------------
	// reduces the amount of zoom per wheel tick
	constexpr const static double AXIS_ZOOM_WHEEL_SCALE = 0.833;


};

#endif//TREESCENE_H
