AC_INIT(source/main.cpp)
AM_INIT_AUTOMAKE(tree, 0.0.1)
AM_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AM_PROG_LIBTOOL

CXXFLAGS="$CXXFLAGS -Wall"
AC_LANG_CPLUSPLUS

PKG_CHECK_MODULES(GTKMM,[gtkmm-2.4 >= 2.4.0])
AC_SUBST(GTKMM_CFLAGS)
AC_SUBST(GTKMM_LIBS)

PKG_CHECK_MODULES(GTKGLEXTMM,[gtkglextmm-1.2 >= 1.1.0])
AC_SUBST(GTKGLEXTMM_CFLAGS)
AC_SUBST(GTKGLEXTMM_LIBS)

#AC_CHECK_LIB(z, deflate, , AC_MSG_ERROR([libz library not found!]))
#AC_CHECK_LIB(png, main, , AC_MSG_ERROR([libpng library not found!]))

#PKG_CHECK_MODULES(VARCONF,[varconf-1.0 >= 0.6.2])
#AC_SUBST(VARCONF_CFLAGS)
#AC_SUBST(VARCONF_LIBS)

########
# Checking for 3D libs, all but cal3d are optional
# cal3d is non-optional because it's rooted too deep into the program

# Test for cal3d

#PKG_CHECK_MODULES(CAL3D, [cal3d])
#PKG_CHECK_MODULES(WFMATH, [wfmath-0.3])

########


if test "x$prefix" != xNONE; then
    PREFIX=$prefix
  else
    PREFIX=$ac_default_prefix
fi

AC_DEFINE_UNQUOTED(INSTALLDIR,"$PREFIX",[Install prefix])

AC_OUTPUT([
    source/Makefile
    Makefile
])
