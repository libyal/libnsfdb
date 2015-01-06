/*
 * Unique Name Key (UNK) functions
 *
 * Copyright (C) 2010-2015, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libnsfdb_libcerror.h"
#include "libnsfdb_unique_name_key.h"

/* Creates an unique name key
 * Make sure the value unique_name_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_unique_name_key_initialize(
     libnsfdb_unique_name_key_t **unique_name_key,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_unique_name_key_initialize";

	if( unique_name_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unique name key.",
		 function );

		return( -1 );
	}
	if( *unique_name_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid unique name key value already set.",
		 function );

		return( -1 );
	}
	*unique_name_key = memory_allocate_structure(
	                    libnsfdb_unique_name_key_t );

	if( *unique_name_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create unique name key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *unique_name_key,
	     0,
	     sizeof( libnsfdb_unique_name_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear unique name key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *unique_name_key != NULL )
	{
		memory_free(
		 *unique_name_key );

		*unique_name_key = NULL;
	}
	return( -1 );
}

/* Frees an unique name key
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_unique_name_key_free(
     libnsfdb_unique_name_key_t **unique_name_key,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_unique_name_key_free";

	if( unique_name_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid unique name key.",
		 function );

		return( -1 );
	}
	if( *unique_name_key != NULL )
	{
		if( ( *unique_name_key )->name != NULL )
		{
			memory_free(
			 ( *unique_name_key )->name );
		}
		memory_free(
		 *unique_name_key );

		*unique_name_key = NULL;
	}
	return( 1 );
}

