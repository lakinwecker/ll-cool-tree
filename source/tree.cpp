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


//------------------------------------------------------------------------------
// Project includes.
#include "tree.h"
#include "parser.h"


//------------------------------------------------------------------------------
// gtkmm includes.
#include <gtkmm/actiongroup.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>
#include <gtkmm/frame.h>
#include <gtkmm/radiobuttongroup.h>

#include <glibmm/iochannel.h>


//------------------------------------------------------------------------------
// std c++ includes
#include <iostream>
#include <sstream>

//------------------------------------------------------------------------------
Tree::Tree()
{
	//----------------------------------------------------------------------
	// Set up the basic window properties.
	set_title("L-Systems Tree Generator");
	set_default_size(800, 600);

	// Get automatically redrawn if any of their children changed
	// allocation.
	set_reallocate_redraws(true);

	myFilename = "";

	//----------------------------------------------------------------------
	// Create all the widgets.
	Gtk::HPaned *vPane = manage( new Gtk::HPaned() );
	Gtk::Frame *frame1 = manage( new Gtk::Frame( "L-System Editor" ) );
	Gtk::Frame *frame2 = manage( new Gtk::Frame( "L-System Renderer" ) );
	Gtk::VBox *myVBox = manage( new Gtk::VBox() );
	Gtk::VBox *vBoxEdit = manage( new Gtk::VBox() );
	Gtk::VBox *vBoxRender = manage( new Gtk::VBox() );
	Gtk::TextView *textEditor = manage( new Gtk::TextView( ) );
	Gtk::ScrolledWindow *textScroll = manage( new Gtk::ScrolledWindow() );
	Gtk::Button *renderButton = manage( new Gtk::Button( "Render" ) );

	myScene = manage( new TreeScene() );


	//----------------------------------------------------------------------
	// Make sure we actually were able to allocate memory.
	if( !vPane || !frame1 || !frame2 ||
		!myVBox || !vBoxEdit || !vBoxRender ||
		!myScene || !textEditor || !textScroll ||
		!renderButton ) {

		std::cerr << "[" __FILE__ << "," << __LINE__ << "]"
			<< " Unable to allocate enough memory!" << std::endl;
		std::exit( -1 );
	}


	//----------------------------------------------------------------------
	// Get references to all of the object's we'll need later.
	myTextBuffer = textEditor->get_buffer();


	//----------------------------------------------------------------------
	// Set all of the options that we're using with the widgets.
	textScroll->add( *textEditor );
	textEditor->set_size_request( 250, 250 );

	frame1->set_shadow_type( Gtk::SHADOW_IN );
	frame2->set_shadow_type( Gtk::SHADOW_IN );

	/// @TODO: make this use config.h's actual defines for the
	/// program name, and version.
	myStatusBar.push( "L-Systems Tree Generator" );


	renderButton->signal_clicked().connect(
		sigc::mem_fun(*this, &Tree::renderSystem) );

	//----------------------------------------------------------------------
	// This next section defines all of the possible actions,
	// then lays them out into a hiearchy of menus and toolbars.
	Glib::RefPtr<Gtk::ActionGroup> actionGroup
		= Gtk::ActionGroup::create();

	createActions( actionGroup );


	//----------------------------------------------------------------------
	//Define how the actions are presented in the menus and toolbars:
	Glib::RefPtr<Gtk::UIManager> uiManager = Gtk::UIManager::create();
	uiManager->insert_action_group(actionGroup);
	add_accel_group(uiManager->get_accel_group());
	createActionsUI( uiManager );


	// Get a pointer to the toolbar and menu bar widgets.
	Gtk::Widget* pMenuBar = uiManager->get_widget("/MenuBar");
	Gtk::Widget* pFileToolBar = uiManager->get_widget("/FileToolBar");
	Gtk::Widget* pRenderToolBar = uiManager->get_widget("/RenderToolBar");

	// Ensure this actually worked, if it didn't we don't want to continue.
	if( !pMenuBar || !pFileToolBar  || !pRenderToolBar ) { 
		std::cerr << "Couldn't get pointer to toolbar or menubar."
			<< std::endl;
		std::exit(-1);
	}


	
	//----------------------------------------------------------------------
	// Now we finally put all of the gui together.
	//--------------------------------------------
	// The main gui is as follows:
	// There is a menubar at the top, followed by a toolbar.
	// then is the main screen which is split vertically into an editor on
	// the left, and a rendering screen on the right.
	// The text editor and renderer are in named frames.
	// on the bottom is a status bar along with a progress bar.
	//
	//----------------------------------------------------------------------
	myVBox->pack_start( *pMenuBar, Gtk::PACK_SHRINK );
	myVBox->pack_start( *pFileToolBar, Gtk::PACK_SHRINK );


	vBoxEdit->pack_start( *textScroll );
	vBoxEdit->pack_start( *renderButton, Gtk::PACK_SHRINK );
	frame1->add( *vBoxEdit );

	vBoxRender->pack_start( *pRenderToolBar, Gtk::PACK_SHRINK );
	vBoxRender->pack_start( *myScene );
	frame2->add( *vBoxRender );

	vPane->pack1( *frame1, Gtk::SHRINK );
	vPane->pack2( *frame2, Gtk::EXPAND );
	myVBox->pack_start( *vPane );

	myStatusBar.pack_start( myProgressBar, Gtk::PACK_SHRINK );
	myVBox->pack_start( myStatusBar, Gtk::PACK_SHRINK );


	//----------------------------------------------------------------------
	// Set the VBox as the primary widget.
	add(*myVBox);

	show_all();
}



//------------------------------------------------------------------------------
Tree::~Tree() {
}

void Tree::createActionsUI( Glib::RefPtr<Gtk::UIManager> uiManager ) {
	//Layout the actions in a menubar and toolbar:
	try {
		Glib::ustring ui_info =
			"<ui>"
			"  <menubar name='MenuBar'>"
			"    <menu action='MenuFile'>"
			"      <menuitem action='New'/>"
			"      <menuitem action='Open'/>"
			"      <menuitem action='Close'/>"
			"      <menuitem action='Save'/>"
			"      <menuitem action='SaveAs'/>"
			"      <separator/>"
			"      <menuitem action='Quit'/>"
			"    </menu>"
			"  </menubar>"
			"  <toolbar  name='FileToolBar'>"
			"    <toolitem action='New'/>"
			"    <toolitem action='Open'/>"
			"    <toolitem action='Close'/>"
			"    <toolitem action='Save'/>"
			"    <toolitem action='SaveAs'/>"
			"    <separator/>"
			"    <toolitem action='Quit'/>"
			"  </toolbar>"
			"  <toolbar  name='RenderToolBar'>"
			"    <toolitem action='ViewPan'/>"
			"    <toolitem action='ViewZoom'/>"
			"    <toolitem action='ViewRotate'/>"
			"  </toolbar>"
			"</ui>";

		uiManager->add_ui_from_string(ui_info);
	}
	catch( const Glib::Error& ex ) {
		std::cerr << "building menus and toolbars failed: "
			<<  ex.what();
	}
}

//------------------------------------------------------------------------------
void Tree::createActions( Glib::RefPtr<Gtk::ActionGroup> actionGroup )  {

	Gtk::RadioButtonGroup sceneControlGroup;


	actionGroup->add( Gtk::Action::create("MenuFile", "_File") );
	actionGroup->add( Gtk::Action::create("New", Gtk::Stock::NEW),
		sigc::mem_fun(*this, &Tree::onNew) );
	actionGroup->add( Gtk::Action::create("Open", Gtk::Stock::OPEN),
		sigc::mem_fun(*this, &Tree::onOpen) );
	actionGroup->add( Gtk::Action::create("Quit", Gtk::Stock::QUIT),
		sigc::mem_fun(*this, &Tree::onQuit) );
	actionGroup->add( Gtk::Action::create("Close", Gtk::Stock::CLOSE),
		sigc::mem_fun(*this, &Tree::onClose) );
	actionGroup->add( Gtk::Action::create("SaveAs", Gtk::Stock::SAVE_AS),
		sigc::mem_fun(*this, &Tree::onSaveAs) );
	actionGroup->add( Gtk::Action::create("Save", Gtk::Stock::SAVE),
		sigc::mem_fun(*this, &Tree::onSave) );
	actionGroup->add(
			Gtk::Action::create("MenuAction", "_Action") );
	actionGroup->add( Gtk::Action::create("Actions", "_Actions") );
	actionGroup->add(
		Gtk::RadioAction::create(
			sceneControlGroup,
			"ViewPan",
			"View Pan",
			"Select this control to be able to move the tree "
			"around the rendering screen."),
		sigc::mem_fun(*myScene, &TreeScene::startpanning ) );
	actionGroup->add(
		Gtk::RadioAction::create(
			sceneControlGroup,
			"ViewZoom",
			"View Zoom",
			"Select this control to be able to zoom in and out."),
		sigc::mem_fun(*myScene, &TreeScene::startzooming ) );
	actionGroup->add(
		Gtk::RadioAction::create(
			sceneControlGroup,
			"ViewRotate",
			"View Rotate",
			"Select this control to be able to rotate the tree."),
		sigc::mem_fun(*myScene, &TreeScene::startarcing ) );

}


//void Tree::on_hide() {
	//Gtk::Widget::on_hide();
//}

//------------------------------------------------------------------------------
void Tree::onClose() {
}


//------------------------------------------------------------------------------
void Tree::onOpen() {
	//----------------------------------------------------------------------
	// Some defines we'll use.
	const int OPEN = 1;
	const int CANCEL = 2;

	
	//----------------------------------------------------------------------
	// Show the file dialog.
	Gtk::FileChooserDialog fileDialog(
		*this, "Choose an l-system file." );
	fileDialog.add_button( Gtk::Stock::CANCEL, CANCEL );
	fileDialog.add_button( Gtk::Stock::OPEN, OPEN );
	int val = fileDialog.run();
	if( val == CANCEL ) {
		return;
	}




	//----------------------------------------------------------------------
	// Get the filename and read it in.
	std::string filename = fileDialog.get_filename();

	Glib::RefPtr<Glib::IOChannel> inputChannel = 
		Glib::IOChannel::create_from_file(
			filename, "r" );

	Glib::ustring text;
	if( inputChannel->read_to_end( text) != Glib::IO_STATUS_NORMAL ) {
		std::cerr << "[" << __FILE__ << "," << __LINE__ << "]"
			<< "Error reading from file: " << filename
			<< std::endl;
		return;
	}

	//----------------------------------------------------------------------
	// Save the filename for future use.
	myFilename = filename;

	//----------------------------------------------------------------------
	// Set the text as the active l-System.
	myTextBuffer->set_text( text );
		
	
	renderSystem();

}


void Tree::renderSystem() {
	// Create a stream to it.
	std::istringstream in( myTextBuffer->get_text() );
	LSystem::Parser p(in);

	try {
		p.parseLSystem();

		myScene->setleavetype( p['L'] );
		myScene->setbranchtype( p['B'] );
		myScene->setmodules( p.evaluateSystem() );
		
	} catch ( LSystem::Error *e ) {
		std::cout << e->getFileName() << ":"
			<< e->getLine() << ": error:"
			<< e->getMsg() << std::endl;
		std::cout << "Line: " << e->getLine() << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << "| " << e->getMsg() << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::cout << "CPP File:      "
			<< e->getFileName() << std::endl;
		std::cout << "CPP File Line: "
			<< e->getFileLine() << std::endl;
	}


	myScene->invalidate();
}


void Tree::onQuit() {
	// Hiding this window cause main to quit.
	hide();
}




void Tree::onNew( void ) {

	if( !myTextBuffer->get_modified() || canContinue( "Create new file" )) {
		//--------------------------------------------------------------
		// If we get this far, they're serious about the whole
		// "new" thing.
		myTextBuffer->set_text( "" );
		myFilename = "";
	}
}



void Tree::onSave( void ) {

	if( myTextBuffer->get_modified() ) {
		save();
	}

}


void Tree::onSaveAs( void ) {
}


void Tree::onAbout( void ) {
}


bool Tree::canContinue( Glib::ustring action ) {
	const int OK = 2;
	const int CANCEL = 3;
	Gtk::MessageDialog dialog(
		action + 
		"You have unsaved changes in this l-system, are you "
		"sure you want to continue? ",
		false,
		Gtk::MESSAGE_QUESTION,
		Gtk::BUTTONS_NONE,
		true );
	dialog.add_button( Gtk::Stock::CANCEL, CANCEL );
	dialog.add_button( Gtk::Stock::OK, OK );

	int val = dialog.run();
	if( val == CANCEL ) {
		return false; 
	}
	
	return true;
}

void Tree::save( void ) {

	std::string filename = myFilename;

	if( myFilename == "" ) {
		//--------------------------------------------------------------
		// Some defines we'll use.
		const int SAVE = 1;
		const int CANCEL = 2;

		
		//--------------------------------------------------------------
		// Show the file dialog.
		Gtk::FileChooserDialog fileDialog(
			*this,
			"Choose filename to save as.",
			Gtk::FILE_CHOOSER_ACTION_SAVE);
		fileDialog.add_button( Gtk::Stock::CANCEL, CANCEL );
		fileDialog.add_button( Gtk::Stock::SAVE, SAVE );
		int val = fileDialog.run();
		if( val == CANCEL ) {
			return;
		}


		//--------------------------------------------------------------
		// Get the filename and read it in.
		filename = fileDialog.get_filename();
	}
	

	Glib::RefPtr<Glib::IOChannel> outputChannel = 
		Glib::IOChannel::create_from_file(
			filename, "w" );

	Glib::ustring text = myTextBuffer->get_text();

	if( outputChannel->write( text ) != Glib::IO_STATUS_NORMAL ) {
		std::cerr << "[" << __FILE__ << "," << __LINE__ << "]"
			<< "Error writing to file: " << filename
			<< std::endl;
		return;
	}

	// The last thing we do is indicate that we have saved this text.
	myTextBuffer->set_modified( false );

}

void Tree::onRenderSystem( std::vector< LSystem::Module > p ) {
}



