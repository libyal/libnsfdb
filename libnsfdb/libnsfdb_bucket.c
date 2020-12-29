/*
 * Bucket functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libnsfdb_bucket.h"
#include "libnsfdb_debug.h"
#include "libnsfdb_definitions.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"
#include "libnsfdb_libfdatetime.h"

#include "nsfdb_bucket.h"

/* Creates a bucket slot
 * Make sure the value bucket_slot is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_slot_initialize(
     libnsfdb_bucket_slot_t **bucket_slot,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_initialize_slot";

	if( bucket_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket slot.",
		 function );

		return( -1 );
	}
	if( *bucket_slot != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid bucket slot value already set.",
		 function );

		return( -1 );
	}
	*bucket_slot = memory_allocate_structure(
	                libnsfdb_bucket_slot_t );

	if( *bucket_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create bucket slot.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *bucket_slot,
	     0,
	     sizeof( libnsfdb_bucket_slot_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear bucket slot.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *bucket_slot != NULL )
	{
		memory_free(
		 *bucket_slot );

		*bucket_slot = NULL;
	}
	return( -1 );
}

/* Frees a bucket slot
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_slot_free(
     libnsfdb_bucket_slot_t **bucket_slot,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_slot_free";

	if( bucket_slot == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket slot.",
		 function );

		return( -1 );
	}
	if( *bucket_slot != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 *bucket_slot );

		*bucket_slot = NULL;
	}
	return( 1 );
}

/* Creates a bucket index entry
 * Make sure the value bucket_index_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_index_entry_initialize(
     libnsfdb_bucket_index_entry_t **bucket_index_entry,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_initialize_index_entry";

	if( bucket_index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket index entry.",
		 function );

		return( -1 );
	}
	if( *bucket_index_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid bucket index entry value already set.",
		 function );

		return( -1 );
	}
	*bucket_index_entry = memory_allocate_structure(
	                       libnsfdb_bucket_index_entry_t );

	if( *bucket_index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create bucket index entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *bucket_index_entry,
	     0,
	     sizeof( libnsfdb_bucket_index_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear bucket index entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *bucket_index_entry != NULL )
	{
		memory_free(
		 *bucket_index_entry );

		*bucket_index_entry = NULL;
	}
	return( -1 );
}

/* Frees a bucket index entry
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_index_entry_free(
     libnsfdb_bucket_index_entry_t **bucket_index_entry,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_index_entry_free";

	if( bucket_index_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket index entry.",
		 function );

		return( -1 );
	}
	if( *bucket_index_entry != NULL )
	{
		memory_free(
		 *bucket_index_entry );

		*bucket_index_entry = NULL;
	}
	return( 1 );
}

/* Creates a bucket
 * Make sure the value bucket is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_initialize(
     libnsfdb_bucket_t **bucket,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_initialize";

	if( bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket.",
		 function );

		return( -1 );
	}
	if( *bucket != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid bucket value already set.",
		 function );

		return( -1 );
	}
	*bucket = memory_allocate_structure(
	           libnsfdb_bucket_t );

	if( *bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create bucket.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *bucket,
	     0,
	     sizeof( libnsfdb_bucket_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear bucket.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *bucket )->slots_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create bucket slots array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *bucket != NULL )
	{
		memory_free(
		 *bucket );

		*bucket = NULL;
	}
	return( -1 );
}

/* Frees a bucket
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_free(
     libnsfdb_bucket_t **bucket,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_free";
	int result            = 1;

	if( bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket.",
		 function );

		return( -1 );
	}
	if( *bucket != NULL )
	{
		if( libcdata_array_free(
		     &( ( *bucket )->slots_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_bucket_slot_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the bucket slots array.",
			 function );

			result = -1;
		}
		if( ( *bucket )->data != NULL )
		{
			memory_free(
			 ( *bucket )->data );
		}
		memory_free(
		 *bucket );

		*bucket = NULL;
	}
	return( result );
}

/* Reads a bucket and its slots
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_read(
     libnsfdb_bucket_t *bucket,
     libbfio_handle_t *file_io_handle,
     off64_t bucket_offset,
     libcerror_error_t **error )
{
	libcdata_array_t *bucket_index_array      = NULL;
	uint8_t *bucket_header_data               = NULL;
	void *reallocation                        = NULL;
	static char *function                     = "libnsfdb_bucket_read";
	ssize_t read_count                        = 0;
	uint32_t number_of_slots                  = 0;
	uint32_t stored_checksum                  = 0;
	uint32_t footer_size                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t nsf_timedate_string[ 32 ];

	libfdatetime_nsf_timedate_t *nsf_timedate = NULL;
#endif

	if( bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket.",
		 function );

		return( -1 );
	}
	if( bucket->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid bucket data already set.",
		 function );

		return( -1 );
	}
	bucket->data = (uint8_t *) memory_allocate(
	                            sizeof( nsfdb_bucket_header_t ) );

	if( bucket->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create bucket data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading bucket offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 bucket_offset,
		 bucket_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              bucket->data,
	              sizeof( nsfdb_bucket_header_t ),
	              bucket_offset,
	              error );

	if( read_count != (ssize_t) sizeof( nsfdb_bucket_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bucket header data at offset: %" PRIu64 " (0x%08" PRIx64 ").",
		 function,
		 bucket_offset,
		 bucket_offset );

		goto on_error;
	}
	bucket_header_data = bucket->data;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bucket header data:\n",
		 function );
		libcnotify_print_data(
		 bucket_header_data,
		 sizeof( nsfdb_bucket_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_bucket_header_t *) bucket_header_data )->size,
	 bucket->data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_bucket_header_t *) bucket_header_data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_bucket_header_t *) bucket_header_data )->number_of_slots,
	 number_of_slots );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_bucket_header_t *) bucket_header_data )->footer_size,
	 footer_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->signature );

		libcnotify_printf(
		 "%s: header size\t\t\t\t\t\t: %" PRIu8 "\n",
		 function,
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->header_size );

		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->unknown1,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 bucket->data_size );

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
		     ( (nsfdb_bucket_header_t *) bucket_header_data )->modification_time,
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
		 "%s: modification time\t\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

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
		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->unknown2,
		 20,
		 0 );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->unknown3,
		 2,
		 0 );

		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: number of slots\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_slots );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->unknown4,
		 2,
		 0 );

		libcnotify_printf(
		 "%s: footer size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 footer_size );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 ( (nsfdb_bucket_header_t *) bucket_header_data )->unknown5,
		 12,
		 0 );
	}
#endif

	if( ( (nsfdb_bucket_header_t *) bucket_header_data )->signature != 0x02 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bucket signature.",
		 function );

		goto on_error;
	}
	if( ( (nsfdb_bucket_header_t *) bucket_header_data )->header_size != 0x42 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported bucket header size.",
		 function );

		goto on_error;
	}
	if( bucket->data_size <= (uint32_t) sizeof( nsfdb_bucket_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bucket size value out of bound.",
		 function );

		goto on_error;
	}
	reallocation = memory_reallocate(
	               bucket->data,
	               bucket->data_size );

	if( reallocation == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to resize bucket data.",
		 function );

		goto on_error;
	}
	bucket->data = (uint8_t *) reallocation;

	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              &( bucket->data[ sizeof( nsfdb_bucket_header_t ) ] ),
	              (size_t) bucket->data_size - sizeof( nsfdb_bucket_header_t ),
	              error );

	if( read_count != (ssize_t) ( bucket->data_size - sizeof( nsfdb_bucket_header_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bucket data.",
		 function );

		goto on_error;
	}
	if( number_of_slots > 0 )
	{
		/* Create the bucket index array
		 */
		if( libcdata_array_initialize(
		     &bucket_index_array,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create bucket index array.",
			 function );

			goto on_error;
		}
		if( libnsfdb_bucket_read_index(
		     bucket_index_array,
		     number_of_slots,
		     bucket->data,
		     bucket->data_size - footer_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read bucket index.",
			 function );

			goto on_error;
		}
		/* The offsets in the bucket index are relative after the bucket header
		 */
		if( libnsfdb_bucket_read_slots(
		     bucket->slots_array,
		     bucket_index_array,
		     bucket->data,
		     bucket->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read bucket slots.",
			 function );

			goto on_error;
		}
		if( libcdata_array_free(
		     &bucket_index_array,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_bucket_index_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the bucket index array.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: bucket footer:\n",
		 function );
		libcnotify_print_data(
		 &( bucket->data[ bucket->data_size - footer_size ] ),
		 footer_size,
		 0 );
	}
#endif
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
	if( bucket_index_array != NULL )
	{
		libcdata_array_free(
		 &bucket_index_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_bucket_index_entry_free,
		 NULL );
	}
	if( bucket->data != NULL )
	{
		memory_free(
		 bucket->data );

		bucket->data = NULL;
	}
	return( -1 );
}

/* Reads the bucket index
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_read_index(
     libcdata_array_t *bucket_index_array,
     uint32_t number_of_bucket_slots,
     uint8_t *bucket_data,
     size_t bucket_data_size,
     libcerror_error_t **error )
{
	libnsfdb_bucket_index_entry_t *bucket_index_entry = NULL;
	uint8_t *bucket_index_data                        = NULL;
	static char *function                             = "libnsfdb_bucket_read_index";
	uint32_t bucket_slot_iterator                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t bucket_index_data_size                     = 0;
#endif

	if( bucket_index_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket index array.",
		 function );

		return( -1 );
	}
	if( bucket_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket data.",
		 function );

		return( -1 );
	}
	if( bucket_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bucket data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		bucket_index_data_size = 4 * number_of_bucket_slots;

		libcnotify_printf(
		 "%s: bucket index:\n",
		 function );
		libcnotify_print_data(
		 &( bucket_data[ bucket_data_size - bucket_index_data_size ] ),
		 bucket_index_data_size,
		 0 );
	}
#endif
	/* Resize the bucket index array
	 */
	if( libcdata_array_resize(
	     bucket_index_array,
	     number_of_bucket_slots,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_bucket_index_entry_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize bucket index array.",
		 function );

		goto on_error;
	}
	/* Read the bucket index back to front
	 */
	bucket_index_data = &( bucket_data[ bucket_data_size - 2 ] );

	for( bucket_slot_iterator = 0;
	     bucket_slot_iterator < number_of_bucket_slots;
	     bucket_slot_iterator++ )
	{
		if( libnsfdb_bucket_index_entry_initialize(
		     &bucket_index_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create bucket index entry.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 bucket_index_data,
		 bucket_index_entry->size );

		bucket_index_data -= 2;

		byte_stream_copy_to_uint16_little_endian(
		 bucket_index_data,
		 bucket_index_entry->offset );

		bucket_index_data -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: bucket slot: %03" PRIu32 " offset\t\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 bucket_slot_iterator,
			 bucket_index_entry->offset,
			 bucket_index_entry->offset );

			libcnotify_printf(
			 "%s: bucket slot: %03" PRIu32 " size\t\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 bucket_slot_iterator,
			 bucket_index_entry->size,
			 bucket_index_entry->size );
		}
#endif
		if( libcdata_array_set_entry_by_index(
		     bucket_index_array,
		     (int) bucket_slot_iterator,
		     (intptr_t *) bucket_index_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set bucket index: %" PRIu32 ".",
			 function,
			 bucket_slot_iterator );

			goto on_error;
		}
		bucket_index_entry = NULL;
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
	if( bucket_index_entry != NULL )
	{
		libnsfdb_bucket_index_entry_free(
		 &bucket_index_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the bucket slots
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_read_slots(
     libcdata_array_t *bucket_slots_array,
     libcdata_array_t *bucket_index_array,
     uint8_t *bucket_data,
     size_t bucket_data_size,
     libcerror_error_t **error )
{
	libnsfdb_bucket_index_entry_t *bucket_index_entry = NULL;
	libnsfdb_bucket_slot_t *bucket_slot               = NULL;
	static char *function                             = "libnsfdb_bucket_read_slots";
	int bucket_slot_iterator                          = 0;
	int number_of_entries                             = 0;

	if( bucket_slots_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket slots array.",
		 function );

		return( -1 );
	}
	if( bucket_index_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket index array.",
		 function );

		return( -1 );
	}
	if( bucket_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket data.",
		 function );

		return( -1 );
	}
	if( bucket_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid bucket slots data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     bucket_index_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries of bucket index array.",
		 function );

		goto on_error;
	}
	/* Resize the bucket slots array
	 */
	if( libcdata_array_resize(
	     bucket_slots_array,
	     number_of_entries,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_bucket_slot_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize bucket slots array.",
		 function );

		goto on_error;
	}
	for( bucket_slot_iterator = 0;
	     bucket_slot_iterator < number_of_entries;
	     bucket_slot_iterator++ )
	{
		if( libcdata_array_get_entry_by_index(
		     bucket_index_array,
		     bucket_slot_iterator,
		     (intptr_t **) &bucket_index_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve bucket slot: %d.",
			 function,
			 bucket_slot_iterator );

			goto on_error;
		}
		if( bucket_index_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid bucket index slot.",
			 function );

			goto on_error;
		}
		if( libnsfdb_bucket_slot_initialize(
		     &bucket_slot,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create bucket slot.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: bucket slot: %03" PRIu32 " offset: % 5" PRIu16 ", size: % 5" PRIu16 "\n",
			 function,
			 bucket_slot_iterator,
			 bucket_index_entry->offset,
			 bucket_index_entry->size );
		}
#endif
		/* TODO check sanity of slot offset and size */

		bucket_slot->data = &( bucket_data[ bucket_index_entry->offset ] );
		bucket_slot->size = bucket_index_entry->size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: bucket slot: %d data:\n",
			 function,
			 bucket_slot_iterator );
			libcnotify_print_data(
			 bucket_slot->data,
			 (size_t) bucket_slot->size,
			 0 );
		}
#endif
		if( libcdata_array_set_entry_by_index(
		     bucket_slots_array,
		     bucket_slot_iterator,
		     (intptr_t *) bucket_slot,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set bucket slot: %d.",
			 function,
			 bucket_slot_iterator );

			goto on_error;
		}
		bucket_slot = NULL;
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
	if( bucket_slot != NULL )
	{
		libnsfdb_bucket_slot_free(
		 &bucket_slot,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of bucket slots
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_get_number_of_slots(
     libnsfdb_bucket_t *bucket,
     uint32_t *number_of_slots,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_get_number_of_slots";
	int number_of_entries = 0;

	if( bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket.",
		 function );

		return( -1 );
	}
	if( bucket->slots_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid bucket - missing slots array.",
		 function );

		return( -1 );
	}
	if( number_of_slots == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of slots.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     bucket->slots_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	if( number_of_entries > (int) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of bucket slots value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_slots = (uint32_t) number_of_entries;

	return( 1 );
}

/* Retrieves the bucket slot at the index
 * Buckets are numbered from 1 to N
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_get_slot(
     libnsfdb_bucket_t *bucket,
     uint32_t slot_index,
     libnsfdb_bucket_slot_t **bucket_slot,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_get_slot";

	if( bucket == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid bucket.",
		 function );

		return( -1 );
	}
	if( bucket->slots_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid bucket - missing slots array.",
		 function );

		return( -1 );
	}
	if( slot_index == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid slot index value zero or less.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     bucket->slots_array,
	     (int) slot_index - 1,
	     (intptr_t **) bucket_slot,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bucket slot: %" PRIu32 ".",
		 function,
		 slot_index );

		return( -1 );
	}
	return( 1 );
}

