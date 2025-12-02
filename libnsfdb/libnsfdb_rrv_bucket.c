/*
 * Record Relocation Vector (RRV) bucket functions
 *
 * Copyright (C) 2010-2025, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libnsfdb_checksum.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"
#include "libnsfdb_rrv_bucket.h"
#include "libnsfdb_rrv_value.h"

#include "nsfdb_rrv_bucket.h"

/* Creates a RRV bucket
 * Make sure the value rrv_bucket is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_initialize(
     libnsfdb_rrv_bucket_t **rrv_bucket,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_bucket_initialize";

	if( rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket.",
		 function );

		return( -1 );
	}
	if( *rrv_bucket != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid RRV bucket value already set.",
		 function );

		return( -1 );
	}
	*rrv_bucket = memory_allocate_structure(
	               libnsfdb_rrv_bucket_t );

	if( *rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create RRV bucket.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *rrv_bucket,
	     0,
	     sizeof( libnsfdb_rrv_bucket_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear RRV bucket.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *rrv_bucket )->values ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *rrv_bucket != NULL )
	{
		memory_free(
		 *rrv_bucket );

		*rrv_bucket = NULL;
	}
	return( -1 );
}

/* Frees a RRV bucket
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_free(
     libnsfdb_rrv_bucket_t **rrv_bucket,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_bucket_free";
	int result            = 1;

	if( rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket.",
		 function );

		return( -1 );
	}
	if( *rrv_bucket != NULL )
	{
		if( libcdata_array_free(
		     &( ( *rrv_bucket )->values ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_rrv_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
		memory_free(
		 *rrv_bucket );

		*rrv_bucket = NULL;
	}
	return( result );
}

/* Reads the Record Relocation Vector (RRV) bucket
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_read(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     uint8_t *rrv_bucket_data,
     size_t rrv_bucket_data_size,
     libcerror_error_t **error )
{
	libnsfdb_rrv_value_t *rrv_value = NULL;
	uint8_t *rrv_entry_data         = NULL;
	static char *function           = "libnsfdb_rrv_bucket_read";
	uint32_t calculated_checksum    = 0;
	uint32_t rrv_entry              = 0;
	uint32_t rrv_entry_bsid         = 0;
	uint32_t rrv_entry_nonsum       = 0;
	uint32_t rrv_identifier         = 0;
	uint32_t stored_checksum        = 0;
	int rrv_value_index             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit            = 0;
#endif

	if( rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket.",
		 function );

		return( -1 );
	}
	if( rrv_bucket_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket data.",
		 function );

		return( -1 );
	}
	if( rrv_bucket_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid RRV bucket data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: RRV bucket header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) rrv_bucket_data,
		 sizeof( nsfdb_rrv_bucket_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->initial_rrv_identifier,
	 rrv_identifier );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->signature );

		libcnotify_printf(
		 "%s: header size\t\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->header_size );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->unknown1,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: initial RRV identifier\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 rrv_identifier );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->unknown2,
		 6,
		 0 );

		byte_stream_copy_to_uint16_little_endian(
		 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->unknown_size,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->unknown3,
		 10,
		 0 );
	}
#endif
	if( ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->signature != 0x06 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported RRV bucket signature.",
		 function );

		goto on_error;
	}
	if( ( (nsfdb_rrv_bucket_header_t *) rrv_bucket_data )->header_size != 0x20 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported RRV bucket header size.",
		 function );

		goto on_error;
	}
	rrv_entry_data        = rrv_bucket_data + sizeof( nsfdb_rrv_bucket_header_t );
	rrv_bucket_data_size -= sizeof( nsfdb_rrv_bucket_header_t );

	if( libnsfdb_checksum_calculate_little_endian_xor32(
	     &calculated_checksum,
	     rrv_entry_data,
	     rrv_bucket_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to calculate XOR-32 checksum.",
		 function );

		goto on_error;
	}
	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in RRV bucket checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
		 function,
		 stored_checksum,
		 calculated_checksum );

#ifndef TODO
		if( ( error != NULL )
		 && ( *error != NULL ) )
		{
			libcnotify_print_error_backtrace(
			 *error );
		}
		libcerror_error_free(
		 error );
#else
		goto on_error;
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: RRV entry data:\n",
		 function );
		libcnotify_print_data(
		 rrv_entry_data,
		 rrv_bucket_data_size,
		 0 );
	}
#endif
	while( rrv_bucket_data_size > 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 rrv_entry_data,
		 rrv_entry );

		rrv_entry_data       += 4;
		rrv_bucket_data_size -= 4;

#ifdef FORMAT_0x15
		if( format_version <= 0x15 )
		{
			if( ( rrv_entry != 0 )
			 && ( rrv_entry != 0xffffffffUL ) )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: RRV entry: 0x%08" PRIx32 " file position: 0x%08" PRIx32 " (offset: 0x%06" PRIx32 "00)\n",
					 function,
					 rrv_identifier,
					 rrv_entry,
					 rrv_entry );
				}
#endif
				if( libnsfdb_rrv_value_initialize(
				     &rrv_value,
				     rrv_identifier,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create RRV value.",
					 function );

					return( -1 );
				}
				if( libnsfdb_rrv_value_set_file_position(
				     rrv_value,
				     rrv_entry,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set file position in RRV value: 0x%08" PRIx32 ".",
					 function,
					 rrv_identifier );

					goto on_error;
				}
				if( libcdata_array_append_entry(
				     rrv_bucket->values,
				     &rrv_value_index,
				     (intptr_t *) rrv_value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append RRV value: 0x%08" PRIx32 ".",
					 function,
					 rrv_identifier );

					goto on_error;
				}
				rrv_value = NULL;
			}
		}
		else
#endif
		{
			byte_stream_copy_to_uint32_little_endian(
			 rrv_entry_data,
			 rrv_entry_bsid );

			rrv_entry_data       += 4;
			rrv_bucket_data_size -= 4;

			if( ( rrv_entry & 0x80000000UL ) == 0 )
			{
				if( ( rrv_entry != 0 )
				 && ( rrv_entry != 0x7fffffffUL ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: RRV entry: 0x%08" PRIx32 " file position: 0x%08" PRIx32 " (offset: 0x%06" PRIx32 "00)\n",
						 function,
						 rrv_identifier,
						 rrv_entry,
						 rrv_entry );
					}
#endif
/* TODO
					if( libnsfdb_rrv_value_initialize(
					     &rrv_value,
					     rrv_identifier,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create RRV value.",
						 function );

						goto on_error;
					}
					if( libnsfdb_rrv_value_set_file_position(
					     rrv_value,
					     rrv_entry,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set file position in RRV value: 0x%08" PRIx32 ".",
						 function,
						 rrv_identifier );

						goto on_error;
					}
					if( libcdata_array_append_entry(
					     rrv_bucket->values,
					     &rrv_value_index,
					     (intptr_t *) rrv_value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append RRV value: 0x%08" PRIx32 ".",
						 function,
						 rrv_identifier );

						goto on_error;
					}
					rrv_value = NULL;
*/
				}
			}
			else
			{
				rrv_entry_nonsum = ( rrv_entry >> 0x07 ) & 0x00e00000UL;

				rrv_entry &= 0x00ffffffUL;

				if( ( rrv_entry != 0 )
				 && ( rrv_entry != 0x00ffffffUL ) )
				{
					rrv_entry_nonsum |= rrv_entry_bsid >> 0x0b;

					rrv_entry_bsid &= 0x000007ffUL;

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: RRV entry: 0x%08" PRIx32 " BSID: 0x%06" PRIx32 ":0x%03" PRIx32 " NONSUM: 0x%06" PRIx32 "\n",
						 function,
						 rrv_identifier,
						 rrv_entry,
						 rrv_entry_bsid,
						 rrv_entry_nonsum );
					}
#endif
					if( libnsfdb_rrv_value_initialize(
					     &rrv_value,
					     rrv_identifier,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
						 "%s: unable to create RRV value.",
						 function );

						goto on_error;
					}
					if( libnsfdb_rrv_value_set_bucket_slot_identifier(
					     rrv_value,
					     rrv_entry,
					     (uint16_t) rrv_entry_bsid,
					     rrv_entry_nonsum,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set BSID in RRV value: 0x%08" PRIx32 ".",
						 function,
						 rrv_identifier );

						goto on_error;
					}
					if( libcdata_array_append_entry(
					     rrv_bucket->values,
					     &rrv_value_index,
					     (intptr_t *) rrv_value,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
						 "%s: unable to append RRV value: 0x%08" PRIx32 ".",
						 function,
						 rrv_identifier );

						goto on_error;
					}
					rrv_value = NULL;
				}
			}
		}
		rrv_identifier += 4;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( rrv_value != NULL )
	{
		libnsfdb_rrv_value_free(
		 &rrv_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of RRV values
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_get_number_of_values(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     int *number_of_rrv_values,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_bucket_get_number_of_values";

	if( rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     rrv_bucket->values,
	     number_of_rrv_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of RRV values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific RRV value
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_rrv_bucket_get_value_by_index(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     int rrv_value_index,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_rrv_bucket_get_value_by_index";

	if( rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     rrv_bucket->values,
	     rrv_value_index,
	     (intptr_t **) rrv_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve RRV value: %d.",
		 function,
		 rrv_value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a RRV value for a specific identifier
 * Returns 1 if successful, 0 if no RRV value was found or -1 on error
 */
int libnsfdb_rrv_bucket_get_value_by_identifier(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     uint32_t rrv_identifier,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error )
{
	static char *function    = "libnsfdb_rrv_bucket_get_value_by_identifier";
	int number_of_rrv_values = 0;
	int rrv_value_index      = 0;

	if( rrv_bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid RRV bucket.",
		 function );

		return( -1 );
	}
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
	if( libcdata_array_get_number_of_entries(
	     rrv_bucket->values,
	     &number_of_rrv_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of RRV values.",
		 function );

		return( -1 );
	}
	for( rrv_value_index = 0;
	     rrv_value_index < number_of_rrv_values;
	     rrv_value_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     rrv_bucket->values,
		     rrv_value_index,
		     (intptr_t **) rrv_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV value: %d.",
			 function,
			 rrv_value_index );

			return( -1 );
		}
		if( *rrv_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing RRV value: %d.",
			 function,
			 rrv_value_index );

			return( -1 );
		}
		if( rrv_identifier == ( *rrv_value )->rrv_identifier )
		{
			return( 1 );
		}
	}
	*rrv_value = NULL;

	return( 0 );
}

