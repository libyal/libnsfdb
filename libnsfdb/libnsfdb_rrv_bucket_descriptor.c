/*
 * Record Relocation Vector (RRV) bucket decriptor functions
 *
 * Copyright (C) 2010-2020, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libnsfdb_libcerror.h"
#include "libnsfdb_rrv_bucket_descriptor.h"

/* Creates a RRV bucket descriptor
 * Make sure the value rrv_bucket_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_descriptor_initialize(
     libnsfdb_rrv_bucket_descriptor_t **rrv_bucket_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_bucket_descriptor_initialize";

	if( rrv_bucket_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket descriptor.",
		 function );

		return( -1 );
	}
	if( *rrv_bucket_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid RRV bucket descriptor value already set.",
		 function );

		return( -1 );
	}
	*rrv_bucket_descriptor = memory_allocate_structure(
	                          libnsfdb_rrv_bucket_descriptor_t );

	if( *rrv_bucket_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create RRV bucket descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *rrv_bucket_descriptor,
	     0,
	     sizeof( libnsfdb_rrv_bucket_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear RRV bucket descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *rrv_bucket_descriptor != NULL )
	{
		memory_free(
		 *rrv_bucket_descriptor );

		*rrv_bucket_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a RRV bucket descriptor
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_descriptor_free(
     libnsfdb_rrv_bucket_descriptor_t **rrv_bucket_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_bucket_descriptor_free";

	if( rrv_bucket_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket descriptor.",
		 function );

		return( -1 );
	}
	if( *rrv_bucket_descriptor != NULL )
	{
		memory_free(
		 *rrv_bucket_descriptor );

		*rrv_bucket_descriptor = NULL;
	}
	return( 1 );
}

