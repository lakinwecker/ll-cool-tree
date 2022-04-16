#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

PKG_NAME="entityforge"

(test -f $srcdir/configure.ac \
  && test -d $srcdir/source \
  && test -f $srcdir/source/main.cpp) || {
    echo -n "**Error**: Directory "\`$srcdir\'" does not look like the"
    echo " top-level tree directory"
    exit 1
}

#export CC=gcc-3.4
#export CXX=g++-3.4
#export CPP=cpp-3.4

test -z "$1" || case "$1" in
   win32)
      export MINGW32PATH=/usr

      sys_lib_search_path_spec=$MINGW32PATH/lib 
	  
      if test -d "$2"; then 
		MINGW32PATH="$2"
		shift 2;
	  else
		shift 1;
	  fi
      
	  autogenoptions="--enable-win32 --host=i586-mingw32 --program-suffix=.exe --bindir=\$(prefix) --cache-file=config.cross.cache --disable-shared $@"
  
    
      echo Configuring for cross-compilation with Mingw32 installation in $MINGW32PATH ...
#
# these are new filenames of the cross-compile package from 
# http://www.libsdl.org/Xmingw32/index.html
#            
      #export CC=$MINGW32PATH/bin/i586-mingw32msvc-gcc 
      #export CXX=$MINGW32PATH/bin/i586-mingw32msvc-g++
      #export AR=$MINGW32PATH/bin/i586-mingw32msvc-ar
      #export AS=$MINGW32PATH/bin/i586-mingw32msvc-as
      #export NM=$MINGW32PATH/bin/i586-mingw32msvc-nm
      #export RANLIB=$MINGW32PATH/bin/i586-mingw32msvc-ranlib
      #export CFLAGS="-I$MINGW32PATH/i586-mingw32msvc/include -I$MINGW32PATH/include"
      #export CPPFLAGS="-I$MINGW32PATH/i586-mingw32msvc/include -I$MINGW32PATH/include -I$MINGW32PATH/lib/sigc++/include"      
      #export CXXFLAGS="-I$MINGW32PATH/i586-mingw32msvc/include"
      #export LDFLAGS="-L$MINGW32PATH/i586-mingw32msvc/lib -L$MINGW32PATH/lib"
      #export LD_PATH=$MINGW32PATH/i586-mingw32msvc/lib
      #export DLLTOOL=$MINGW32PATH/bin/i586-mingw32msvc-dlltool  
      #export DLLWARP=$MINGW32PATH/bin/i586-mingw32msvc-dllwrap
	;;
   *)
      autogenoptions="$@"
      ;;
esac

. $srcdir/real-autogen.sh $autogenoptions
