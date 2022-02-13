/*
 * Library rrv_bucket type test program
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libnsfdb/libnsfdb_rrv_bucket.h"

#if defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT )

/* Tests the libnsfdb_rrv_bucket_initialize function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_rrv_bucket_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libnsfdb_rrv_bucket_t *rrv_bucket = NULL;
	int result                        = 0;

#if defined( HAVE_NSFDB_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 1;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test regular cases
	 */
	result = libnsfdb_rrv_bucket_initialize(
	          &rrv_bucket,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "rrv_bucket",
	 rrv_bucket );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libnsfdb_rrv_bucket_free(
	          &rrv_bucket,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "rrv_bucket",
	 rrv_bucket );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libnsfdb_rrv_bucket_initialize(
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

	rrv_bucket = (libnsfdb_rrv_bucket_t *) 0x12345678UL;

	result = libnsfdb_rrv_bucket_initialize(
	          &rrv_bucket,
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

	rrv_bucket = NULL;

#if defined( HAVE_NSFDB_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libnsfdb_rrv_bucket_initialize with malloc failing
		 */
		nsfdb_test_malloc_attempts_before_fail = test_number;

		result = libnsfdb_rrv_bucket_initialize(
		          &rrv_bucket,
		          &error );

		if( nsfdb_test_malloc_attempts_before_fail != -1 )
		{
			nsfdb_test_malloc_attempts_before_fail = -1;

			if( rrv_bucket != NULL )
			{
				libnsfdb_rrv_bucket_free(
				 &rrv_bucket,
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
			 "rrv_bucket",
			 rrv_bucket );

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
		/* Test libnsfdb_rrv_bucket_initialize with memset failing
		 */
		nsfdb_test_memset_attempts_before_fail = test_number;

		result = libnsfdb_rrv_bucket_initialize(
		          &rrv_bucket,
		          &error );

		if( nsfdb_test_memset_attempts_before_fail != -1 )
		{
			nsfdb_test_memset_attempts_before_fail = -1;

			if( rrv_bucket != NULL )
			{
				libnsfdb_rrv_bucket_free(
				 &rrv_bucket,
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
			 "rrv_bucket",
			 rrv_bucket );

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
	if( rrv_bucket != NULL )
	{
		libnsfdb_rrv_bucket_free(
		 &rrv_bucket,
		 NULL );
	}
	return( 0 );
}

/* Tests the libnsfdb_rrv_bucket_free function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_rrv_bucket_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libnsfdb_rrv_bucket_free(
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

/* Tests the libnsfdb_rrv_bucket_get_number_of_values function
 * Returns 1 if successful or 0 if not
 */
int nsfdb_test_rrv_bucket_get_number_of_values(
     void )
{
	libcerror_error_t *error          = NULL;
	libnsfdb_rrv_bucket_t *rrv_bucket = NULL;
	int number_of_values              = 0;
	int number_of_values_is_set       = 0;
	int result                        = 0;

	/* Initialize test
	 */
	result = libnsfdb_rrv_bucket_initialize(
	          &rrv_bucket,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NOT_NULL(
	 "rrv_bucket",
	 rrv_bucket );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libnsfdb_rrv_bucket_get_number_of_values(
	          rrv_bucket,
	          &number_of_values,
	          &error );

	NSFDB_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_values_is_set = result;

	/* Test error cases
	 */
	result = libnsfdb_rrv_bucket_get_number_of_values(
	          NULL,
	          &number_of_values,
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

	if( number_of_values_is_set != 0 )
	{
		result = libnsfdb_rrv_bucket_get_number_of_values(
		          rrv_bucket,
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
	}
	/* Clean up
	 */
	result = libnsfdb_rrv_bucket_free(
	          &rrv_bucket,
	          &error );

	NSFDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	NSFDB_TEST_ASSERT_IS_NULL(
	 "rrv_bucket",
	 rrv_bucket );

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
	if( rrv_bucket != NULL )
	{
		libnsfdb_rrv_bucket_free(
		 &rrv_bucket,
		 NULL );
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
	 "libnsfdb_rrv_bucket_initialize",
	 nsfdb_test_rrv_bucket_initialize );

	NSFDB_TEST_RUN(
	 "libnsfdb_rrv_bucket_free",
	 nsfdb_test_rrv_bucket_free );

	/* TODO: add tests for libnsfdb_rrv_bucket_read */

	NSFDB_TEST_RUN(
	 "libnsfdb_rrv_bucket_get_number_of_values",
	 nsfdb_test_rrv_bucket_get_number_of_values );

	/* TODO: add tests for libnsfdb_rrv_bucket_get_value_by_index */

	/* TODO: add tests for libnsfdb_rrv_bucket_get_value_by_identifier */

#endif /* defined( __GNUC__ ) && !defined( LIBNSFDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

