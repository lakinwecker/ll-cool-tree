///-----------------------------------------------------------------------------
/// This file is part of tree.
///
/// tree is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
///
/// tree is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with tree; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
///
/// Copyright (c) 2004 
/// Lakin Wecker aka nikal@nucleus.com
///-----------------------------------------------------------------------------
#ifndef TREE_H
#define TREE_H


//------------------------------------------------------------------------------
// Project includes.
#include "treescene.h"
#include "module.h"


//------------------------------------------------------------------------------
// gtkmm includes.
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/menubar.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/menu.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/textview.h>



///-----------------------------------------------------------------------------
/// This class is the main window for the tree program.
///
/// It setups up all of the layout, widgets, menus, and general information.
///-----------------------------------------------------------------------------
class Tree : public Gtk::Window
{


///-----------------------------------------------------------------------------
/// The public API
///-----------------------------------------------------------------------------
public:


	///---------------------------------------------------------------------
	/// The default constructor.
	///---------------------------------------------------------------------
	Tree();


	///---------------------------------------------------------------------
	/// @TODO: There needs to be a copy constructor and assignment operator
	/// implemented for this class.
	///---------------------------------------------------------------------


	///---------------------------------------------------------------------
	/// The default constructor.
	///---------------------------------------------------------------------
	virtual ~Tree();


///-----------------------------------------------------------------------------
/// Some internal signals used for handling the various menu events.
///-----------------------------------------------------------------------------
protected:

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onNew( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onOpen( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onSave( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onSaveAs( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onClose( void );

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onQuit( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onAbout( void );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	virtual void onRenderSystem( std::vector< LSystem::Module > p );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void createActions( Glib::RefPtr<Gtk::ActionGroup> actionGroup );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void createActionsUI( Glib::RefPtr<Gtk::UIManager> uiManager );

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void save();


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	bool canContinue( Glib::ustring action );


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	void renderSystem();

///-----------------------------------------------------------------------------
/// Protected member methods.
///-----------------------------------------------------------------------------
protected:


	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	//virtual void on_hide();



///-----------------------------------------------------------------------------
/// Member variables.
///-----------------------------------------------------------------------------
protected:
	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	Glib::ustring myFilename;

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	Gtk::Statusbar myStatusBar;

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	Gtk::ProgressBar myProgressBar;

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	Glib::RefPtr< Gtk::TextBuffer > myTextBuffer;

	///---------------------------------------------------------------------
	/// @TODO: Document this.
	///---------------------------------------------------------------------
	TreeScene *myScene;

};
#endif //TREE_H
