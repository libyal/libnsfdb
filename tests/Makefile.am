AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

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
	nsfdb_test_functions.c nsfdb_test_functions.h \
	nsfdb_test_getopt.c nsfdb_test_getopt.h \
	nsfdb_test_libcerror.h \
	nsfdb_test_libclocale.h \
	nsfdb_test_libcnotify.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_libuna.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_unused.h

nsfdb_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
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
	nsfdb_test_libcerror.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_macros.h \
	nsfdb_test_notify.c \
	nsfdb_test_unused.h

nsfdb_test_notify_LDADD = \
	../libnsfdb/libnsfdb.la \
	@LIBCERROR_LIBADD@

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
	nsfdb_test_functions.c nsfdb_test_functions.h \
	nsfdb_test_getopt.c nsfdb_test_getopt.h \
	nsfdb_test_libbfio.h \
	nsfdb_test_libcerror.h \
	nsfdb_test_libclocale.h \
	nsfdb_test_libcnotify.h \
	nsfdb_test_libnsfdb.h \
	nsfdb_test_libuna.h \
	nsfdb_test_macros.h \
	nsfdb_test_memory.c nsfdb_test_memory.h \
	nsfdb_test_support.c \
	nsfdb_test_unused.h

nsfdb_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libnsfdb.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libnsfdb

test_inputs_nsfdbinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) nsfdbinfo

test_inputs_nsfdbexport.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) nsfdbexport

test_library: package.m4 test_inputs_libnsfdb.at test_library.at test_macros.at

test_manpages: package.m4 test_manpages.at

test_tools: package.m4 test_macros.at test_inputs_nsfdbexport.at test_inputs_nsfdbinfo.at test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

