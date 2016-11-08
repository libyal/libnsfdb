AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBNSFDB_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	nsfdbexport \
	nsfdbinfo

nsfdbexport_SOURCES = \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	nsfdbexport.c \
	nsfdbtools_libcerror.h \
	nsfdbtools_libcfile.h \
	nsfdbtools_libclocale.h \
	nsfdbtools_libcnotify.h \
	nsfdbtools_libcpath.h \
	nsfdbtools_libcsystem.h \
	nsfdbtools_libfdatetime.h \
	nsfdbtools_libnsfdb.h \
	nsfdboutput.c nsfdboutput.h

nsfdbexport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libnsfdb/libnsfdb.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

nsfdbinfo_SOURCES = \
	nsfdbinfo.c \
	nsfdbtools_libcerror.h \
	nsfdbtools_libclocale.h \
	nsfdbtools_libcnotify.h \
	nsfdbtools_libcsystem.h \
	nsfdbtools_libfdatetime.h \
	nsfdbtools_libnsfdb.h \
	nsfdboutput.c nsfdboutput.h

nsfdbinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libnsfdb/libnsfdb.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on nsfdbexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(nsfdbexport_SOURCES)
	@echo "Running splint on nsfdbinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(nsfdbinfo_SOURCES)

