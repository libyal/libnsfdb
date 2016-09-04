AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBCSYSTEM_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBNSFDB_DLL_IMPORT@

TESTS = \
	test_api_functions.sh \
	test_nsfdbinfo.sh \
	test_nsfdbexport.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_nsfdbexport.sh \
	test_nsfdbinfo.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	nsfdb_test_error \
	nsfdb_test_get_version \
	nsfdb_test_open_close

nsfdb_test_error_SOURCES = \
	nsfdb_test_error.c \
	nsfdb_test_libcstring.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_unused.h

nsfdb_test_error_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCSTRING_LIBADD@

nsfdb_test_get_version_SOURCES = \
	nsfdb_test_get_version.c \
	nsfdb_test_libcstring.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_unused.h

nsfdb_test_get_version_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCSTRING_LIBADD@

nsfdb_test_open_close_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libcstring.h \
	nsfdb_test_libcsystem.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_open_close.c

nsfdb_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libnsfdb/libnsfdb.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@PTHREAD_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

