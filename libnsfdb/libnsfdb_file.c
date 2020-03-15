/*
 * File functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libnsfdb_bucket.h"
#include "libnsfdb_debug.h"
#include "libnsfdb_definitions.h"
#include "libnsfdb_file.h"
#include "libnsfdb_io_handle.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"
#include "libnsfdb_note.h"
#include "libnsfdb_rrv_value.h"
#include "libnsfdb_types.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_initialize(
     libnsfdb_file_t **file,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libnsfdb_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libnsfdb_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		goto on_error;
	}
	if( libnsfdb_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create io handle.",
		 function );

		goto on_error;
	}
	*file = (libnsfdb_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_free(
     libnsfdb_file_t **file,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_free";
	int result                              = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libnsfdb_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
			if( libnsfdb_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

		if( libnsfdb_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free io handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals the libnsfdb file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_signal_abort(
     libnsfdb_file_t *file,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_file_signal_abort";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	( (libnsfdb_internal_file_t *) file )->abort = 1;

	return( 1 );
}

/* Opens a Notes Storage Facility file
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_open(
     libnsfdb_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_open";
	size_t filename_length                  = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBNSFDB_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBNSFDB_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBNSFDB_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = narrow_string_length(
	                   filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libnsfdb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a Notes Storage Facility file
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_open_wide(
     libnsfdb_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_open_wide";
	size_t filename_length                  = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBNSFDB_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBNSFDB_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBNSFDB_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = wide_string_length(
	                   filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libnsfdb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a Notes Storage Facility file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_open_file_io_handle(
     libnsfdb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_open_file_io_handle";
	int bfio_access_flags                   = 0;
	int file_io_handle_is_open              = 0;
	int file_io_handle_opened_in_library    = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
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
	if( ( ( access_flags & LIBNSFDB_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBNSFDB_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBNSFDB_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBNSFDB_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to determine if file IO handle is open.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	if( libnsfdb_file_open_read(
	     internal_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		goto on_error;
	}
	internal_file->file_io_handle                   = file_io_handle;
	internal_file->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

	return( 1 );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a Notes Storage Facility file
 * Returns 0 if successful or -1 on error
 */
int libnsfdb_file_close(
     libnsfdb_file_t *file,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_close";
	int result                              = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libnsfdb_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	if( libnsfdb_io_handle_clear(
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( internal_file->summary_bucket_list != NULL )
	{
		if( libfdata_list_free(
		     &( internal_file->summary_bucket_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free summary bucket list.",
			 function );

			result = -1;
		}
	}
	if( internal_file->summary_bucket_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( internal_file->summary_bucket_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free summary bucket cache.",
			 function );

			result = -1;
		}
	}
	if( internal_file->non_summary_bucket_list != NULL )
	{
		if( libfdata_list_free(
		     &( internal_file->non_summary_bucket_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free non-summary bucket list.",
			 function );

			result = -1;
		}
	}
	if( internal_file->non_summary_bucket_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( internal_file->non_summary_bucket_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free non-summary bucket cache.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Opens a Notes Storage Facility file for reading
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_open_read(
     libnsfdb_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function                   = "libnsfdb_file_open_read";
	uint32_t database_header_size           = 0;
	off64_t bucket_descriptor_block1_offset = 0;
	off64_t bucket_descriptor_block2_offset = 0;
	off64_t data_rrv_bucket_offset          = 0;
	off64_t non_data_rrv_bucket_offset      = 0;
	off64_t superblock1_offset              = 0;
	off64_t superblock2_offset              = 0;
	off64_t superblock3_offset              = 0;
	off64_t superblock4_offset              = 0;
	uint32_t bucket_descriptor_block1_size  = 0;
	uint32_t bucket_descriptor_block2_size  = 0;
	uint32_t superblock1_size               = 0;
	uint32_t superblock2_size               = 0;
	uint32_t superblock3_size               = 0;
	uint32_t superblock4_size               = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing io handle.",
		 function );

		return( -1 );
	}
	if( internal_file->summary_bucket_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - summary bucket list already set.",
		 function );

		return( -1 );
	}
	if( internal_file->summary_bucket_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - summary bucket cache already set.",
		 function );

		return( -1 );
	}
	if( internal_file->non_summary_bucket_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - non summary bucket list already set.",
		 function );

		return( -1 );
	}
	if( internal_file->non_summary_bucket_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - non summary bucket cache already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libnsfdb_io_handle_read_file_header(
	     internal_file->io_handle,
	     file_io_handle,
	     &database_header_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading database header:\n" );
	}
#endif
	if( libnsfdb_io_handle_read_database_header(
	     internal_file->io_handle,
	     file_io_handle,
	     database_header_size,
	     &superblock1_offset,
	     &superblock1_size,
	     &superblock2_offset,
	     &superblock2_size,
	     &superblock3_offset,
	     &superblock3_size,
	     &superblock4_offset,
	     &superblock4_size,
	     &bucket_descriptor_block1_offset,
	     &bucket_descriptor_block1_size,
	     &bucket_descriptor_block2_offset,
	     &bucket_descriptor_block2_size,
	     &non_data_rrv_bucket_offset,
	     &data_rrv_bucket_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read database header.",
		 function );

		goto on_error;
	}
	if( libfdata_list_initialize(
	     &( internal_file->summary_bucket_list ),
	     NULL,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libnsfdb_io_handle_read_bucket,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create summary bucket list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->summary_bucket_cache ),
	     LIBNSFDB_MAXIMUM_CACHE_ENTRIES_BUCKETS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create summary bucket cache.",
		 function );

		goto on_error;
	}
	if( libfdata_list_initialize(
	     &( internal_file->non_summary_bucket_list ),
	     NULL,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libnsfdb_io_handle_read_bucket,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create non-summary bucket list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->non_summary_bucket_cache ),
	     LIBNSFDB_MAXIMUM_CACHE_ENTRIES_BUCKETS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create non-summary bucket cache.",
		 function );

		goto on_error;
	}
	/* TODO determine the last modified superblock
	 * try to read it
	 * fallback to previously modified superblocks if necessary?
	 */

#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading superblock:\n" );
	}
#endif
	if( libnsfdb_io_handle_read_superblock(
	     internal_file->io_handle,
	     file_io_handle,
	     superblock1_offset,
	     superblock1_size,
	     internal_file->summary_bucket_list,
	     internal_file->summary_bucket_cache,
	     internal_file->non_summary_bucket_list,
	     internal_file->non_summary_bucket_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock.",
		 function );

		goto on_error;
	}
#ifdef TODO
/* TODO read secondary superblock */
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading superblock:\n" );
	}
#endif
	if( libnsfdb_io_handle_read_superblock(
	     internal_file->io_handle,
	     file_io_handle,
	     superblock2_offset,
	     superblock2_size,
	     internal_file->summary_bucket_list,
	     internal_file->summary_bucket_cache,
	     internal_file->non_summary_bucket_list,
	     internal_file->non_summary_bucket_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read superblock.",
		 function );

		goto on_error;
	}
#endif
	/* TODO determine the last modified BDB
	 * try to read it
	 * fallback to previously BDB superblocks if necessary?
	 */

#if defined( HAVE_VERBOSE_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading bucket descriptor block:\n" );
	}
#endif
	if( libnsfdb_io_handle_read_bucket_descriptor_block(
	     internal_file->io_handle,
	     file_io_handle,
	     bucket_descriptor_block1_offset,
	     bucket_descriptor_block1_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read bucket descriptor block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_file->non_summary_bucket_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->non_summary_bucket_cache ),
		 NULL );
	}
	if( internal_file->non_summary_bucket_list != NULL )
	{
		libfdata_list_free(
		 &( internal_file->non_summary_bucket_list ),
		 NULL );
	}
	if( internal_file->summary_bucket_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->summary_bucket_cache ),
		 NULL );
	}
	if( internal_file->summary_bucket_list != NULL )
	{
		libfdata_list_free(
		 &( internal_file->summary_bucket_list ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of notes
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_get_number_of_notes(
     libnsfdb_file_t *file,
     uint8_t note_type,
     int *number_of_notes,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	static char *function                   = "libnsfdb_file_get_number_of_notes";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

	if( libnsfdb_io_handle_get_number_of_rrv_values(
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     note_type,
	     number_of_notes,
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

/* Retrieves a specific note
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_file_get_note(
     libnsfdb_file_t *file,
     uint8_t note_type,
     int note_index,
     libnsfdb_note_t **note,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	libnsfdb_rrv_value_t *rrv_value         = NULL;
	static char *function                   = "libnsfdb_file_get_note";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

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
		 "%s: note already set.",
		 function );

		return( -1 );
	}
	if( libnsfdb_io_handle_get_rrv_value_by_index(
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     note_type,
	     note_index,
	     &rrv_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve RRV value: %d.",
		 function,
		 note_index );

		return( -1 );
	}
	if( libnsfdb_note_initialize(
	     note,
	     internal_file->file_io_handle,
	     internal_file->io_handle,
	     rrv_value,
	     internal_file->summary_bucket_list,
	     internal_file->summary_bucket_cache,
	     internal_file->non_summary_bucket_list,
	     internal_file->non_summary_bucket_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create note.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a note for a specific identifier
 * Returns 1 if successful, 0 if no such note was found or -1 on error
 */
int libnsfdb_file_get_note_by_identifier(
     libnsfdb_file_t *file,
     uint8_t note_type,
     uint32_t note_identifier,
     libnsfdb_note_t **note,
     libcerror_error_t **error )
{
	libnsfdb_internal_file_t *internal_file = NULL;
	libnsfdb_rrv_value_t *rrv_value         = NULL;
	static char *function                   = "libnsfdb_file_get_note";
	int result                              = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libnsfdb_internal_file_t *) file;

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
		 "%s: note already set.",
		 function );

		return( -1 );
	}
	result = libnsfdb_io_handle_get_rrv_value_by_identifier(
	          internal_file->io_handle,
	          internal_file->file_io_handle,
	          note_type,
	          note_identifier,
	          &rrv_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve RRV value: 0x%08" PRIx32 ".",
		 function,
		 note_identifier );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libnsfdb_note_initialize(
		     note,
		     internal_file->file_io_handle,
		     internal_file->io_handle,
		     rrv_value,
		     internal_file->summary_bucket_list,
		     internal_file->summary_bucket_cache,
		     internal_file->non_summary_bucket_list,
		     internal_file->non_summary_bucket_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create note.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

