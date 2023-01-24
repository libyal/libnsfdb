/*
 * Note functions
 *
 * Copyright (C) 2010-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include "libnsfdb_bucket_list.h"
#include "libnsfdb_checksum.h"
#include "libnsfdb_debug.h"
#include "libnsfdb_definitions.h"
#include "libnsfdb_io_handle.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"
#include "libnsfdb_libfdatetime.h"
#include "libnsfdb_note.h"
#include "libnsfdb_note_item_class.h"
#include "libnsfdb_note_value.h"
#include "libnsfdb_rrv_value.h"
#include "libnsfdb_types.h"
#include "libnsfdb_unique_name_key.h"

#include "nsfdb_note.h"

/* Creates a note
 * Make sure the value note is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_initialize(
     libnsfdb_note_t **note,
     libbfio_handle_t *file_io_handle,
     libnsfdb_io_handle_t *io_handle,
     libnsfdb_rrv_value_t *rrv_value,
     libfdata_list_t *summary_bucket_list,
     libfcache_cache_t *summary_bucket_cache,
     libfdata_list_t *non_summary_bucket_list,
     libfcache_cache_t *non_summary_bucket_cache,
     libcerror_error_t **error )
{
	libnsfdb_internal_note_t *internal_note = NULL;
	static char *function                   = "libnsfdb_note_initialize";

	if( note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note.",
		 function );

		return( -1 );
	}
	if( *note != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid note value already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
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
	internal_note = memory_allocate_structure(
	                 libnsfdb_internal_note_t );

	if( internal_note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create note.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_note,
	     0,
	     sizeof( libnsfdb_internal_note_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear note.",
		 function );

		memory_free(
		 internal_note );

		return( -1 );
	}
	if( libnsfdb_rrv_value_clone(
	     &( internal_note->rrv_value ),
	     rrv_value,
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
	internal_note->file_io_handle           = file_io_handle;
	internal_note->io_handle                = io_handle;
	internal_note->summary_bucket_list      = summary_bucket_list;
	internal_note->summary_bucket_cache     = summary_bucket_cache;
	internal_note->non_summary_bucket_list  = non_summary_bucket_list;
	internal_note->non_summary_bucket_cache = non_summary_bucket_cache;

	*note = (libnsfdb_note_t *) internal_note;

	return( 1 );

on_error:
	if( internal_note != NULL )
	{
		memory_free(
		 internal_note );
	}
	return( -1 );
}

/* Frees a note
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_free(
     libnsfdb_note_t **note,
     libcerror_error_t **error )
{
	libnsfdb_internal_note_t *internal_note = NULL;
	static char *function                   = "libnsfdb_note_free";
	int result                              = 1;

	if( note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note.",
		 function );

		return( -1 );
	}
	if( *note != NULL )
	{
		internal_note = (libnsfdb_internal_note_t *) *note;
		*note         = NULL;

		/* The file_io_handle, io_handle, summary_bucket_list, summary_bucket_cache
		 * non_summary_bucket_list and non_summary_bucket_cache references are freed elsewhere
		 */
		if( libnsfdb_rrv_value_free(
		     &( internal_note->rrv_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free RRV value.",
			 function );

			result = -1;
		}
		if( internal_note->values != NULL )
		{
			if( libcdata_array_free(
			     &( internal_note->values ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_note_value_free,
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
		}
		memory_free(
		 internal_note );
	}
	return( result );
}

/* Clones a note
 * Return 1 if successful or -1 on error
 */
int libnsfdb_note_clone(
     libnsfdb_note_t **destination_note,
     libnsfdb_note_t *source_note,
     libcerror_error_t **error )
{
	libnsfdb_internal_note_t *internal_source_note = NULL;
	static char *function                          = "libnsfdb_note_clone";

	if( destination_note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination note.",
		 function );

		return( -1 );
	}
	if( *destination_note != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination note already set.",
		 function );

		return( -1 );
	}
	if( source_note == NULL )
	{
		*destination_note = NULL;

		return( 1 );
	}
	internal_source_note = (libnsfdb_internal_note_t *) source_note;

	if( libnsfdb_note_initialize(
	     destination_note,
	     internal_source_note->file_io_handle,
	     internal_source_note->io_handle,
	     internal_source_note->rrv_value,
	     internal_source_note->summary_bucket_list,
	     internal_source_note->summary_bucket_cache,
	     internal_source_note->non_summary_bucket_list,
	     internal_source_note->non_summary_bucket_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination note.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the note
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_read(
     libnsfdb_internal_note_t *internal_note,
     libcerror_error_t **error )
{
	libnsfdb_bucket_t *non_summary_bucket           = NULL;
	libnsfdb_bucket_t *summary_bucket               = NULL;
	libnsfdb_bucket_slot_t *non_summary_bucket_slot = NULL;
	libnsfdb_bucket_slot_t *summary_bucket_slot     = NULL;
	libnsfdb_note_value_t *note_value               = NULL;
	libnsfdb_unique_name_key_t *unique_name_key     = NULL;
	uint8_t *note_data                              = NULL;
	static char *function                           = "libnsfdb_note_read";
	size_t note_data_size                           = 0;
	uint32_t non_summary_data_identifier            = 0;
	uint32_t non_summary_data_size                  = 0;
	uint32_t note_size                              = 0;
	uint16_t note_item_index                        = 0;
	uint16_t note_signature                         = 0;
	uint16_t number_of_note_items                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t nsf_timedate_string[ 32 ];

	libfdatetime_nsf_timedate_t *nsf_timedate       = NULL;
	uint64_t value_64bit                            = 0;
	uint32_t value_32bit                            = 0;
	uint16_t value_16bit                            = 0;
#endif

	if( internal_note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal note.",
		 function );

		return( -1 );
	}
	if( internal_note->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal note - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_note->rrv_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal note - missing RRV value.",
		 function );

		return( -1 );
	}
	if( internal_note->values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid note - values already set.",
		 function );

		return( -1 );
	}
	if( internal_note->rrv_value->type != LIBNSFDB_RRV_VALUE_TYPE_BUCKET_SLOT_IDENTIFIER )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported RRV value type.",
		 function );

		return( -1 );
	}
	if( internal_note->rrv_value->type == LIBNSFDB_RRV_VALUE_TYPE_BUCKET_SLOT_IDENTIFIER )
	{
		if( libnsfdb_bucket_list_get_bucket_by_index(
		     internal_note->summary_bucket_list,
		     internal_note->file_io_handle,
		     internal_note->summary_bucket_cache,
		     internal_note->rrv_value->bucket_index,
		     &summary_bucket,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve summary bucket: %" PRIu32 ".",
			 function,
			 internal_note->rrv_value->bucket_index );

			goto on_error;
		}
		if( libnsfdb_bucket_get_slot(
		     summary_bucket,
		     (int) internal_note->rrv_value->slot_index,
		     &summary_bucket_slot,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve summary bucket: 1 slot: %" PRIu16 ".",
			 function,
			 internal_note->rrv_value->slot_index );

			goto on_error;
		}
		if( summary_bucket_slot == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing summary bucket slot: %" PRIu16 ".",
			 function,
			 internal_note->rrv_value->slot_index );

			goto on_error;
		}
		note_data      = summary_bucket_slot->data;
		note_data_size = (size_t) summary_bucket_slot->size;
	}
	else if( internal_note->rrv_value->type == LIBNSFDB_RRV_VALUE_TYPE_FILE_POSITION )
	{
		if( note_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid note data size value exceeds maximum.",
			 function );

			goto on_error;
		}
	}
	if( note_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note data.",
		 function );

		goto on_error;
	}
	if( note_data_size < sizeof( nsfdb_note_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid note data size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: note header:\n",
		 function );
		libcnotify_print_data(
		 note_data,
		 sizeof( nsfdb_note_header_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (nsfdb_note_header_t *) note_data )->signature,
	 note_signature );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_note_header_t *) note_data )->size,
	 note_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (nsfdb_note_header_t *) note_data )->note_class,
	 internal_note->note_class );

	byte_stream_copy_to_uint16_little_endian(
	 ( (nsfdb_note_header_t *) note_data )->number_of_note_items,
	 number_of_note_items );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_note_header_t *) note_data )->non_summary_data_identifier,
	 non_summary_data_identifier );

	byte_stream_copy_to_uint32_little_endian(
	 ( (nsfdb_note_header_t *) note_data )->non_summary_data_size,
	 non_summary_data_size );

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
		 note_signature );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 note_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->rrv_identifier,
		 value_32bit );

		libcnotify_printf(
		 "%s: RRV identifier\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->file_identifier,
		 value_64bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     ( (nsfdb_note_header_t *) note_data )->file_identifier,
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
		 "%s: file identifier\t\t\t\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%s)\n",
		 function,
		 value_64bit >> 32,
		 value_64bit & 0xffffffffUL,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint64_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->note_identifier,
		 value_64bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     ( (nsfdb_note_header_t *) note_data )->note_identifier,
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
		 "%s: note identifier\t\t\t\t\t: 0x%08" PRIx64 " - 0x%08" PRIx64 " (%s)\n",
		 function,
		 value_64bit >> 32,
		 value_64bit & 0xffffffffUL,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint32_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->sequence_number,
		 value_32bit );

		libcnotify_printf(
		 "%s: sequence number\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     ( (nsfdb_note_header_t *) note_data )->sequence_time,
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
		 "%s: sequence time\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		byte_stream_copy_to_uint16_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->status_flags,
		 value_16bit );

		libcnotify_printf(
		 "%s: status flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );
		libnsfdb_debug_print_note_status_flags(
		 value_16bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: note class\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 internal_note->note_class );
		libnsfdb_debug_print_note_class_flags(
		 internal_note->note_class );
		libcnotify_printf(
		 "\n" );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     ( (nsfdb_note_header_t *) note_data )->modification_time,
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
		 "%s: number of note items\t\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_note_items );

		byte_stream_copy_to_uint16_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->unknown1,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->number_of_responses,
		 value_16bit );

		libcnotify_printf(
		 "%s: number of responses\t\t\t\t\t: 0x%04" PRIx32 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: non-summary data identifier\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 non_summary_data_identifier );

		libcnotify_printf(
		 "%s: non-summary data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 non_summary_data_size );

		byte_stream_copy_to_uint64_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->file_identifier,
		 value_64bit );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     ( (nsfdb_note_header_t *) note_data )->access_time,
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
		 "%s: access time\t\t\t\t\t\t: %s\n",
		 function,
		 (char *) nsf_timedate_string );

		if( libfdatetime_nsf_timedate_copy_from_byte_stream(
		     nsf_timedate,
		     ( (nsfdb_note_header_t *) note_data )->creation_time,
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
		 "%s: creation time\t\t\t\t\t: %s\n",
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
		byte_stream_copy_to_uint32_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->parent_note_identifier,
		 value_32bit );

		libcnotify_printf(
		 "%s: parent note identifier\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->unknown3,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown3\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->folder_reference_count,
		 value_32bit );

		libcnotify_printf(
		 "%s: folder reference count\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->unknown4,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown4\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->folder_note_identifier,
		 value_32bit );

		libcnotify_printf(
		 "%s: folder note identifier\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (nsfdb_note_header_t *) note_data )->unknown5,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown5\t\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( note_signature != 0x0004 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported note signature.",
		 function );

		goto on_error;
	}
	if( note_size != note_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid note size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( non_summary_data_identifier != 0 )
	 && ( ( non_summary_data_identifier & 0x80000000UL ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported non-summary data identifier.",
		 function );

		goto on_error;
	}
	note_data      += sizeof( nsfdb_note_header_t );
	note_data_size -= sizeof( nsfdb_note_header_t );

	if( libcdata_array_initialize(
	     &( internal_note->values ),
	     number_of_note_items,
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
	if( number_of_note_items > 0 )
	{
		if( note_data_size < ( number_of_note_items * sizeof( nsfdb_note_item_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid note data size value out of bounds.",
			 function );

			goto on_error;
		}
		for( note_item_index = 0;
		     note_item_index < number_of_note_items;
		     note_item_index++ )
		{
			if( libnsfdb_note_value_initialize(
			     &note_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create note value.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint16_little_endian(
			 ( (nsfdb_note_item_t *) note_data )->unk_table_index,
			 note_value->unk_table_index );

			byte_stream_copy_to_uint16_little_endian(
			 ( (nsfdb_note_item_t *) note_data )->field_flags,
			 note_value->field_flags );

			byte_stream_copy_to_uint16_little_endian(
			 ( (nsfdb_note_item_t *) note_data )->data_size,
			 note_value->data_size );

			byte_stream_copy_to_uint16_little_endian(
			 ( (nsfdb_note_item_t *) note_data )->unknown1,
			 note_value->unknown1 );

			if( libcdata_array_set_entry_by_index(
			     internal_note->values,
			     (int) note_item_index,
			     (intptr_t *) note_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set note value: %" PRIu16 ".",
				 function,
				 note_item_index );

				goto on_error;
			}
			note_value = NULL;

			note_data      += sizeof( nsfdb_note_item_t );
			note_data_size -= sizeof( nsfdb_note_item_t );
		}
	}
	if( number_of_note_items > 0 )
	{
		if( ( non_summary_data_identifier != 0 )
		 && ( ( non_summary_data_identifier & 0x80000000UL ) != 0 ) )
		{
			if( libnsfdb_bucket_list_get_bucket_by_index(
			     internal_note->non_summary_bucket_list,
			     internal_note->file_io_handle,
			     internal_note->non_summary_bucket_cache,
			     non_summary_data_identifier & 0x00ffffffUL,
			     &non_summary_bucket,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve non-summary bucket: %" PRIu32 ".",
				 function,
				 non_summary_data_identifier & 0x00ffffffUL );

				goto on_error;
			}
			if( libnsfdb_bucket_get_slot(
			     non_summary_bucket,
			     (int) ( ( non_summary_data_identifier >> 24 ) & 0x7f ),
			     &non_summary_bucket_slot,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve non-summary bucket: 1 slot: %" PRIu32 ".",
				 function,
				 ( non_summary_data_identifier >> 24 ) & 0x7f );

				goto on_error;
			}
			if( non_summary_bucket_slot == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing non-summary bucket slot: %" PRIu32 ".",
				 function,
				 ( non_summary_data_identifier >> 24 ) & 0x7f );

				goto on_error;
			}
			/* TODO */
		}
		for( note_item_index = 0;
		     note_item_index < number_of_note_items;
		     note_item_index++ )
		{
			if( libcdata_array_get_entry_by_index(
			     internal_note->values,
			     (int) note_item_index,
			     (intptr_t **) &note_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve note value: %" PRIu16 ".",
				 function,
				 note_item_index );

				goto on_error;
			}
			if( note_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid note value.",
				 function );

				goto on_error;
			}
			if( libcdata_array_get_entry_by_index(
			     internal_note->io_handle->unique_name_key_table,
			     (int) note_value->unk_table_index,
			     (intptr_t **) &unique_name_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve unique name key: %" PRIu16 ".",
				 function,
				 note_value->unk_table_index );

				note_value = NULL;

				goto on_error;
			}
			if( unique_name_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: invalid unique name key.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: note item: %03" PRIu16 " UNK table index\t\t\t: %" PRIu16 "\n",
				 function,
				 note_item_index,
				 note_value->unk_table_index );

				libcnotify_printf(
				 "%s: note item: %03" PRIu16 " item name\t\t\t\t: %s\n",
				 function,
				 note_item_index,
				 (char*) unique_name_key->name );

				libcnotify_printf(
				 "%s: note item: %03" PRIu16 " note item class\t\t\t: 0x%02" PRIx8 " (%s : %s)\n",
				 function,
				 note_item_index,
				 unique_name_key->note_item_class,
				 libnsfdb_note_item_class_get_identifier(
				  unique_name_key->note_item_class ),
				 libnsfdb_note_item_class_get_description(
				  unique_name_key->note_item_class ) );
				libcnotify_printf(

				 "%s: note item: %03" PRIu16 " note item type\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 note_item_index,
				 unique_name_key->note_item_type );

				libcnotify_printf(
				 "%s: note item: %03" PRIu16 " field flags\t\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 note_item_index,
				 note_value->field_flags );
				libnsfdb_debug_print_note_item_field_flags(
				 note_value->field_flags );
				libcnotify_printf(
				 "\n" );

				libcnotify_printf(
				 "%s: note item: %03" PRIu16 " data size\t\t\t\t: %" PRIu16 "\n",
				 function,
				 note_item_index,
				 note_value->data_size );

				libcnotify_printf(
				 "%s: note item: %03" PRIu16 " unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 note_item_index,
				 note_value->unknown1 );
			}
#endif
			if( ( note_value->field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_SUMMARY ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "\n" );
				}
#endif
				continue;
			}
			if( note_value->data_size == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "\n" );
				}
#endif
				continue;
			}
			if( note_value->data_size > note_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid note data size value out of bounds.",
				 function );

				note_value = NULL;

				goto on_error;
			}
			if( libnsfdb_note_value_set_data(
			     note_value,
			     note_data,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set data in note value: %" PRIu16 ".",
				 function,
				 note_item_index );

				note_value = NULL;

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: note item data: %03" PRIu16 ":\n",
				 function,
				 note_item_index );
				libcnotify_print_data(
				 note_value->data,
				 (size_t) note_value->data_size,
				 0 );
			}
#endif
			note_data      += note_value->data_size;
			note_data_size -= note_value->data_size;

			note_value = NULL;

		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( note_data_size > 0 )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 note_data,
			 note_data_size,
			 0 );
		}
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
	if( note_value != NULL )
	{
		libnsfdb_note_value_free(
		 &note_value,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_note_get_number_of_values(
     libnsfdb_note_t *note,
     int *number_of_values,
     libcerror_error_t **error )
{
	libnsfdb_internal_note_t *internal_note = NULL;
	static char *function                   = "libnsfdb_note_get_number_of_values";

	if( note == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid note.",
		 function );

		return( -1 );
	}
	internal_note = (libnsfdb_internal_note_t *) note;

	if( internal_note->values == NULL )
	{
		if( libnsfdb_note_read(
		     internal_note,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read note.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_number_of_entries(
	     internal_note->values,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	return( 1 );
}

