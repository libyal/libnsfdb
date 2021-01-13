/*
 * Library unique_name_key type test program
 *
 * Copyright (C) 2010-2021, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "nsfdb_test_libcerror.h"
#include "nsfdb_test_libnsfdb.h"
#include "nsfdb_test_macros.h"
#include "nsfdb_test_memory.h"
#include "nsfdb_test_unused.h"

#include "../libnsfdb/libnsfdb_unique_name_key.h"

#if defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT )

/* Tests the libnsfdb_unique_name_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_unique_name_key_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libnsfdb_unique_name_key_t *unique_name_key = NULL;
	int result                                  = 0;

#if defined( HAVE_NSFDB_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libnsfdb_unique_name_key_initialize(
	          &unique_name_key,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "unique_name_key",
	 unique_name_key );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_unique_name_key_free(
	          &unique_name_key,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "unique_name_key",
	 unique_name_key );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libnsfdb_unique_name_key_initialize(
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

	unique_name_key = (libnsfdb_unique_name_key_t *) 0x12345678UL;

	result = libnsfdb_unique_name_key_initialize(
	          &unique_name_key,
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

	unique_name_key = NULL;

#if defined( HAVE_NSFDB_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libnsfdb_unique_name_key_initialize with malloc failing
		 */
		nsfdb_test_malloc_attempts_before_fail = test_number;

		result = libnsfdb_unique_name_key_initialize(
		          &unique_name_key,
		          &error );

		if( nsfdb_test_malloc_attempts_before_fail != -1 )
		{
			nsfdb_test_malloc_attempts_before_fail = -1;

			if( unique_name_key != NULL )
			{
				libnsfdb_unique_name_key_free(
				 &unique_name_key,
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
			 "unique_name_key",
			 unique_name_key );

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
		/* Test libnsfdb_unique_name_key_initialize with memset failing
		 */
		nsfdb_test_memset_attempts_before_fail = test_number;

		result = libnsfdb_unique_name_key_initialize(
		          &unique_name_key,
		          &error );

		if( nsfdb_test_memset_attempts_before_fail != -1 )
		{
			nsfdb_test_memset_attempts_before_fail = -1;

			if( unique_name_key != NULL )
			{
				libnsfdb_unique_name_key_free(
				 &unique_name_key,
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
			 "unique_name_key",
			 unique_name_key );

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
	if( unique_name_key != NULL )
	{
		libnsfdb_unique_name_key_free(
		 &unique_name_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libnsfdb_unique_name_key_free function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_unique_name_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libnsfdb_unique_name_key_free(
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

#endif /* defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc NSFDB_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] NSFDB_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc NSFDB_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] NSFDB_TEST_ATTRIBUTE_UNUSED )
#endif
{
	NSFDB_TEST_UNREFERENCED_PARAMETER( argc )
	NSFDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT )

	NSFDB_TEST_RUN(
	 "libnsfdb_unique_name_key_initialize",
	 nsfdb_test_unique_name_key_initialize );

	NSFDB_TEST_RUN(
	 "libnsfdb_unique_name_key_free",
	 nsfdb_test_unique_name_key_free );

#endif /* defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

