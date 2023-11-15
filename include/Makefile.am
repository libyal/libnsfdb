include_HEADERS = \
	libnsfdb.h

pkginclude_HEADERS = \
	libnsfdb/codepage.h \
	libnsfdb/definitions.h \
	libnsfdb/error.h \
	libnsfdb/extern.h \
	libnsfdb/features.h \
	libnsfdb/types.h

EXTRA_DIST = \
	libnsfdb.h.in \
	libnsfdb/definitions.h.in \
	libnsfdb/features.h.in \
	libnsfdb/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libnsfdb.h
	-rm -f libnsfdb/definitions.h
	-rm -f libnsfdb/features.h
	-rm -f libnsfdb/types.h
	-rm -f Makefile

