AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
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
	@LIBFVALUE_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBNSFDB_DLL_IMPORT@

TESTS = \
	test_api_functions.sh \
	test_api_types.sh \
	test_nsfdbinfo.sh \
	test_nsfdbexport.sh

check_SCRIPTS = \
	test_api_functions.sh \
	test_api_types.sh \
	test_nsfdbexport.sh \
	test_nsfdbinfo.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	nsfdb_test_bucket \
	nsfdb_test_error \
	nsfdb_test_file \
	nsfdb_test_io_handle \
	nsfdb_test_note \
	nsfdb_test_note_item_class \
	nsfdb_test_note_item_type \
	nsfdb_test_note_value \
	nsfdb_test_notify \
	nsfdb_test_rrv_bucket \
	nsfdb_test_rrv_bucket_descriptor \
	nsfdb_test_rrv_value \
	nsfdb_test_support \
	nsfdb_test_unique_name_key

nsfdb_test_bucket_SOURCES = \
	nsfdb_test_bucket.c \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_unused.h

nsfdb_test_bucket_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_error_SOURCES = \
	nsfdb_test_error.c \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_unused.h

nsfdb_test_error_LDADD = \
	../libnsfdb/libnsfdb.la

nsfdb_test_file_SOURCES = \
	nsfdb_test_file.c \
	nsfdb_test_getopt.c nsfdb_test_getopt.h \
	nsfdb_test_libcerror.h \
	nsfdb_test_libclocale.h \
	nsfdb_test_libcnotify.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_libuna.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h

nsfdb_test_file_LDADD = \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libnsfdb/libnsfdb.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

nsfdb_test_io_handle_SOURCES = \
	nsfdb_test_io_handle.c \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_unused.h

nsfdb_test_io_handle_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_note_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_note.c \
	nsfdb_test_unused.h

nsfdb_test_note_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_note_item_class_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_note_item_class.c \
	nsfdb_test_unused.h

nsfdb_test_note_item_class_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_note_item_type_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_note_item_type.c \
	nsfdb_test_unused.h

nsfdb_test_note_item_type_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_note_value_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_note_value.c \
	nsfdb_test_unused.h

nsfdb_test_note_value_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_notify_SOURCES = \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_notify.c \
	nsfdb_test_unused.h

nsfdb_test_notify_LDADD = \
	../libnsfdb/libnsfdb.la

nsfdb_test_rrv_bucket_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_rrv_bucket.c \
	nsfdb_test_unused.h

nsfdb_test_rrv_bucket_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_rrv_bucket_descriptor_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_rrv_bucket_descriptor.c \
	nsfdb_test_unused.h

nsfdb_test_rrv_bucket_descriptor_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_rrv_value_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_rrv_value.c \
	nsfdb_test_unused.h

nsfdb_test_rrv_value_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_support_SOURCES = \
	nsfdb_test_getopt.c nsfdb_test_getopt.h \
	nsfdb_test_libbfio.h \
	nsfdb_test_libcerror.h \
	nsfdb_test_libclocale.h \
	nsfdb_test_libcnotify.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_libuna.h \
	nsfdb_test_macros.h \
	nsfdb_test_support.c

nsfdb_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

nsfdb_test_unique_name_key_SOURCES = \
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_unique_name_key.c \
	nsfdb_test_unused.h

nsfdb_test_unique_name_key_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

