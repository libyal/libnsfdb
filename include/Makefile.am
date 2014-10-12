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
	/bin/rm -f libnsfdb.h
	/bin/rm -f libnsfdb/definitions.h
	/bin/rm -f libnsfdb/features.h
	/bin/rm -f libnsfdb/types.h
	/bin/rm -f Makefile

