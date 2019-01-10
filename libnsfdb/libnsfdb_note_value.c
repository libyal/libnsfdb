/*
 * Note value functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include "libnsfdb_note_value.h"

/* Creates a note value
 * Make sure the value note_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_value_initialize(
     libnsfdb_note_value_t **note_value,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_note_value_initialize";

	if( note_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note value.",
		 function );

		return( -1 );
	}
	if( *note_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid note value already set.",
		 function );

		return( -1 );
	}
	*note_value = memory_allocate_structure(
	               libnsfdb_note_value_t );

	if( *note_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create note value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *note_value,
	     0,
	     sizeof( libnsfdb_note_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear note value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *note_value != NULL )
	{
		memory_free(
		 *note_value );

		*note_value = NULL;
	}
	return( -1 );
}

/* Frees a note value
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_value_free(
     libnsfdb_note_value_t **note_value,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_note_value_free";

	if( note_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note value.",
		 function );

		return( -1 );
	}
	if( *note_value != NULL )
	{
		if( ( *note_value )->data != NULL )
		{
			memory_free(
			 ( *note_value )->data );
		}
		memory_free(
		 *note_value );

		*note_value = NULL;
	}
	return( 1 );
}

/* Sets the data in the note value
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_value_set_data(
     libnsfdb_note_value_t *note_value,
     uint8_t *data,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_note_value_set_data";

	if( note_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( note_value->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data type definition - data already set.",
		 function );

		return( -1 );
	}
	if( note_value->data_size > 0 )
	{
		note_value->data = (uint8_t *) memory_allocate(
		                                sizeof( uint8_t ) * note_value->data_size );

		if( note_value->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data type definition data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     note_value->data,
		     data,
		     sizeof( uint8_t ) * note_value->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( note_value->data != NULL )
	{
		memory_free(
		 note_value->data );

		note_value->data = NULL;
	}
	return( -1 );
}

