## Process this file with automake to produce Makefile.in


AUTOMAKE_OPTIONS = 1.4

SUBDIRS = source

## Files that aren't included automatically (e.g. html, docs, etc.)
EXTRA_DIST =   autogen.sh COPYING TODO README \
	       BUGS 

#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = wftk@WFTK_SUFFIX@.pc

# Rule to build tar-gzipped distribution package
$(PACKAGE)-$(VERSION).tar.gz: dist

# Rule to build RPM distribution package
#rpm: $(PACKAGE)-$(VERSION).tar.gz wftk.spec
#cp $(PACKAGE)-$(VERSION).tar.gz @RPM_DIR@/SOURCES
#rpm -ba wftk.spec     

# Rule to install the libraries only - prevent rebuilding apps
#install-lib:
#	cd wftk && $(MAKE) install-libLTLIBRARIES

# Run ldconfig after installing the library:
#install-hook:
#	-ldconfig    

#documentation
doc:
	doxygen Doxyfile 

dist-hook:
	mkdirhier $(distdir)/macros
	cp -f $(srcdir)/macros/autogen.sh $(distdir)/macros

