/*
 * Library file type test program
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "nsfdb_test_functions.h"
#include "nsfdb_test_getopt.h"
#include "nsfdb_test_libcerror.h"
#include "nsfdb_test_libclocale.h"
#include "nsfdb_test_libnsfdb.h"
#include "nsfdb_test_libuna.h"
#include "nsfdb_test_macros.h"
#include "nsfdb_test_memory.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make nsfdb_test_file generate verbose output
#define NSFDB_TEST_FILE_VERBOSE
 */

/* Creates and opens a source file
 * Returns 1 if successful or -1 on error
 */
int nsfdb_test_file_open_source(
     libnsfdb_file_t **file,
     const system_character_t *source,
     libcerror_error_t **error )
{
	static char *function = "nsfdb_test_file_open_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( libnsfdb_file_initialize(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libnsfdb_file_open_wide(
	          *file,
	          source,
	          LIBNSFDB_OPEN_READ,
	          error );
#else
	result = libnsfdb_file_open(
	          *file,
	          source,
	          LIBNSFDB_OPEN_READ,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file != NULL )
	{
		libnsfdb_file_free(
		 file,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source file
 * Returns 1 if successful or -1 on error
 */
int nsfdb_test_file_close_source(
     libnsfdb_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "nsfdb_test_file_close_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libnsfdb_file_close(
	     *file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		result = -1;
	}
	if( libnsfdb_file_free(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libnsfdb_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libnsfdb_file_t *file           = NULL;
	int result                      = 0;

#if defined( HAVE_NSFDB_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libnsfdb_file_initialize(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_file_free(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libnsfdb_file_initialize(
	          NULL,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	file = (libnsfdb_file_t *) 0x12345678UL;

	result = libnsfdb_file_initialize(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	file = NULL;

#if defined( HAVE_NSFDB_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libnsfdb_file_initialize with malloc failing
		 */
		nsfdb_test_malloc_attempts_before_fail = test_number;

		result = libnsfdb_file_initialize(
		          &file,
		          &error );

		if( nsfdb_test_malloc_attempts_before_fail != -1 )
		{
			nsfdb_test_malloc_attempts_before_fail = -1;

			if( file != NULL )
			{
				libnsfdb_file_free(
				 &file,
				 NULL );
			}
		}
		else
		{
			NSFDB_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			NSFDB_TEST_ASSERT_IS_NULL(
			 "file",
			 file );

			NSFDB_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libnsfdb_file_initialize with memset failing
		 */
		nsfdb_test_memset_attempts_before_fail = test_number;

		result = libnsfdb_file_initialize(
		          &file,
		          &error );

		if( nsfdb_test_memset_attempts_before_fail != -1 )
		{
			nsfdb_test_memset_attempts_before_fail = -1;

			if( file != NULL )
			{
				libnsfdb_file_free(
				 &file,
				 NULL );
			}
		}
		else
		{
			NSFDB_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			NSFDB_TEST_ASSERT_IS_NULL(
			 "file",
			 file );

			NSFDB_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_NSFDB_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libnsfdb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libnsfdb_file_free function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libnsfdb_file_free(
	          NULL,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libnsfdb_file_open function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libnsfdb_file_t *file    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = nsfdb_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_file_initialize(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libnsfdb_file_open(
	          file,
	          narrow_source,
	          LIBNSFDB_OPEN_READ,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libnsfdb_file_open(
	          file,
	          narrow_source,
	          LIBNSFDB_OPEN_READ,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libnsfdb_file_free(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libnsfdb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libnsfdb_file_open_wide function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libnsfdb_file_t *file    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = nsfdb_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_file_initialize(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libnsfdb_file_open_wide(
	          file,
	          wide_source,
	          LIBNSFDB_OPEN_READ,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libnsfdb_file_open_wide(
	          file,
	          wide_source,
	          LIBNSFDB_OPEN_READ,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libnsfdb_file_free(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libnsfdb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libnsfdb_file_close function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libnsfdb_file_close(
	          NULL,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libnsfdb_file_open and libnsfdb_file_close functions
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libnsfdb_file_t *file    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libnsfdb_file_initialize(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libnsfdb_file_open_wide(
	          file,
	          source,
	          LIBNSFDB_OPEN_READ,
	          &error );
#else
	result = libnsfdb_file_open(
	          file,
	          source,
	          LIBNSFDB_OPEN_READ,
	          &error );
#endif

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_file_close(
	          file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libnsfdb_file_open_wide(
	          file,
	          source,
	          LIBNSFDB_OPEN_READ,
	          &error );
#else
	result = libnsfdb_file_open(
	          file,
	          source,
	          LIBNSFDB_OPEN_READ,
	          &error );
#endif

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_file_close(
	          file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libnsfdb_file_free(
	          &file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libnsfdb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libnsfdb_file_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_file_signal_abort(
     libnsfdb_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libnsfdb_file_signal_abort(
	          file,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libnsfdb_file_signal_abort(
	          NULL,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libcerror_error_t *error   = NULL;
	libnsfdb_file_t *file      = NULL;
	system_character_t *source = NULL;
	system_integer_t option    = 0;
	int result                 = 0;

	while( ( option = nsfdb_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( NSFDB_TEST_FILE_VERBOSE )
	libnsfdb_notify_set_verbose(
	 1 );
	libnsfdb_notify_set_stream(
	 stderr,
	 NULL );
#endif

	NSFDB_TEST_RUN(
	 "libnsfdb_file_initialize",
	 nsfdb_test_file_initialize );

	NSFDB_TEST_RUN(
	 "libnsfdb_file_free",
	 nsfdb_test_file_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		NSFDB_TEST_RUN_WITH_ARGS(
		 "libnsfdb_file_open",
		 nsfdb_test_file_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		NSFDB_TEST_RUN_WITH_ARGS(
		 "libnsfdb_file_open_wide",
		 nsfdb_test_file_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBNSFDB_HAVE_BFIO )

		/* TODO add test for libnsfdb_file_open_file_io_handle */

#endif /* defined( LIBNSFDB_HAVE_BFIO ) */

		NSFDB_TEST_RUN(
		 "libnsfdb_file_close",
		 nsfdb_test_file_close );

		NSFDB_TEST_RUN_WITH_ARGS(
		 "libnsfdb_file_open_close",
		 nsfdb_test_file_open_close,
		 source );

		/* Initialize test
		 */
		result = nsfdb_test_file_open_source(
		          &file,
		          source,
		          &error );

		NSFDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        NSFDB_TEST_ASSERT_IS_NOT_NULL(
	         "file",
	         file );

	        NSFDB_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		NSFDB_TEST_RUN_WITH_ARGS(
		 "libnsfdb_file_signal_abort",
		 nsfdb_test_file_signal_abort,
		 file );

#if defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT )

		/* TODO: add tests for libnsfdb_file_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT ) */

		/* TODO: add tests for libnsfdb_file_get_number_of_notes */

		/* TODO: add tests for libnsfdb_file_get_note */

		/* TODO: add tests for libnsfdb_file_get_note_by_identifier */

		/* Clean up
		 */
		result = nsfdb_test_file_close_source(
		          &file,
		          &error );

		NSFDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		NSFDB_TEST_ASSERT_IS_NULL(
	         "file",
	         file );

	        NSFDB_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		nsfdb_test_file_close_source(
		 &file,
		 NULL );
	}
	return( EXIT_FAILURE );
}

