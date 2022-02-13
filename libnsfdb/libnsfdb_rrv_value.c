/*
 * Record Relocation Vector (RRV) value functions
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
#include <memory.h>
#include <types.h>

#include "libnsfdb_libcerror.h"
#include "libnsfdb_rrv_value.h"

/* Creates a RRV value
 * Make sure the value rrv_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_value_initialize(
     libnsfdb_rrv_value_t **rrv_value,
     uint32_t rrv_identifier,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_value_initialize";

	if( rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV value.",
		 function );

		return( -1 );
	}
	if( *rrv_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid RRV value value already set.",
		 function );

		return( -1 );
	}
	*rrv_value = memory_allocate_structure(
	              libnsfdb_rrv_value_t );

	if( *rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create RRV value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *rrv_value,
	     0,
	     sizeof( libnsfdb_rrv_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear RRV value.",
		 function );

		goto on_error;
	}
	( *rrv_value )->rrv_identifier = rrv_identifier;

	return( 1 );

on_error:
	if( *rrv_value != NULL )
	{
		memory_free(
		 *rrv_value );

		*rrv_value = NULL;
	}
	return( -1 );
}

/* Frees a RRV value
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_value_free(
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_value_free";

	if( rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV value.",
		 function );

		return( -1 );
	}
	if( *rrv_value != NULL )
	{
		memory_free(
		 *rrv_value );

		*rrv_value = NULL;
	}
	return( 1 );
}

/* Clones the RRV value
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_value_clone(
     libnsfdb_rrv_value_t **destination_rrv_value,
     libnsfdb_rrv_value_t *source_rrv_value,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_value_clone";

	if( destination_rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination RRV value.",
		 function );

		return( -1 );
	}
	if( *destination_rrv_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination RRV value already set.",
		 function );

		return( -1 );
	}
	if( source_rrv_value == NULL )
	{
		*destination_rrv_value = NULL;

		return( 1 );
	}
	*destination_rrv_value = memory_allocate_structure(
	                          libnsfdb_rrv_value_t );

	if( *destination_rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create item destination descriptor.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_rrv_value,
	     source_rrv_value,
	     sizeof( libnsfdb_rrv_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy RRV value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *destination_rrv_value != NULL )
	{
		memory_free(
		 *destination_rrv_value );

		*destination_rrv_value = NULL;
	}
	return( -1 );
}

/* Sets the bucket slot identifier (BSID)
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_value_set_bucket_slot_identifier(
     libnsfdb_rrv_value_t *rrv_value,
     uint32_t bucket_index,
     uint16_t slot_index,
     uint32_t nonsum,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_value_set_bucket_slot_identifier";

	if( rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV value.",
		 function );

		return( -1 );
	}
	rrv_value->type         = LIBNSFDB_RRV_VALUE_TYPE_BUCKET_SLOT_IDENTIFIER;
	rrv_value->bucket_index = bucket_index;
	rrv_value->slot_index   = slot_index;
	rrv_value->nonsum       = nonsum;

	return( 1 );
}

/* Sets the file position
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_value_set_file_position(
     libnsfdb_rrv_value_t *rrv_value,
     uint32_t file_position,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_value_set_file_position";

	if( rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV value.",
		 function );

		return( -1 );
	}
	rrv_value->type        = LIBNSFDB_RRV_VALUE_TYPE_FILE_POSITION;
	rrv_value->file_offset = file_position << 8;

	return( 1 );
}

