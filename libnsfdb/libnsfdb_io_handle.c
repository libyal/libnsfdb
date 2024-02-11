/*
 * Input/Output (IO) handle
 *
 * Copyright (C) 2010-2024, Joachim Metz <joachim.metz@gmail.com>
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

#include "libnsfdb_bucket.h"
#include "libnsfdb_checksum.h"
#include "libnsfdb_compression.h"
#include "libnsfdb_debug.h"
#include "libnsfdb_definitions.h"
#include "libnsfdb_io_handle.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"
#include "libnsfdb_libfdatetime.h"
#include "libnsfdb_rrv_bucket.h"
#include "libnsfdb_rrv_bucket_descriptor.h"
#include "libnsfdb_unique_name_key.h"
#include "libnsfdb_unique_name_key_table.h"
#include "libnsfdb_unused.h"

#include "nsfdb_bucket.h"
#include "nsfdb_bucket_descriptor_block.h"
#include "nsfdb_database_header.h"
#include "nsfdb_file_header.h"
#include "nsfdb_superblock.h"

const char nsfdb_file_signature[ 2 ] = { 0x1a, 0x00 };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_initialize(
     libnsfdb_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libnsfdb_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libnsfdb_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 *io_handle );

		*io_handle = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *io_handle )->rrv_bucket_descriptors ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create RRV bucket descriptors array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *io_handle )->unique_name_key_table ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create unique name key table.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		if( ( *io_handle )->rrv_bucket_descriptors != NULL )
		{
			libcdata_array_free(
			 &( ( *io_handle )->rrv_bucket_descriptors ),
			 NULL,
			 NULL );
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_free(
     libnsfdb_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( libcdata_array_free(
		     &( ( *io_handle )->rrv_bucket_descriptors ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_rrv_bucket_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free RRV bucket descriptors array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *io_handle )->unique_name_key_table ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_unique_name_key_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free unique name key table.",
			 function );

			result = -1;
		}
		if( ( *io_handle )->rrv_bucket_vector != NULL )
		{
			if( libfdata_vector_free(
			     &( ( *io_handle )->rrv_bucket_vector ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free RRV bucket vector.",
				 function );

				result = -1;
			}
		}
		if( ( *io_handle )->rrv_bucket_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *io_handle )->rrv_bucket_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free RRV bucket cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_clear(
     libnsfdb_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_io_handle_clear";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcdata_array_empty(
	     io_handle->rrv_bucket_descriptors,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_rrv_bucket_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty RRV bucket descriptors array.",
		 function );

		result = -1;
	}
	if( libcdata_array_empty(
	     io_handle->unique_name_key_table,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_unique_name_key_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty unique name key table.",
		 function );

		result = -1;
	}
	if( io_handle->rrv_bucket_vector != NULL )
	{
		if( libfdata_vector_empty(
		     io_handle->rrv_bucket_vector,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free RRV bucket vector.",
			 function );

			result = -1;
		}
	}
	if( io_handle->rrv_bucket_cache != NULL )
	{
		if( libfcache_cache_empty(
		     io_handle->rrv_bucket_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty RRV bucket cache.",
			 function );

			result = -1;
		}
	}
        io_handle->format_version                  = 0;
        io_handle->rrv_bucket_size                 = 0;
        io_handle->summary_bucket_size             = 0;
        io_handle->minimum_summary_bucket_size     = 0;
        io_handle->maximum_summary_bucket_size     = 0;
        io_handle->minimum_non_summary_bucket_size = 0;
        io_handle->maximum_non_summary_bucket_size = 0;
        io_handle->file_size                       = 0;

/* TODO refactor IO handle
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libnsfdb_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
*/
	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_read_file_header(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *database_header_size,
     libcerror_error_t **error )
{
	nsfdb_file_header_t file_header;

	static char *function = "libnsfdb_io_handle_read_file_header";
	ssize_t read_count    = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( database_header_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database header size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif

	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &file_header,
	              sizeof( nsfdb_file_header_t ),
	              0,
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header at offset: 0 (0x00000000).",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &file_header,
		 sizeof( nsfdb_file_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     file_header.signature,
	     nsfdb_file_signature,
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 file_header.database_header_size,
	 *database_header_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t\t: 0x%02x 0x%02x\n",
		 function,
		 file_header.signature[ 0 ],
		 file_header.signature[ 1 ] );

		libcnotify_printf(
		 "%s: database header size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *database_header_size );
	}
#endif
	return( 1 );
}

/* Reads the database header
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_read_database_header(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t database_header_size,
     off64_t *superblock1_offset,
     uint32_t *superblock1_size,
     off64_t *superblock2_offset,
     uint32_t *superblock2_size,
     off64_t *superblock3_offset,
     uint32_t *superblock3_size,
     off64_t *superblock4_offset,
     uint32_t *superblock4_size,
     off64_t *bucket_descriptor_block1_offset,
     uint32_t *bucket_descriptor_block1_size,
     off64_t *bucket_descriptor_block2_offset,
     uint32_t *bucket_descriptor_block2_size,
     off64_t *non_data_rrv_bucket_offset,
     off64_t *data_rrv_bucket_offset,
     libcerror_error_t **error )
{
	nsfdb_database_information_t database_information;
	nsfdb_database_replication_information_t database_replication_information;
	nsfdb_database_header_t database_header;
	nsfdb_database_information2_t database_information2;
	nsfdb_database_information3_t database_information3;
	nsfdb_database_information4_t database_information4;

	static char *function                     = "libnsfdb_io_handle_read_database_header";
	ssize_t read_count                        = 0;
	int segment_index                         = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t nsf_timedate_string[ 32 ];

	libfdatetime_nsf_timedate_t *nsf_timedate = NULL;
	uint64_t value_64bit                      = 0;
	uint32_t value_32bit                      = 0;
	uint16_t value_16bit                      = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( superblock1_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 1 offset.",
		 function );

		return( -1 );
	}
	if( superblock1_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 1 size.",
		 function );

		return( -1 );
	}
	if( superblock2_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 2 offset.",
		 function );

		return( -1 );
	}
	if( superblock2_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 2 size.",
		 function );

		return( -1 );
	}
	if( superblock3_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 3 offset.",
		 function );

		return( -1 );
	}
	if( superblock3_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 3 size.",
		 function );

		return( -1 );
	}
	if( superblock4_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 4 offset.",
		 function );

		return( -1 );
	}
	if( superblock4_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid superblock 4 size.",
		 function );

		return( -1 );
	}
	if( bucket_descriptor_block1_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket descriptor block 1 offset.",
		 function );

		return( -1 );
	}
	if( bucket_descriptor_block1_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket descriptor block 1 size.",
		 function );

		return( -1 );
	}
	if( bucket_descriptor_block2_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket descriptor block 2 offset.",
		 function );

		return( -1 );
	}
	if( bucket_descriptor_block2_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket descriptor block 2 size.",
		 function );

		return( -1 );
	}
	if( non_data_rrv_bucket_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid non-data RRV bucket offset.",
		 function );

		return( -1 );
	}
	if( data_rrv_bucket_offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data RRV bucket offset.",
		 function );

		return( -1 );
	}
	if( database_header_size != 1024 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported database header size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading database header at offset: 6 (0x00000006)\n",
		 function );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &database_information,
	              sizeof( nsfdb_database_information_t ),
	              6,
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_database_information_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database information at offset: 6 (0x00000006).",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database information:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &database_information,
		 sizeof( nsfdb_database_information_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 database_information.format_version,
	 io_handle->format_version );

	byte_stream_copy_to_uint32_little_endian(
	 database_information.non_data_rrv_bucket_position,
	 *non_data_rrv_bucket_offset );

	byte_stream_copy_to_uint32_little_endian(
	 database_information.data_rrv_bucket_position,
	 *data_rrv_bucket_offset );

	byte_stream_copy_to_uint16_little_endian(
	 database_information.rrv_bucket_size,
	 io_handle->rrv_bucket_size );

	byte_stream_copy_to_uint16_little_endian(
	 database_information.summary_bucket_size,
	 io_handle->summary_bucket_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information.file_size,
	 io_handle->file_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: format version\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->format_version );

		byte_stream_copy_to_uint64_little_endian(
		 database_information.database_identifier,
		 value_64bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.database_identifier,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: database identifier\t\t\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%s)\n",
		 function,
		 value_64bit >> 32,
		 value_64bit & 0xffffffffUL,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.application_version,
		 value_16bit );

		libcnotify_printf(
		 "%s: application version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: non-data RRV bucket position\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *non_data_rrv_bucket_offset,
		 *non_data_rrv_bucket_offset );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.available_non_data_rrv_identifier,
		 value_32bit );

		libcnotify_printf(
		 "%s: available non-data RRV identifier\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.number_of_available_non_data_rrvs,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of available non-data RRVs\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.activity_log_offset,
		 value_32bit );

		libcnotify_printf(
		 "%s: activity log offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.bucket_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: bucket modification time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.database_class,
		 value_16bit );

		libcnotify_printf(
		 "%s: database class\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.database_flags,
		 value_16bit );

		libcnotify_printf(
		 "%s: database flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
		libnsfdb_debug_print_database_flags(
		 value_16bit );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.bucket_descriptor_block_size,
		 value_32bit );

		libcnotify_printf(
		 "%s: bucket descriptor block size\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.bucket_descriptor_block_position,
		 value_32bit );

		libcnotify_printf(
		 "%s: bucket descriptor block position\t\t: 0x%08" PRIx32 " (offset: 0x%06" PRIx32 "00)\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.bdt_size,
		 value_16bit );

		libcnotify_printf(
		 "%s: BDT size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.bdt_position,
		 value_32bit );

		libcnotify_printf(
		 "%s: BDT position\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.bdt_bitmaps,
		 value_16bit );

		libcnotify_printf(
		 "%s: BDT bitmaps\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: data RRV bucket position\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *data_rrv_bucket_offset,
		 *data_rrv_bucket_offset );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.first_data_rrv_identifier,
		 value_32bit );

		libcnotify_printf(
		 "%s: first data RRV identifier\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.available_data_rrv_identifier,
		 value_32bit );

		libcnotify_printf(
		 "%s: available data RRV identifier\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.number_of_available_data_rrvs,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of available data RRVs\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: RRV bucket size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->rrv_bucket_size );

		libcnotify_printf(
		 "%s: summary bucket size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->summary_bucket_size );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.bitmap_size,
		 value_16bit );

		libcnotify_printf(
		 "%s: bitmap size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.allocation_granularity,
		 value_16bit );

		libcnotify_printf(
		 "%s: allocation granularity\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.extention_granularity,
		 value_32bit );

		libcnotify_printf(
		 "%s: extention granularity\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: file size\t\t\t\t\t: %" PRIu64 " (%" PRIu64 " bytes)\n",
		 function,
		 io_handle->file_size,
		 io_handle->file_size << 8 );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.number_of_file_truncations,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of file truncations\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.delivery_sequence_number,
		 value_32bit );

		libcnotify_printf(
		 "%s: delivery sequence number\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.number_of_bdb_replacements,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of BDB replacements\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.number_of_allocated_rrvs,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of allocated RRVs\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.number_of_deallocations,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of de-allocations\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.number_of_non_bucket_allocations,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of non-bucket allocations\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.number_of_bucket_allocations,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of bucket allocations\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.folder_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: folder modification time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.data_note_identifier_table_position,
		 value_32bit );

		libcnotify_printf(
		 "%s: data note identifier table position\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.data_note_identifier_table_size,
		 value_32bit );

		libcnotify_printf(
		 "%s: data note identifier table size\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.data_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: data modification time\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.next_purge_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: next purge time\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.named_object_hash_position_version3,
		 value_32bit );

		libcnotify_printf(
		 "%s: named object hash position (version 3)\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information.named_object_hash_position,
		 value_32bit );

		libcnotify_printf(
		 "%s: named object hash position\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.private_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: private modification time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.maximum_number_of_privates,
		 value_16bit );

		libcnotify_printf(
		 "%s: maximum number of privates\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information.named_object_hash_version,
		 value_16bit );

		libcnotify_printf(
		 "%s: named object hash version\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information.non_data_modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: non-data modification time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &database_replication_information,
	              sizeof( nsfdb_database_replication_information_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_database_replication_information_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database replication information.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database replication information:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &database_replication_information,
		 sizeof( nsfdb_database_replication_information_t ),
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 database_replication_information.replication_identifier,
		 value_64bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_replication_information.replication_identifier,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: replication identifier\t\t\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%s)\n",
		 function,
		 value_64bit >> 32,
		 value_64bit & 0xffffffffUL,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint16_little_endian(
		 database_replication_information.replication_flags,
		 value_16bit );

		libcnotify_printf(
		 "%s: replication flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
		libnsfdb_debug_print_replication_flags(
		 value_16bit );
		libcnotify_printf(
		 "\n" );

		byte_stream_copy_to_uint16_little_endian(
		 database_replication_information.replication_cutoff_interval,
		 value_16bit );

		libcnotify_printf(
		 "%s: replication cutoff interval\t\t\t: %" PRIu16 " days\n",
		 function,
		 value_16bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_replication_information.replication_cutoff_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: replication cutoff time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &database_header,
	              sizeof( nsfdb_database_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_database_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &database_header,
		 sizeof( nsfdb_database_header_t ),
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database information buffer:\n",
		 function );
		libcnotify_print_data(
		 database_header.database_information_buffer,
		 128,
		 0 );

		libcnotify_printf(
		 "%s: special note identifiers:\n",
		 function );
		libcnotify_print_data(
		 database_header.special_note_identifiers,
		 128,
		 0 );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 database_header.unknown1,
		 64,
		 0 );
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &database_information2,
	              sizeof( nsfdb_database_information2_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_database_information2_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database information2.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database information2:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &database_information2,
		 sizeof( nsfdb_database_information2_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock1_position,
	 *superblock1_offset );
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock1_size,
	 *superblock1_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock2_position,
	 *superblock2_offset );
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock2_size,
	 *superblock2_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock3_position,
	 *superblock3_offset );
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock3_size,
	 *superblock3_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock4_position,
	 *superblock4_offset );
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.superblock4_size,
	 *superblock4_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.minimum_summary_bucket_size,
	 io_handle->minimum_summary_bucket_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.minimum_non_summary_bucket_size,
	 io_handle->minimum_non_summary_bucket_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.maximum_summary_bucket_size,
	 io_handle->maximum_summary_bucket_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.maximum_non_summary_bucket_size,
	 io_handle->maximum_non_summary_bucket_size );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.bucket_descriptor_block1_size,
	 *bucket_descriptor_block1_size );
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.bucket_descriptor_block1_position,
	 *bucket_descriptor_block1_offset );

	byte_stream_copy_to_uint32_little_endian(
	 database_information2.bucket_descriptor_block2_size,
	 *bucket_descriptor_block2_size );
	byte_stream_copy_to_uint32_little_endian(
	 database_information2.bucket_descriptor_block2_position,
	 *bucket_descriptor_block2_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information2.last_fixup_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: last fix-up time\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint32_little_endian(
		 database_information2.database_quota_limit,
		 value_32bit );

		libcnotify_printf(
		 "%s: database quota limit\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 database_information2.database_quota_warn_threshold,
		 value_32bit );

		libcnotify_printf(
		 "%s: database quota warn threshold\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information2.unknown_time1,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown time1\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information2.unknown_time2,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown time2\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint64_little_endian(
		 database_information2.object_store_replica_identifier,
		 value_64bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information2.object_store_replica_identifier,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: object store replica identifier\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%s)\n",
		 function,
		 value_64bit >> 32,
		 value_64bit & 0xffffffffUL,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: superblock 1 position\t\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *superblock1_offset,
		 *superblock1_offset );
		libcnotify_printf(
		 "%s: superblock 1 size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *superblock1_size );

		libcnotify_printf(
		 "%s: superblock 2 position\t\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *superblock2_offset,
		 *superblock2_offset );
		libcnotify_printf(
		 "%s: superblock 2 size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *superblock2_size );

		libcnotify_printf(
		 "%s: superblock 3 position\t\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *superblock3_offset,
		 *superblock3_offset );
		libcnotify_printf(
		 "%s: superblock 3 size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *superblock3_size );

		libcnotify_printf(
		 "%s: superblock 4 position\t\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *superblock4_offset,
		 *superblock4_offset );
		libcnotify_printf(
		 "%s: superblock 4 size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 *superblock4_size );

		byte_stream_copy_to_uint32_little_endian(
		 database_information2.maximum_extension_granularity,
		 value_32bit );

		libcnotify_printf(
		 "%s: maximum extension granularity\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information2.summary_bucket_granularity,
		 value_16bit );

		libcnotify_printf(
		 "%s: summary bucket granularity\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information2.non_summary_bucket_granularity,
		 value_16bit );

		libcnotify_printf(
		 "%s: non-summary bucket granularity\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: minimum summary bucket size\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->minimum_summary_bucket_size );

		libcnotify_printf(
		 "%s: minimum non-summary bucket size\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->minimum_non_summary_bucket_size );

		libcnotify_printf(
		 "%s: maximum summary bucket size\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->maximum_summary_bucket_size );

		libcnotify_printf(
		 "%s: maximum non-summary bucket size\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->maximum_non_summary_bucket_size );

		byte_stream_copy_to_uint16_little_endian(
		 database_information2.non_summary_append_size,
		 value_16bit );

		libcnotify_printf(
		 "%s: non-summary append size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information2.non_summary_append_factor,
		 value_16bit );

		libcnotify_printf(
		 "%s: non-summary append factor\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information2.summary_bucket_fill_factor,
		 value_16bit );

		libcnotify_printf(
		 "%s: summary bucket fill factor\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 database_information2.non_summary_bucket_fill_factor,
		 value_16bit );

		libcnotify_printf(
		 "%s: non-summary bucket fill factor\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: bucket descriptor block 1 size\t\t\t: %" PRIu32 "\n",
		 function,
		 *bucket_descriptor_block1_size );
		libcnotify_printf(
		 "%s: bucket descriptor block 1 position\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *bucket_descriptor_block1_offset,
		 *bucket_descriptor_block1_offset );

		libcnotify_printf(
		 "%s: bucket descriptor block 2 size\t\t\t: %" PRIu32 "\n",
		 function,
		 *bucket_descriptor_block2_size );
		libcnotify_printf(
		 "%s: bucket descriptor block 2 position\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
		 function,
		 *bucket_descriptor_block2_offset,
		 *bucket_descriptor_block2_offset );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 database_information2.unknown2,
		 4,
		 0 );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &database_information3,
	              sizeof( nsfdb_database_information3_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_database_information3_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database information3.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database information3:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &database_information3,
		 sizeof( nsfdb_database_information3_t ),
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 database_information3.unknown3,
		 44,
		 0 );
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &database_information4,
	              sizeof( nsfdb_database_information4_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_database_information4_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database information4.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: database information4:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &database_information4,
		 sizeof( nsfdb_database_information4_t ),
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown9:\n",
		 function );
		libcnotify_print_data(
		 database_information4.unknown9,
		 96,
		 0 );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information4.unknown_time7,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown time7\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: unknown10:\n",
		 function );
		libcnotify_print_data(
		 database_information4.unknown10,
		 34,
		 0 );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information4.unknown_time8,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown time8\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information4.unknown_time9,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown time9\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: unknown11:\n",
		 function );
		libcnotify_print_data(
		 database_information4.unknown11,
		 24,
		 0 );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     database_information4.unknown_time10,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown time10\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: unknown12:\n",
		 function );
		libcnotify_print_data(
		 database_information4.unknown12,
		 150,
		 0 );

		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif

	/* Change the file positions into offsets
	 */
	*non_data_rrv_bucket_offset      <<= 8;
	*data_rrv_bucket_offset          <<= 8;
	*superblock1_offset              <<= 8;
	*superblock2_offset              <<= 8;
	*superblock3_offset              <<= 8;
	*superblock4_offset              <<= 8;
	*bucket_descriptor_block1_offset <<= 8;
	*bucket_descriptor_block2_offset <<= 8;

	if( io_handle->file_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing file size.",
		 function );

		goto on_error;
	}
	io_handle->file_size <<= 8;

	/* TODO if file size is missing use libbfio to obtain it */

	if( io_handle->rrv_bucket_vector == NULL )
	{
		/* TODO free and clone function ? */

		if( libfdata_vector_initialize(
		     &( io_handle->rrv_bucket_vector ),
		     io_handle->rrv_bucket_size,
		     (intptr_t *) io_handle,
		     NULL,
		     NULL,
		     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libnsfdb_io_handle_read_rrv_bucket,
		     NULL,
		     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create RRV bucket vector.",
			 function );

			goto on_error;
		}
		if( libfdata_vector_append_segment(
		     io_handle->rrv_bucket_vector,
		     &segment_index,
		     0,
		     0,
		     io_handle->file_size,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append segment ot RRV bucket vector.",
			 function );

			goto on_error;
		}
	}
	if( io_handle->rrv_bucket_cache == NULL )
	{
		if( libfcache_cache_initialize(
		     &( io_handle->rrv_bucket_cache ),
		     LIBNSFDB_MAXIMUM_CACHE_ENTRIES_RRV_BUCKETS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create RRV bucket cache.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( nsf_timedate != NULL )
	{
		libfdatetime_nsf_timedate_free(
		 &nsf_timedate,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the superblock
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_read_superblock(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t superblock_offset,
     uint32_t superblock_size,
     libfdata_list_t *summary_bucket_list,
     libfcache_cache_t *summary_bucket_cache,
     libfdata_list_t *non_summary_bucket_list,
     libfcache_cache_t *non_summary_bucket_cache,
     libcerror_error_t **error )
{
	nsfdb_superblock_header_t superblock_header;
	nsfdb_superblock_footer_t superblock_footer;

	libnsfdb_rrv_bucket_t *rrv_bucket                      = NULL;
	uint8_t *compressed_data                               = NULL;
	uint8_t *superblock_data                               = NULL;
	uint8_t *uncompressed_data                             = NULL;
	static char *function                                  = "libnsfdb_io_handle_read_superblock";
	off64_t bucket_descriptor_offset                       = 0;
	size_t bitmap_descriptors_size                         = 0;
	ssize_t read_count                                     = 0;
	uint32_t bucket_descriptor_iterator                    = 0;
	uint32_t bitmap_size                                   = 0;
	uint32_t calculated_checksum                           = 0;
	uint32_t data_note_identifier_table_size               = 0;
	uint32_t folder_directory_object_size                  = 0;
	uint32_t number_of_bitmaps                             = 0;
	uint32_t number_of_non_summary_buckets                 = 0;
	uint32_t number_of_non_summary_bucket_descriptor_pages = 0;
	uint32_t number_of_summary_buckets                     = 0;
	uint32_t number_of_summary_bucket_descriptor_pages     = 0;
	uint32_t modified_note_log_size                        = 0;
	uint32_t rrv_bucket_size                               = 0;
	uint32_t stored_checksum                               = 0;
	uint32_t stored_size                                   = 0;
	uint32_t uncompressed_size                             = 0;
	uint16_t compression_type                              = 0;
	uint16_t header_signature                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t nsf_timedate_string[ 32 ];

	libfdatetime_nsf_timedate_t *nsf_timedate              = NULL;
	uint32_t value_32bit                                   = 0;
	uint16_t value_16bit                                   = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading superblock at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 superblock_offset,
		 superblock_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &superblock_header,
	              sizeof( nsfdb_superblock_header_t ),
	              superblock_offset,
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_superblock_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock header at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 superblock_offset,
		 superblock_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: superblock header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &superblock_header,
		 sizeof( nsfdb_superblock_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 superblock_header.signature,
	 header_signature );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.uncompressed_size,
	 uncompressed_size );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.number_of_summary_buckets,
	 number_of_summary_buckets );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.number_of_non_summary_buckets,
	 number_of_non_summary_buckets );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.number_of_bitmaps,
	 number_of_bitmaps );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.rrv_bucket_size,
	 rrv_bucket_size );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.bitmap_size,
	 bitmap_size );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.data_note_identifier_table_size,
	 data_note_identifier_table_size );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.modified_note_log_size,
	 modified_note_log_size );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.folder_directory_object_size,
	 folder_directory_object_size );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.size,
	 stored_size );

	byte_stream_copy_to_uint16_little_endian(
	 superblock_header.compression_type,
	 compression_type );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.number_of_summary_bucket_descriptor_pages,
	 number_of_summary_bucket_descriptor_pages );

	byte_stream_copy_to_uint32_little_endian(
	 superblock_header.number_of_non_summary_bucket_descriptor_pages,
	 number_of_non_summary_bucket_descriptor_pages );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 header_signature );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     superblock_header.modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: modification time\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: uncompressed size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 uncompressed_size );

		libcnotify_printf(
		 "%s: number of summary buckets\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_summary_buckets );

		libcnotify_printf(
		 "%s: number of non-summary buckets\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_non_summary_buckets );

		libcnotify_printf(
		 "%s: number of bitmaps\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_bitmaps );

		libcnotify_printf(
		 "%s: RRV bucket size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 rrv_bucket_size );

		byte_stream_copy_to_uint32_little_endian(
		 superblock_header.data_rrv_bucket_position,
		 value_32bit );

		libcnotify_printf(
		 "%s: data RRV bucket position\t\t\t\t: 0x%08" PRIx32 " (offset: 0x%06" PRIx32 "00)\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 superblock_header.rrv_identifier_low,
		 value_32bit );

		libcnotify_printf(
		 "%s: RRV identifier low\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 superblock_header.rrv_identifier_high,
		 value_32bit );

		libcnotify_printf(
		 "%s: RRV identifier high\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: bitmap size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 bitmap_size );

		libcnotify_printf(
		 "%s: data note identifier table size\t\t\t: %" PRIu32 "\n",
		 function,
		 data_note_identifier_table_size );

		libcnotify_printf(
		 "%s: modified note log size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 modified_note_log_size );

		libcnotify_printf(
		 "%s: folder directory object size\t\t\t: %" PRIu32 "\n",
		 function,
		 folder_directory_object_size );

		byte_stream_copy_to_uint16_little_endian(
		 superblock_header.flags,
		 value_16bit );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 superblock_header.write_count,
		 value_32bit );

		libcnotify_printf(
		 "%s: write count\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 stored_size );

		libcnotify_printf(
		 "%s: compression type\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 compression_type );

		libcnotify_printf(
		 "%s: number of summary bucket descriptor pages\t\t: %" PRIu32 "\n",
		 function,
		 number_of_summary_bucket_descriptor_pages );

		libcnotify_printf(
		 "%s: number of non-summary bucket descriptor pages\t: %" PRIu32 "\n",
		 function,
		 number_of_non_summary_bucket_descriptor_pages );

		byte_stream_copy_to_uint32_little_endian(
		 superblock_header.number_of_soft_deleted_note_entries,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of soft deleted note entries\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 superblock_header.shared_template_information_size,
		 value_16bit );

		libcnotify_printf(
		 "%s: shared template information size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 superblock_header.unknown1,
		 2,
		 0 );

		byte_stream_copy_to_uint16_little_endian(
		 superblock_header.number_of_form_names,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of form names\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 superblock_header.form_bitmap_size,
		 value_32bit );

		libcnotify_printf(
		 "%s: form bitmap size\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 superblock_header.unknown2,
		 8,
		 0 );

		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	if( header_signature != 0x000e )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported superblock signature.",
		 function );

		goto on_error;
	}
	if( stored_size > superblock_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: stored superblock size value out of bounds.",
		 function );

		goto on_error;
	}
	/* TODO check stored size with total */
	stored_size -= (uint32_t) sizeof( nsfdb_superblock_header_t )
	             + (uint32_t) sizeof( nsfdb_superblock_footer_t );

	compressed_data = (uint8_t *) memory_allocate(
	                               (size_t) stored_size );

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              compressed_data,
	              stored_size,
	              error );

	if( read_count != (ssize_t) stored_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed data:\n",
		 function );
		libcnotify_print_data(
		 compressed_data,
		 stored_size,
		 0 );
	}
#endif
	if( libnsfdb_checksum_calculate_little_endian_xor32(
	     &calculated_checksum,
	     compressed_data,
	     stored_size,
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
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &superblock_footer,
	              sizeof( nsfdb_superblock_footer_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_superblock_footer_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock footer.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: superblock footer:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &superblock_footer,
		 sizeof( nsfdb_superblock_footer_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 superblock_footer.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     superblock_footer.modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: modification time\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	/* TODO read and print trailing data */

	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in superblock checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( compression_type != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression type.",
		 function );

		goto on_error;
	}
	/* TODO sanity check of uncompressed size
	 */

/*
	uncompressed_size -= sizeof( nsfdb_superblock_header_t )
	                  + sizeof( nsfdb_superblock_footer_t );
*/

	uncompressed_data = (uint8_t *) memory_allocate(
	                                 (size_t) uncompressed_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     uncompressed_data,
	     0,
	     uncompressed_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear uncompressed data.",
		 function );

		goto on_error;
	}
	/* TODO check first 4 bytes of 'compressed data' */
	if( libnsfdb_compression_cx_decompress(
	     uncompressed_data,
	     uncompressed_size,
	     &( compressed_data[ 4 ] ),
	     stored_size - 4,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress superblock data.",
		 function );

		goto on_error;
	}
	memory_free(
	 compressed_data );

	compressed_data = NULL;

	if( number_of_summary_bucket_descriptor_pages > 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of summary bucket descriptor pages.",
		 function );

		goto on_error;
	}
	if( number_of_non_summary_bucket_descriptor_pages > 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported number of non-summary bucket descriptor pages.",
		 function );

		goto on_error;
	}
	if( rrv_bucket_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported RRV bucket size.",
		 function );

		goto on_error;
	}
	if( bitmap_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bitmap size.",
		 function );

		goto on_error;
	}
	if( data_note_identifier_table_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data note identifier table size.",
		 function );

		goto on_error;
	}
	if( modified_note_log_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported modified note log size.",
		 function );

		goto on_error;
	}
	if( folder_directory_object_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported folder directory object size.",
		 function );

		goto on_error;
	}
	superblock_data = uncompressed_data;

	if( number_of_summary_bucket_descriptor_pages > 0 )
	{
		if( uncompressed_size >= 8206 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: summary bucket descriptor page unknown1 (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 4,
				 0 );
			}
#endif
			superblock_data   += 4;
			uncompressed_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfdatetime_nsf_timedate_initialize(
				     &nsf_timedate,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create nsf timedate.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: summary bucket page descriptor (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 sizeof( nsfdb_summary_bucket_page_descriptor_t ),
				 0 );

				if( libfdatetime_nsf_timedate_copy_from_byte_stream(
				     nsf_timedate,
				     ( (nsfdb_summary_bucket_page_descriptor_t *) superblock_data )->modification_time,
				     8,
				     LIBFDATETIME_ENDIAN_LITTLE,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to copy nsf timedate from byte stream.",
					 function );

					goto on_error;
				}
				if( libfdatetime_nsf_timedate_copy_to_utf8_string(
				     nsf_timedate,
				     nsf_timedate_string,
				     32,
				     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to copy nsf timedate to string.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: summary bucket page descriptor modification time\t\t\t: %s\n",
				 function,
				 (char *) nsf_timedate_string );

				libcnotify_printf(
				 "%s: summary bucket page descriptor largest number of free bytes\t\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_summary_bucket_page_descriptor_t *) superblock_data )->largest_number_of_free_bytes );

				libcnotify_printf(
				 "%s: summary bucket page descriptor second largest number of free bytes\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_summary_bucket_page_descriptor_t *) superblock_data )->second_largest_number_of_free_bytes );

				libcnotify_printf(
				 "\n" );

				if( libfdatetime_nsf_timedate_free(
				     &nsf_timedate,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free nsf timedate.",
					 function );

					goto on_error;
				}
			}
#endif
			superblock_data   += sizeof( nsfdb_summary_bucket_page_descriptor_t );
			uncompressed_size -= sizeof( nsfdb_summary_bucket_page_descriptor_t );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( libfdatetime_nsf_timedate_initialize(
				     &nsf_timedate,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
					 "%s: unable to create nsf timedate.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: summary bucket group descriptor (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 sizeof( nsfdb_summary_bucket_group_descriptor_t ),
				 0 );

				if( libfdatetime_nsf_timedate_copy_from_byte_stream(
				     nsf_timedate,
				     ( (nsfdb_summary_bucket_group_descriptor_t *) superblock_data )->modification_time,
				     8,
				     LIBFDATETIME_ENDIAN_LITTLE,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to copy nsf timedate from byte stream.",
					 function );

					goto on_error;
				}
				if( libfdatetime_nsf_timedate_copy_to_utf8_string(
				     nsf_timedate,
				     nsf_timedate_string,
				     32,
				     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to copy nsf timedate to string.",
					 function );

					goto on_error;
				}
				libcnotify_printf(
				 "%s: summary bucket group descriptor modification time\t\t\t: %s\n",
				 function,
				 (char *) nsf_timedate_string );

				libcnotify_printf(
				 "%s: summary bucket group descriptor largest number of free bytes\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_summary_bucket_group_descriptor_t *) superblock_data )->largest_number_of_free_bytes );

				libcnotify_printf(
				 "%s: summary bucket group descriptor second largest number of free bytes\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_summary_bucket_group_descriptor_t *) superblock_data )->second_largest_number_of_free_bytes );

				libcnotify_printf(
				 "\n" );

				if( libfdatetime_nsf_timedate_free(
				     &nsf_timedate,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free nsf timedate.",
					 function );

					goto on_error;
				}
			}
#endif
			superblock_data   += sizeof( nsfdb_summary_bucket_group_descriptor_t );
			uncompressed_size -= sizeof( nsfdb_summary_bucket_group_descriptor_t );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: summary bucket descriptor page unknown2 (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 200,
				 0 );
			}
#endif
			superblock_data   += 200;
			uncompressed_size -= 200;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: summary bucket descriptors (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 sizeof( nsfdb_summary_bucket_descriptor_t ) * number_of_summary_buckets,
				 0 );
			}
#endif
			if( number_of_summary_buckets > 0 )
			{
				if( libfdata_list_resize(
				     summary_bucket_list,
				     number_of_summary_buckets,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
					 "%s: unable to resize summary bucket list.",
					 function );

					goto on_error;
				}
				for( bucket_descriptor_iterator = 0;
				     bucket_descriptor_iterator < number_of_summary_buckets;
				     bucket_descriptor_iterator++ )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (nsfdb_summary_bucket_descriptor_t *) superblock_data )->file_position,
					 bucket_descriptor_offset );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( libfdatetime_nsf_timedate_initialize(
						     &nsf_timedate,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
							 "%s: unable to create nsf timedate.",
							 function );

							goto on_error;
						}
						libcnotify_printf(
						 "%s: summary bucket descriptor: %03" PRIu32 " file position\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
						 function,
						 bucket_descriptor_iterator,
						 bucket_descriptor_offset,
						 bucket_descriptor_offset );

						if( libfdatetime_nsf_timedate_copy_from_byte_stream(
						     nsf_timedate,
						     ( (nsfdb_summary_bucket_descriptor_t *) superblock_data )->modification_time,
						     8,
						     LIBFDATETIME_ENDIAN_LITTLE,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to copy nsf timedate from byte stream.",
							 function );

							goto on_error;
						}
						if( libfdatetime_nsf_timedate_copy_to_utf8_string(
						     nsf_timedate,
						     nsf_timedate_string,
						     32,
						     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to copy nsf timedate to string.",
							 function );

							goto on_error;
						}
						libcnotify_printf(
						 "%s: summary bucket descriptor: %03" PRIu32 " modification time\t\t\t: %s\n",
						 function,
						 bucket_descriptor_iterator,
						 (char *) nsf_timedate_string );

						libcnotify_printf(
						 "%s: summary bucket descriptor: %03" PRIu32 " largest number of free bytes\t\t: %" PRIu8 "\n",
						 function,
						 bucket_descriptor_iterator,
						 ( (nsfdb_summary_bucket_descriptor_t *) superblock_data )->largest_number_of_free_bytes );

						libcnotify_printf(
						 "%s: summary bucket descriptor: %03" PRIu32 " second largest number of free bytes\t: %" PRIu8 "\n",
						 function,
						 bucket_descriptor_iterator,
						 ( (nsfdb_summary_bucket_descriptor_t *) superblock_data )->second_largest_number_of_free_bytes );

						if( libfdatetime_nsf_timedate_free(
						     &nsf_timedate,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
							 "%s: unable to free nsf timedate.",
							 function );

							goto on_error;
						}
					}
#endif
					bucket_descriptor_offset <<= 8;

					if( libfdata_list_set_element_by_index(
					     summary_bucket_list,
					     (int) bucket_descriptor_iterator,
					     0,
					     bucket_descriptor_offset,
					     (size64_t) 0,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set summary bucket descriptors array entry: %" PRIu32 ".",
						 function,
						 bucket_descriptor_iterator );

						goto on_error;
					}
					superblock_data   += sizeof( nsfdb_summary_bucket_descriptor_t );
					uncompressed_size -= sizeof( nsfdb_summary_bucket_descriptor_t );
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );

				libcnotify_printf(
				 "%s: summary bucket descriptor page unknown3 (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 7982 - ( sizeof( nsfdb_summary_bucket_descriptor_t ) * number_of_summary_buckets ),
				 0 );
			}
#endif
			superblock_data   += 7982 - ( sizeof( nsfdb_summary_bucket_descriptor_t ) * number_of_summary_buckets );
			uncompressed_size -= 7982 - ( sizeof( nsfdb_summary_bucket_descriptor_t ) * number_of_summary_buckets );
		}
	}
	if( number_of_non_summary_bucket_descriptor_pages > 0 )
	{
		if( uncompressed_size >= 8198 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: non-summary bucket descriptor page unknown1 (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 4,
				 0 );
			}
#endif
			superblock_data   += 4;
			uncompressed_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: non-summary bucket page descriptor (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 sizeof( nsfdb_non_summary_bucket_page_descriptor_t ),
				 0 );

				libcnotify_printf(
				 "%s: non-summary bucket page descriptor largest number of free bytes\t\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_non_summary_bucket_page_descriptor_t *) superblock_data )->largest_number_of_free_bytes );

				libcnotify_printf(
				 "%s: non-summary bucket page descriptor second largest number of free bytes\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_non_summary_bucket_page_descriptor_t *) superblock_data )->second_largest_number_of_free_bytes );

				libcnotify_printf(
				 "\n" );
			}
#endif
			superblock_data   += sizeof( nsfdb_non_summary_bucket_page_descriptor_t );
			uncompressed_size -= sizeof( nsfdb_non_summary_bucket_page_descriptor_t );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: non-summary bucket group descriptor (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 sizeof( nsfdb_non_summary_bucket_group_descriptor_t ),
				 0 );

				libcnotify_printf(
				 "%s: non-summary bucket group descriptor largest number of free bytes\t\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_non_summary_bucket_group_descriptor_t *) superblock_data )->largest_number_of_free_bytes );

				libcnotify_printf(
				 "%s: non-summary bucket group descriptor second largest number of free bytes\t: %" PRIu8 "\n",
				 function,
				 ( (nsfdb_non_summary_bucket_group_descriptor_t *) superblock_data )->second_largest_number_of_free_bytes );

				libcnotify_printf(
				 "\n" );
			}
#endif
			superblock_data   += sizeof( nsfdb_non_summary_bucket_group_descriptor_t );
			uncompressed_size -= sizeof( nsfdb_non_summary_bucket_group_descriptor_t );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: non-summary bucket descriptor page unknown2 (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 62,
				 0 );
			}
#endif
			superblock_data   += 62;
			uncompressed_size -= 62;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: non-summary bucket descriptors (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 sizeof( nsfdb_non_summary_bucket_descriptor_t ) * number_of_summary_buckets,
				 0 );
			}
#endif
			if( number_of_non_summary_buckets > 0 )
			{
				if( libfdata_list_resize(
				     non_summary_bucket_list,
				     number_of_non_summary_buckets,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
					 "%s: unable to resize non-summary bucket list.",
					 function );

					goto on_error;
				}
				for( bucket_descriptor_iterator = 0;
				     bucket_descriptor_iterator < number_of_non_summary_buckets;
				     bucket_descriptor_iterator++ )
				{
					byte_stream_copy_to_uint32_little_endian(
					 ( (nsfdb_non_summary_bucket_descriptor_t *) superblock_data )->file_position,
					 bucket_descriptor_offset );

#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: non-summary bucket descriptor: %03" PRIu32 " file position\t\t\t\t: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00)\n",
						 function,
						 bucket_descriptor_iterator,
						 bucket_descriptor_offset,
						 bucket_descriptor_offset );

						libcnotify_printf(
						 "%s: non-summary bucket descriptor: %03" PRIu32 " largest number of free bytes\t\t: %" PRIu8 "\n",
						 function,
						 bucket_descriptor_iterator,
						 ( (nsfdb_non_summary_bucket_descriptor_t *) superblock_data )->largest_number_of_free_bytes );

						libcnotify_printf(
						 "%s: non-summary bucket descriptor: %03" PRIu32 " second largest number of free bytes\t: %" PRIu8 "\n",
						 function,
						 bucket_descriptor_iterator,
						 ( (nsfdb_non_summary_bucket_descriptor_t *) superblock_data )->second_largest_number_of_free_bytes );
					}
#endif
					bucket_descriptor_offset <<= 8;

					if( libfdata_list_set_element_by_index(
					     non_summary_bucket_list,
					     (int) bucket_descriptor_iterator,
					     0,
					     bucket_descriptor_offset,
					     (size64_t) 0,
					     0,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set non-summary bucket descriptors array entry: %" PRIu32 ".",
						 function,
						 bucket_descriptor_iterator );

						goto on_error;
					}
					superblock_data   += sizeof( nsfdb_non_summary_bucket_descriptor_t );
					uncompressed_size -= sizeof( nsfdb_non_summary_bucket_descriptor_t );
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );

				libcnotify_printf(
				 "%s: non-summary bucket descriptor page unknown3 (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 8128 - ( sizeof( nsfdb_non_summary_bucket_descriptor_t ) * number_of_non_summary_buckets ),
				 0 );
			}
#endif
			superblock_data   += 8128 - ( sizeof( nsfdb_non_summary_bucket_descriptor_t ) * number_of_non_summary_buckets );
			uncompressed_size -= 8128 - ( sizeof( nsfdb_non_summary_bucket_descriptor_t ) * number_of_non_summary_buckets );
		}
	}
	if( number_of_bitmaps > 0 )
	{
		bitmap_descriptors_size = 20 * number_of_bitmaps;

		if( uncompressed_size >= bitmap_descriptors_size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: bitmap descriptor (uncompressed) data:\n",
				 function );
				libcnotify_print_data(
				 superblock_data,
				 bitmap_descriptors_size,
				 0 );
			}
#endif
			superblock_data   += bitmap_descriptors_size;
			uncompressed_size -= bitmap_descriptors_size;
		}
	}
	if( uncompressed_size >= rrv_bucket_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: initial RRV bucket:\n",
			 function );
		}
#endif
		if( libnsfdb_rrv_bucket_initialize(
		     &rrv_bucket,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create RRV bucket.",
			 function );

			goto on_error;
		}
		if( libnsfdb_rrv_bucket_read(
		     rrv_bucket,
		     superblock_data,
		     rrv_bucket_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read RRV bucket data.",
			 function );

			goto on_error;
		}
		if( libnsfdb_rrv_bucket_free(
		     &rrv_bucket,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free RRV bucket.",
			 function );

			goto on_error;
		}
		superblock_data   += rrv_bucket_size;
		uncompressed_size -= rrv_bucket_size;
	}
	if( uncompressed_size >= bitmap_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: initial bitmap (uncompressed) data:\n",
			 function );
			libcnotify_print_data(
			 superblock_data,
			 bitmap_size,
			 0 );
		}
#endif
		superblock_data   += bitmap_size;
		uncompressed_size -= bitmap_size;
	}
	if( uncompressed_size >= data_note_identifier_table_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data note identifier table (uncompressed) data:\n",
			 function );
			libcnotify_print_data(
			 superblock_data,
			 data_note_identifier_table_size,
			 0 );
		}
#endif
		superblock_data   += data_note_identifier_table_size;
		uncompressed_size -= data_note_identifier_table_size;
	}
	if( uncompressed_size >= modified_note_log_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: modified note log (uncompressed) data:\n",
			 function );
			libcnotify_print_data(
			 superblock_data,
			 modified_note_log_size,
			 0 );
		}
#endif
		superblock_data   += modified_note_log_size;
		uncompressed_size -= modified_note_log_size;
	}
	if( uncompressed_size >= folder_directory_object_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: folder directory object (uncompressed) data:\n",
			 function );
			libcnotify_print_data(
			 superblock_data,
			 folder_directory_object_size,
			 0 );
		}
#endif
		superblock_data   += folder_directory_object_size;
		uncompressed_size -= folder_directory_object_size;
	}
	if( uncompressed_size > 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unknown trailing (uncompressed) data:\n",
			 function );
			libcnotify_print_data(
			 superblock_data,
			 uncompressed_size,
			 0 );
		}
#endif
	}
	memory_free(
	 uncompressed_data );

	uncompressed_data = NULL;

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( nsf_timedate != NULL )
	{
		libfdatetime_nsf_timedate_free(
		 &nsf_timedate,
		 NULL );
	}
#endif
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	if( compressed_data != NULL )
	{
		memory_free(
		 compressed_data );
	}
	return( -1 );
}

/* Reads the bucket descriptor block
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_read_bucket_descriptor_block(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t bucket_descriptor_block_offset,
     uint32_t bucket_descriptor_block_size,
     libcerror_error_t **error )
{
	nsfdb_bucket_descriptor_block_header_t bucket_descriptor_block_header;
	nsfdb_bucket_descriptor_block_footer_t bucket_descriptor_block_footer;

	libnsfdb_rrv_bucket_descriptor_t *rrv_bucket_descriptor = NULL;
	uint8_t *bucket_descriptor_block_data                   = NULL;
	uint8_t *compressed_data                                = NULL;
	uint8_t *uncompressed_data                              = NULL;
	static char *function                                   = "libnsfdb_io_handle_read_bucket_descriptor_block";
	ssize_t read_count                                      = 0;
	uint32_t number_of_rrv_bucket_descriptors               = 0;
	uint32_t number_of_unique_name_keys                     = 0;
	uint32_t calculated_checksum                            = 0;
	uint32_t rrv_bucket_descriptor_iterator                 = 0;
	uint32_t stored_checksum                                = 0;
	uint32_t stored_size                                    = 0;
	uint32_t uncompressed_size                              = 0;
	uint32_t unique_name_key_text_size                      = 0;
	uint16_t compression_type                               = 0;
	uint16_t header_version                                 = 0;
	uint16_t header_signature                               = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t nsf_timedate_string[ 32 ];

	libfdatetime_nsf_timedate_t *nsf_timedate               = NULL;
	uint32_t value_32bit                                    = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading bucket descriptor block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 bucket_descriptor_block_offset,
		 bucket_descriptor_block_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &bucket_descriptor_block_header,
	              sizeof( nsfdb_bucket_descriptor_block_header_t ),
	              bucket_descriptor_block_offset,
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_bucket_descriptor_block_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bucket descriptor block header at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 bucket_descriptor_block_offset,
		 bucket_descriptor_block_offset );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bucket descriptor block header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &bucket_descriptor_block_header,
		 sizeof( nsfdb_bucket_descriptor_block_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 bucket_descriptor_block_header.signature,
	 header_signature );

	byte_stream_copy_to_uint16_little_endian(
	 bucket_descriptor_block_header.version,
	 header_version );

	byte_stream_copy_to_uint16_little_endian(
	 bucket_descriptor_block_header.compression_type,
	 compression_type );

	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_header.uncompressed_size,
	 uncompressed_size );

	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_header.size,
	 stored_size );

	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_header.number_of_unique_name_keys,
	 number_of_unique_name_keys );

	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_header.unique_name_key_text_size,
	 unique_name_key_text_size );

	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_header.number_of_rrv_bucket_descriptors,
	 number_of_rrv_bucket_descriptors );

	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_header.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: signature\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 header_signature );

		libcnotify_printf(
		 "%s: version\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 header_version );

		libcnotify_printf(
		 "%s: compression type\t\t\t: %" PRIu16 "\n",
		 function,
		 compression_type );

		libcnotify_printf(
		 "%s: uncompressed size\t\t\t: %" PRIu32 "\n",
		 function,
		 uncompressed_size );

		byte_stream_copy_to_uint32_little_endian(
		 bucket_descriptor_block_header.write_count,
		 value_32bit );

		libcnotify_printf(
		 "%s: write count\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 stored_size );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     bucket_descriptor_block_header.modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: modification time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: number of unique name keys\t\t: %" PRIu32 "\n",
		 function,
		 number_of_unique_name_keys );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 bucket_descriptor_block_header.unknown1,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: unique name key text size\t\t: %" PRIu32 "\n",
		 function,
		 unique_name_key_text_size );

		libcnotify_printf(
		 "%s: number of RRV bucket descriptors\t: %" PRIu32 "\n",
		 function,
		 number_of_rrv_bucket_descriptors );

		byte_stream_copy_to_uint32_little_endian(
		 bucket_descriptor_block_header.number_of_unk_hash_table_entries,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of UNK hash table entries\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 bucket_descriptor_block_header.unknown2,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 bucket_descriptor_block_header.unknown3,
		 8,
		 0 );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	if( header_signature != 0x0001 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bucket descriptor block signature.",
		 function );

		goto on_error;
	}
	if( header_version != 0x0002 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bucket descriptor block version.",
		 function );

		goto on_error;
	}
	if( libnsfdb_checksum_calculate_little_endian_xor32(
	     &calculated_checksum,
	     (uint8_t *) &bucket_descriptor_block_header,
	     sizeof( nsfdb_bucket_descriptor_block_header_t ) - 12,
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
	/* Ignore the checksum itself
	 */
	if( libnsfdb_checksum_calculate_little_endian_xor32(
	     &calculated_checksum,
	     (uint8_t *) bucket_descriptor_block_header.unknown3,
	     8,
	     calculated_checksum,
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
		 "%s: mismatch in bucket descriptor block header checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( stored_size > bucket_descriptor_block_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: stored bucket descriptor block size value out of bounds.",
		 function );

		goto on_error;
	}
	/* TODO check stored size with total */
	stored_size -= (uint32_t) sizeof( nsfdb_bucket_descriptor_block_header_t )
	             + (uint32_t) sizeof( nsfdb_bucket_descriptor_block_footer_t );

	compressed_data = (uint8_t *) memory_allocate(
	                               stored_size );

	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create compressed data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              compressed_data,
	              stored_size,
	              error );

	if( read_count != (ssize_t) stored_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read compressed data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: compressed data:\n",
		 function );
		libcnotify_print_data(
		 compressed_data,
		 stored_size,
		 0 );
	}
#endif
	if( libnsfdb_checksum_calculate_little_endian_xor32(
	     &calculated_checksum,
	     compressed_data,
	     stored_size,
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
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &bucket_descriptor_block_footer,
	              sizeof( nsfdb_bucket_descriptor_block_footer_t ),
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_bucket_descriptor_block_footer_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bucket descriptor block footer.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bucket descriptor block footer:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &bucket_descriptor_block_footer,
		 sizeof( nsfdb_bucket_descriptor_block_footer_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 bucket_descriptor_block_footer.checksum,
	 stored_checksum );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libfdatetime_nsf_timedate_initialize(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create nsf timedate.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     bucket_descriptor_block_footer.modification_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate from byte stream.",
			 function );

			goto on_error;
		}
		if( libfdatetime_nsf_timedate_copy_to_utf8_string(
		     nsf_timedate,
		     nsf_timedate_string,
		     32,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_MILLI_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy nsf timedate to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: modification time\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_free(
		     &nsf_timedate,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free nsf timedate.",
			 function );

			goto on_error;
		}
	}
#endif
	/* TODO read and print trailing data */

	if( stored_checksum != calculated_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in bucket descriptor block checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
		 function,
		 stored_checksum,
		 calculated_checksum );

		goto on_error;
	}
	if( compression_type != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compression type.",
		 function );

		goto on_error;
	}
	/* TODO sanity check of uncompressed size
	 */

/*
	uncompressed_size -= sizeof( nsfdb_bucket_descriptor_block_header_t )
	                   + sizeof( nsfdb_bucket_descriptor_block_footer_t );
*/

	uncompressed_data = (uint8_t *) memory_allocate(
	                                 (size_t) uncompressed_size );

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create uncompressed data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     uncompressed_data,
	     0,
	     uncompressed_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear uncompressed data.",
		 function );

		goto on_error;
	}
	/* TODO check first 4 bytes of 'compressed data' */
	if( libnsfdb_compression_cx_decompress(
	     uncompressed_data,
	     uncompressed_size,
	     &( compressed_data[ 4 ] ),
	     stored_size - 4,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
		 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
		 "%s: unable to decompress bucket descriptor block data.",
		 function );

		goto on_error;
	}
	memory_free(
	 compressed_data );

	compressed_data = NULL;

	/* TODO bucket descriptor block info */

	bucket_descriptor_block_data = uncompressed_data;

	if( number_of_rrv_bucket_descriptors > 0 )
	{
		/* TODO check if size is in range */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: RRV bucket descriptor data:\n",
			 function );
			libcnotify_print_data(
			 bucket_descriptor_block_data,
			 number_of_rrv_bucket_descriptors * 8,
			 0 );
		}
#endif
		if( libcdata_array_resize(
		     io_handle->rrv_bucket_descriptors,
		     number_of_rrv_bucket_descriptors,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_rrv_bucket_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize RRV bucket descriptors array.",
			 function );

			goto on_error;
		}
		for( rrv_bucket_descriptor_iterator = 0;
		     rrv_bucket_descriptor_iterator < number_of_rrv_bucket_descriptors;
		     rrv_bucket_descriptor_iterator++ )
		{
			if( libnsfdb_rrv_bucket_descriptor_initialize(
			     &rrv_bucket_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize RRV bucket descriptor.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint32_little_endian(
			 bucket_descriptor_block_data,
			 rrv_bucket_descriptor->file_offset );

			bucket_descriptor_block_data += 4;
			uncompressed_size            -= 4;

			byte_stream_copy_to_uint32_little_endian(
			 bucket_descriptor_block_data,
			 rrv_bucket_descriptor->initial_rrv_identifier );

			bucket_descriptor_block_data += 4;
			uncompressed_size            -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: %s RRV bucket descriptor: %03d file position: 0x%08" PRIx64 " (offset: 0x%06" PRIx64 "00), initial RRV: 0x%08" PRIx32 "\n",
				 function,
				 ( ( ( rrv_bucket_descriptor->file_offset & 0x00000001UL ) != 0 ) ? "non-data" : "data" ),
				 rrv_bucket_descriptor_iterator,
				 rrv_bucket_descriptor->file_offset & 0xfffffffeUL,
				 rrv_bucket_descriptor->file_offset & 0xfffffffeUL,
				 rrv_bucket_descriptor->initial_rrv_identifier );
			}
#endif
			if( ( rrv_bucket_descriptor->file_offset & 0x00000001UL ) != 0 )
			{
				rrv_bucket_descriptor->file_offset -= 1;
				rrv_bucket_descriptor->type         = LIBNSFDB_RRV_BUCKET_TYPE_NON_DATA;
			}
			else
			{
				rrv_bucket_descriptor->type = LIBNSFDB_RRV_BUCKET_TYPE_DATA;
			}
			rrv_bucket_descriptor->file_offset <<= 8;

			if( libcdata_array_set_entry_by_index(
			     io_handle->rrv_bucket_descriptors,
			     (int) rrv_bucket_descriptor_iterator,
			     (intptr_t *) rrv_bucket_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set RRV bucket descriptors array entry: %" PRIu32 ".",
				 function,
				 rrv_bucket_descriptor_iterator );

				goto on_error;
			}
			rrv_bucket_descriptor = NULL;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	read_count = libnsfdb_unique_name_key_table_read(
	              io_handle->unique_name_key_table,
	              bucket_descriptor_block_data,
	              uncompressed_size,
	              number_of_unique_name_keys,
	              unique_name_key_text_size,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read UNK table.",
		 function );

		goto on_error;
	}
	bucket_descriptor_block_data += read_count;
	uncompressed_size            -= read_count;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: (remaining) uncompressed data:\n",
		 function );
		libcnotify_print_data(
		 bucket_descriptor_block_data,
		 uncompressed_size,
		 0 );
	}
#endif
	memory_free(
	 uncompressed_data );

	uncompressed_data = NULL;

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( nsf_timedate != NULL )
	{
		libfdatetime_nsf_timedate_free(
		 &nsf_timedate,
		 NULL );
	}
#endif
	if( rrv_bucket_descriptor != NULL )
	{
		libnsfdb_rrv_bucket_descriptor_free(
		 &rrv_bucket_descriptor,
		 NULL );
	}
	if( uncompressed_data != NULL )
	{
		memory_free(
		 uncompressed_data );
	}
	if( compressed_data != NULL )
	{
		memory_free(
		 compressed_data );
	}
	return( -1 );
}

/* Reads the Record Relocation Vector (RRV) bucket
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_read_rrv_bucket(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libnsfdb_rrv_bucket_t *rrv_bucket = NULL;
	uint8_t *rrv_bucket_data          = NULL;
	static char *function             = "libnsfdb_io_handle_read_rrv_bucket";
	ssize_t read_count                = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( element_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( element_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( element_size != io_handle->rrv_bucket_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element size value does not match RRV bucket size.",
		 function );

		return( -1 );
	}
	rrv_bucket_data = (uint8_t *) memory_allocate(
	                               sizeof( uint8_t ) * (size_t) element_size );

	if( rrv_bucket_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create RRV bucket data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading RRV bucket at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 element_offset,
		 element_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              rrv_bucket_data,
	              (size_t) element_size,
	              element_offset,
	              error );

	if( read_count != (ssize_t) element_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read RRV bucket data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 element_offset,
		 element_offset );

		goto on_error;
	}
	if( libnsfdb_rrv_bucket_initialize(
	     &rrv_bucket,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create RRV bucket.",
		 function );

		goto on_error;
	}
	if( libnsfdb_rrv_bucket_read(
	     rrv_bucket,
	     rrv_bucket_data,
	     (size_t) element_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read RRV bucket.",
		 function );

		goto on_error;
	}
	memory_free(
	 rrv_bucket_data );

	rrv_bucket_data = NULL;

	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     element_index,
	     (intptr_t *) rrv_bucket,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_rrv_bucket_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set RRV bucket as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( rrv_bucket != NULL )
	{
		libnsfdb_rrv_bucket_free(
		 &rrv_bucket,
		 NULL );
	}
	if( rrv_bucket_data != NULL )
	{
		memory_free(
		 rrv_bucket_data );
	}
	return( -1 );
}

/* Reads the bucket
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_read_bucket(
     intptr_t *data_handle LIBNSFDB_ATTRIBUTE_UNUSED,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error )
{
	libnsfdb_bucket_t *bucket = NULL;
	static char *function     = "libnsfdb_io_handle_read_bucket";

	LIBNSFDB_UNREFERENCED_PARAMETER( data_handle )

	if( element_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( libnsfdb_bucket_initialize(
	     &bucket,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create bucket.",
		 function );

		goto on_error;
	}
	if( libnsfdb_bucket_read(
	     bucket,
	     file_io_handle,
	     element_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bucket.",
		 function );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) cache,
	     (intptr_t *) bucket,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_bucket_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set bucket as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( bucket != NULL )
	{
		libnsfdb_bucket_free(
		 &bucket,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of RRV values
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_get_number_of_rrv_values(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t note_type,
     int *number_of_rrv_values,
     libcerror_error_t **error )
{
	libnsfdb_rrv_bucket_descriptor_t *rrv_bucket_descriptor = NULL;
	libnsfdb_rrv_bucket_t *rrv_bucket                       = NULL;
	static char *function                                   = "libnsfdb_io_handle_get_number_of_rrv_values";
	off64_t element_data_offset                             = 0;
	int number_of_rrv_bucket_descriptors                    = 0;
	int number_of_rrv_bucket_values                         = 0;
	int rrv_bucket_descriptor_index                         = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( note_type != LIBNSFDB_NOTE_TYPE_ALL )
	 && ( note_type != LIBNSFDB_NOTE_TYPE_DATA )
	 && ( note_type != LIBNSFDB_NOTE_TYPE_NON_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported note type.",
		 function );

		return( -1 );
	}
	if( number_of_rrv_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of RRV values.",
		 function );

		return( -1 );
	}
	*number_of_rrv_values = 0;

	if( libcdata_array_get_number_of_entries(
	     io_handle->rrv_bucket_descriptors,
	     &number_of_rrv_bucket_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of RRV bucket descriptors.",
		 function );

		return( -1 );
	}
	for( rrv_bucket_descriptor_index = 0;
	     rrv_bucket_descriptor_index < number_of_rrv_bucket_descriptors;
	     rrv_bucket_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     io_handle->rrv_bucket_descriptors,
		     rrv_bucket_descriptor_index,
		     (intptr_t **) &rrv_bucket_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV bucket descriptor: %" PRIu32 ".",
			 function,
			 rrv_bucket_descriptor_index );

			return( -1 );
		}
		if( rrv_bucket_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing RRV bucket descriptor: %" PRIu32 ".",
			 function,
			 rrv_bucket_descriptor_index );

			return( -1 );
		}
		if( ( note_type == LIBNSFDB_NOTE_TYPE_DATA )
		 && ( rrv_bucket_descriptor->type != LIBNSFDB_RRV_BUCKET_TYPE_DATA ) )
		{
			continue;
		}
		else if( ( note_type == LIBNSFDB_NOTE_TYPE_NON_DATA )
		      && ( rrv_bucket_descriptor->type != LIBNSFDB_RRV_BUCKET_TYPE_NON_DATA ) )
		{
			continue;
		}
		if( libfdata_vector_get_element_value_at_offset(
		     io_handle->rrv_bucket_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) io_handle->rrv_bucket_cache,
		     rrv_bucket_descriptor->file_offset,
		     &element_data_offset,
		     (intptr_t **) &rrv_bucket,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV bucket at offset: %" PRIi64 ".",
			 function,
			 rrv_bucket_descriptor->file_offset );

			return( -1 );
		}
		if( libnsfdb_rrv_bucket_get_number_of_values(
		     rrv_bucket,
		     &number_of_rrv_bucket_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to number of RRV values from bucket.",
			 function );

			return( -1 );
		}
		*number_of_rrv_values += number_of_rrv_bucket_values;
	}
	return( 1 );
}

/* Retrieves a specific RRV value
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_io_handle_get_rrv_value_by_index(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t note_type,
     int rrv_value_index,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error )
{
	libnsfdb_rrv_bucket_descriptor_t *rrv_bucket_descriptor = NULL;
	libnsfdb_rrv_bucket_t *rrv_bucket                       = NULL;
	static char *function                                   = "libnsfdb_io_handle_get_rrv_value_by_index";
	off64_t element_data_offset                             = 0;
	int number_of_rrv_bucket_descriptors                    = 0;
	int number_of_rrv_bucket_values                         = 0;
	int rrv_bucket_descriptor_index                         = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( note_type != LIBNSFDB_NOTE_TYPE_ALL )
	 && ( note_type != LIBNSFDB_NOTE_TYPE_DATA )
	 && ( note_type != LIBNSFDB_NOTE_TYPE_NON_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported note type.",
		 function );

		return( -1 );
	}
	if( rrv_value_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: RRV value index out of bounds.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     io_handle->rrv_bucket_descriptors,
	     &number_of_rrv_bucket_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of RRV bucket descriptors.",
		 function );

		return( -1 );
	}
	for( rrv_bucket_descriptor_index = 0;
	     rrv_bucket_descriptor_index < number_of_rrv_bucket_descriptors;
	     rrv_bucket_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     io_handle->rrv_bucket_descriptors,
		     rrv_bucket_descriptor_index,
		     (intptr_t **) &rrv_bucket_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV bucket descriptor: %" PRIu32 ".",
			 function,
			 rrv_bucket_descriptor_index );

			return( -1 );
		}
		if( rrv_bucket_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing RRV bucket descriptor: %" PRIu32 ".",
			 function,
			 rrv_bucket_descriptor_index );

			return( -1 );
		}
		if( ( note_type == LIBNSFDB_NOTE_TYPE_DATA )
		 && ( rrv_bucket_descriptor->type != LIBNSFDB_RRV_BUCKET_TYPE_DATA ) )
		{
			continue;
		}
		else if( ( note_type == LIBNSFDB_NOTE_TYPE_NON_DATA )
		      && ( rrv_bucket_descriptor->type != LIBNSFDB_RRV_BUCKET_TYPE_NON_DATA ) )
		{
			continue;
		}
		if( libfdata_vector_get_element_value_at_offset(
		     io_handle->rrv_bucket_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) io_handle->rrv_bucket_cache,
		     rrv_bucket_descriptor->file_offset,
		     &element_data_offset,
		     (intptr_t **) &rrv_bucket,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV bucket at offset: %" PRIi64 ".",
			 function,
			 rrv_bucket_descriptor->file_offset );

			return( -1 );
		}
		if( libnsfdb_rrv_bucket_get_number_of_values(
		     rrv_bucket,
		     &number_of_rrv_bucket_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to number of RRV values from bucket.",
			 function );

			return( -1 );
		}
		if( rrv_value_index < number_of_rrv_bucket_values )
		{
			if( libnsfdb_rrv_bucket_get_value_by_index(
			     rrv_bucket,
			     rrv_value_index,
			     rrv_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to RRV value: %d from bucket.",
				 function,
				 rrv_value_index );

				return( -1 );
			}
			break;
		}
		rrv_value_index -= number_of_rrv_bucket_values;
	}
	if( rrv_bucket_descriptor_index >= number_of_rrv_bucket_descriptors )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: RRV bucket descriptor index out of bounds.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a RRV value for a specific identifier
 * Returns 1 if successful, 0 if no RRV value was found or -1 on error
 */
int libnsfdb_io_handle_get_rrv_value_by_identifier(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t note_type,
     uint32_t rrv_identifier,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error )
{
	libnsfdb_rrv_bucket_descriptor_t *last_rrv_bucket_descriptor = NULL;
	libnsfdb_rrv_bucket_descriptor_t *rrv_bucket_descriptor      = NULL;
	libnsfdb_rrv_bucket_t *rrv_bucket                            = NULL;
	static char *function                                        = "libnsfdb_io_handle_get_rrv_value_by_identifier";
	off64_t element_data_offset                                  = 0;
	int number_of_rrv_bucket_descriptors                         = 0;
	int result                                                   = 0;
	int rrv_bucket_descriptor_index                              = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( ( note_type != LIBNSFDB_NOTE_TYPE_ALL )
	 && ( note_type != LIBNSFDB_NOTE_TYPE_DATA )
	 && ( note_type != LIBNSFDB_NOTE_TYPE_NON_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported note type.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     io_handle->rrv_bucket_descriptors,
	     &number_of_rrv_bucket_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of RRV bucket descriptors.",
		 function );

		return( -1 );
	}
	for( rrv_bucket_descriptor_index = 0;
	     rrv_bucket_descriptor_index < number_of_rrv_bucket_descriptors;
	     rrv_bucket_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     io_handle->rrv_bucket_descriptors,
		     rrv_bucket_descriptor_index,
		     (intptr_t **) &rrv_bucket_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV bucket descriptor: %" PRIu32 ".",
			 function,
			 rrv_bucket_descriptor_index );

			return( -1 );
		}
		if( rrv_bucket_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing RRV bucket descriptor: %" PRIu32 ".",
			 function,
			 rrv_bucket_descriptor_index );

			return( -1 );
		}
		if( ( note_type == LIBNSFDB_NOTE_TYPE_DATA )
		 && ( rrv_bucket_descriptor->type != LIBNSFDB_RRV_BUCKET_TYPE_DATA ) )
		{
			continue;
		}
		else if( ( note_type == LIBNSFDB_NOTE_TYPE_NON_DATA )
		      && ( rrv_bucket_descriptor->type != LIBNSFDB_RRV_BUCKET_TYPE_NON_DATA ) )
		{
			continue;
		}
		if( rrv_identifier == rrv_bucket_descriptor->initial_rrv_identifier )
		{
			if( libfdata_vector_get_element_value_at_offset(
			     io_handle->rrv_bucket_vector,
			     (intptr_t *) file_io_handle,
			     (libfdata_cache_t *) io_handle->rrv_bucket_cache,
			     rrv_bucket_descriptor->file_offset,
			     &element_data_offset,
			     (intptr_t **) &rrv_bucket,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve RRV bucket at offset: %" PRIi64 ".",
				 function,
				 rrv_bucket_descriptor->file_offset );

				return( -1 );
			}
			break;
		}
		else if( rrv_identifier < rrv_bucket_descriptor->initial_rrv_identifier )
		{
			if( last_rrv_bucket_descriptor == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing last RRV bucket descriptor.",
				 function );

				return( -1 );
			}
			if( libfdata_vector_get_element_value_at_offset(
			     io_handle->rrv_bucket_vector,
			     (intptr_t *) file_io_handle,
			     (libfdata_cache_t *) io_handle->rrv_bucket_cache,
			     last_rrv_bucket_descriptor->file_offset,
			     &element_data_offset,
			     (intptr_t **) &rrv_bucket,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve RRV bucket at offset: %" PRIi64 ".",
				 function,
				 last_rrv_bucket_descriptor->file_offset );

				return( -1 );
			}
			break;
		}
		last_rrv_bucket_descriptor = rrv_bucket_descriptor;
	}
	if( rrv_bucket_descriptor_index >= number_of_rrv_bucket_descriptors )
	{
		if( last_rrv_bucket_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing last RRV bucket descriptor.",
			 function );

			return( -1 );
		}
		if( libfdata_vector_get_element_value_at_offset(
		     io_handle->rrv_bucket_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) io_handle->rrv_bucket_cache,
		     last_rrv_bucket_descriptor->file_offset,
		     &element_data_offset,
		     (intptr_t **) &rrv_bucket,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV bucket at offset: %" PRIi64 ".",
			 function,
			 last_rrv_bucket_descriptor->file_offset );

			return( -1 );
		}
	}
	if( rrv_bucket != NULL )
	{
		result = libnsfdb_rrv_bucket_get_value_by_identifier(
			  rrv_bucket,
			  rrv_identifier,
			  rrv_value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve RRV value: 0x%08" PRIx32 " from bucket.",
			 function,
			 rrv_identifier );

			return( -1 );
		}
	}
	return( result );
}

