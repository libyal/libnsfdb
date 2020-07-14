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
	@LIBNSFDB_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	nsfdbexport \
	nsfdbinfo

nsfdbexport_SOURCES = \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	nsfdbexport.c \
	nsfdbtools_getopt.c nsfdbtools_getopt.h \
	nsfdbtools_libcerror.h \
	nsfdbtools_libcfile.h \
	nsfdbtools_libclocale.h \
	nsfdbtools_libcnotify.h \
	nsfdbtools_libcpath.h \
	nsfdbtools_libfdatetime.h \
	nsfdbtools_libnsfdb.h \
	nsfdbtools_output.c nsfdbtools_output.h \
	nsfdbtools_signal.c nsfdbtools_signal.h \
	nsfdbtools_unused.h

nsfdbexport_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

nsfdbinfo_SOURCES = \
	info_handle.c info_handle.h \
	nsfdbinfo.c \
	nsfdbtools_getopt.c nsfdbtools_getopt.h \
	nsfdbtools_libcerror.h \
	nsfdbtools_libclocale.h \
	nsfdbtools_libcnotify.h \
	nsfdbtools_libfdatetime.h \
	nsfdbtools_libnsfdb.h \
	nsfdbtools_output.c nsfdbtools_output.h \
	nsfdbtools_signal.c nsfdbtools_signal.h \
	nsfdbtools_unused.h

nsfdbinfo_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libnsfdb/libnsfdb.la \
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

