/*
 * Unique Name Key (UNK) table functions
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"
#include "libnsfdb_note_item_class.h"
#include "libnsfdb_unique_name_key.h"
#include "libnsfdb_unique_name_key_table.h"

/* Reads the Unique Name Key (UNK) table
 * Returns the number of bytes read if successful or -1 on error
 */
ssize_t libnsfdb_unique_name_key_table_read(
         libcdata_array_t *unique_name_key_table,
         uint8_t *unk_data,
         size_t unk_data_size,
         uint32_t number_of_unique_name_keys,
         uint32_t unique_name_key_text_size,
         libcerror_error_t **error )
{
	libnsfdb_unique_name_key_t *unique_name_key = NULL;
	uint8_t *unique_name_key_text               = NULL;
	static char *function                       = "libnsfdb_unique_name_key_table_read";
	size_t unique_name_key_entries_size         = 0;
	ssize_t unique_name_key_data_size           = 0;
	uint32_t unique_name_key_iterator           = 0;
	uint32_t unk_name_offset                    = 0;
	uint16_t unk_name_length                    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                        = 0;
#endif

	if( unique_name_key_table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UNK table.",
		 function );

		return( -1 );
	}
	if( unk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UNK data.",
		 function );

		return( -1 );
	}
	if( unk_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UNK data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( number_of_unique_name_keys > 0 )
	{
		unique_name_key_entries_size = ( number_of_unique_name_keys * 10 ) + 4;

		if( unique_name_key_entries_size > unk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UNK name key entries size value out of bounds.",
			 function );

			goto on_error;
		}
		unique_name_key_data_size = unique_name_key_entries_size + unique_name_key_text_size;

		if( unique_name_key_data_size > (ssize_t) unk_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UNK name key text size value out of bounds.",
			 function );

			goto on_error;
		}
		unique_name_key_text = unk_data + unique_name_key_entries_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: UNK data:\n",
			 function );
			libcnotify_print_data(
			 unk_data,
			 unique_name_key_entries_size + unique_name_key_text_size,
			 0 );
		}
#endif
		if( libcdata_array_resize(
		     unique_name_key_table,
		     number_of_unique_name_keys,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libnsfdb_unique_name_key_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize unique name key table.",
			 function );

			goto on_error;
		}
		for( unique_name_key_iterator = 0;
		     unique_name_key_iterator < number_of_unique_name_keys;
		     unique_name_key_iterator++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 unk_data,
			 unk_name_offset );

			unk_data += sizeof( uint32_t );

			byte_stream_copy_to_uint16_little_endian(
			 unk_data,
			 unk_name_length );

			unk_data += sizeof( uint16_t );

			if( unk_name_offset > unique_name_key_text_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid UNK name offset value out of bounds.",
				 function );

				goto on_error;
			}
			if( ( unk_name_offset + unk_name_length ) > unique_name_key_text_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid UNK name length value out of bounds.",
				 function );

				goto on_error;
			}
			if( libnsfdb_unique_name_key_initialize(
			     &unique_name_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize unique name key.",
				 function );

				goto on_error;
			}
			unique_name_key->name_size = unk_name_length + 1;

			unique_name_key->name = (uint8_t *) memory_allocate(
			                                     sizeof( uint8_t ) * unique_name_key->name_size );

			if( unique_name_key->name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create UNK name.",
				 function );

				goto on_error;
			}
			if( memory_copy(
			     unique_name_key->name,
			     &( unique_name_key_text[ unk_name_offset ] ),
			     unk_name_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy UNK name.",
				 function );

				goto on_error;
			}
			unique_name_key->name[ unk_name_length ] = 0;

			unique_name_key->note_item_type = *unk_data;

			unk_data += 1;

			unique_name_key->note_item_class = *unk_data;

			unk_data += 1;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: UNK entry: %04" PRIu32 " name offset\t\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
				 function,
				 unique_name_key_iterator,
				 unk_name_offset,
				 unk_name_offset );

				libcnotify_printf(
				 "%s: UNK entry: %04" PRIu32 " name length\t\t\t: %" PRIu16 "\n",
				 function,
				 unique_name_key_iterator,
				 unk_name_length );

				libcnotify_printf(
				 "%s: UNK entry: %04" PRIu32 " name\t\t\t\t: %s\n",
				 function,
				 unique_name_key_iterator,
				 (char *) unique_name_key->name );

				libcnotify_printf(
				 "%s: UNK entry: %04" PRIu32 " note item type\t\t\t: 0x%02" PRIx8 "\n",
				 function,
				 unique_name_key_iterator,
				 unique_name_key->note_item_type );

				libcnotify_printf(
				 "%s: UNK entry: %04" PRIu32 " note item class\t\t\t: 0x%02" PRIx8 " (%s : %s)\n",
				 function,
				 unique_name_key_iterator,
				 unique_name_key->note_item_class,
				 libnsfdb_note_item_class_get_identifier(
				  unique_name_key->note_item_class ),
				 libnsfdb_note_item_class_get_description(
				  unique_name_key->note_item_class ) );
			}
#endif

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint16_little_endian(
				 unk_data,
				 value_16bit );

				libcnotify_printf(
				 "%s: UNK entry: %04" PRIu32 " unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
				 function,
				 unique_name_key_iterator,
				 value_16bit );
			}
#endif
			unk_data += 2;

			if( libcdata_array_set_entry_by_index(
			     unique_name_key_table,
			     (int) unique_name_key_iterator,
			     (intptr_t *) unique_name_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set unique name key table entry: %" PRIu32 ".",
				 function,
				 unique_name_key_iterator );

				goto on_error;
			}
			unique_name_key = NULL;
		}
		/* TODO 4 byte text size ? */
		unk_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	return( unique_name_key_data_size );

on_error:
	if( unique_name_key != NULL )
	{
		libnsfdb_unique_name_key_free(
		 &unique_name_key,
		 NULL );
	}
	return( -1 );
}

