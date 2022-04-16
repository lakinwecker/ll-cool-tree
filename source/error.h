//------------------------------------------------------------------------------
// Copyright (C) Sun Feb 15 2004  nikal
//
// error.h: Started at Sun Feb 15 2004 - 16:45:24
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//------------------------------------------------------------------------------
#ifndef ERROR_H
#define ERROR_H

#define DEBUG_ON 0

#include <exception>
#include <sstream>

namespace LSystem {
///-----------------------------------------------------------------------------
/// Error represents a syntactical or semantic error in an LSystem.
///
/// Error represents a syntactical or semantic error in an LSystem,
/// it contains the line number and the column number where the 
/// error was first detected.  It also includes a message describing
/// the error along with the filename and line number of the code
/// that generated the error.  The latter two are useful in tracking
/// bugs that may cause a valid LSystem to not be accepted.
///
/// @author Lakin Wecker
///
/// @see std::exception
///-----------------------------------------------------------------------------
class Error : public std::exception
{ 
	private: 
		std::string myMsg; 
		int myLine;
		int myCol;
		std::string myFileName;
		int myFileLine;
		std::string myWhat;
		
	public: 
		Error(std::string msg, 
				int line, 
				int col, 
				std::string file,
				int fileLine
				) throw() 
			: 
			myMsg( msg ),
			myLine( line ),
			myCol( col ),
			myFileName( file ),
			myFileLine( fileLine )
			{
				std::ostringstream out;
				out << "--[Line:" << line <<
					",Col:" << col << "]-["
					<< file << ":" << fileLine
					<< "]--\t" << msg;
				myWhat = out.str();
			} 
		~Error() throw() {}
		virtual const char *what() throw() { return myWhat.c_str(); }

		std::string getMsg() const {
			return myMsg;
		}
		int getLine() const {
			return myLine;
		}
		int getCol() const {
			return myCol;
		}
		std::string getFileName() const {
			return myFileName;
		}
		int getFileLine() const {
			return myFileLine;
		}
};
};//end namespace LSystem


//------------------------------------------------------------------------------
#define LDEBUG(x) \
if( DEBUG_ON )  \
{ \
	std::cout << "==[DEBUG " << __FILE__ << ":" << __LINE__ << " ==>\t"; \
	x \
	std::cout << " ]==" << std::endl << std::flush;\
}
//------------------------------------------------------------------------------

#endif //#define ERROR_H
