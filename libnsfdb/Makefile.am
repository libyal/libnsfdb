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
	@PTHREAD_CPPFLAGS@

lib_LTLIBRARIES = libnsfdb.la

libnsfdb_la_SOURCES = \
	libnsfdb.c \
	libnsfdb_bucket.c libnsfdb_bucket.h \
	libnsfdb_bucket_list.c libnsfdb_bucket_list.h \
	libnsfdb_checksum.c libnsfdb_checksum.h \
	libnsfdb_compression.c libnsfdb_compression.h \
	libnsfdb_debug.c libnsfdb_debug.h \
	libnsfdb_definitions.h \
	libnsfdb_error.c libnsfdb_error.h \
	libnsfdb_extern.h \
	libnsfdb_file.c libnsfdb_file.h \
	libnsfdb_io_handle.c libnsfdb_io_handle.h \
	libnsfdb_libbfio.h \
	libnsfdb_libcdata.h \
	libnsfdb_libcerror.h \
	libnsfdb_libclocale.h \
	libnsfdb_libcnotify.h \
	libnsfdb_libfcache.h \
	libnsfdb_libfdata.h \
	libnsfdb_libfdatetime.h \
	libnsfdb_libuna.h \
	libnsfdb_note.c libnsfdb_note.h \
	libnsfdb_note_item_class.c libnsfdb_note_item_class.h \
	libnsfdb_note_item_type.c libnsfdb_note_item_type.h \
	libnsfdb_note_value.c libnsfdb_note_value.h \
	libnsfdb_notify.c libnsfdb_notify.h \
	libnsfdb_rrv_bucket.c libnsfdb_rrv_bucket.h \
	libnsfdb_rrv_bucket_descriptor.c libnsfdb_rrv_bucket_descriptor.h \
	libnsfdb_rrv_value.c libnsfdb_rrv_value.h \
	libnsfdb_support.c libnsfdb_support.h \
	libnsfdb_types.h \
	libnsfdb_unique_name_key.c libnsfdb_unique_name_key.h \
	libnsfdb_unique_name_key_table.c libnsfdb_unique_name_key_table.h \
	libnsfdb_unused.h \
	nsfdb_bucket.h \
	nsfdb_bucket_descriptor_block.h \
	nsfdb_database_header.h \
	nsfdb_file_header.h \
	nsfdb_note.h \
	nsfdb_rrv_bucket.h \
	nsfdb_superblock.h

libnsfdb_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@PTHREAD_LIBADD@

libnsfdb_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libnsfdb_definitions.h.in \
	libnsfdb.rc \
	libnsfdb.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libnsfdb_definitions.h
	/bin/rm -f libnsfdb.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libnsfdb ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libnsfdb_la_SOURCES)

