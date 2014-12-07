/*
 * File functions
 *
 * Copyright (C) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNSFDB_INTERNAL_FILE_H )
#define _LIBNSFDB_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libnsfdb_extern.h"
#include "libnsfdb_io_handle.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"
#include "libnsfdb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libnsfdb_internal_file libnsfdb_internal_file_t;

struct libnsfdb_internal_file
{
	/* The io handle
	 */
	libnsfdb_io_handle_t *io_handle;

	/* The file io handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* The summary bucket list
	 */
	libfdata_list_t *summary_bucket_list;

	/* The summary bucket cache
	 */
	libfcache_cache_t *summary_bucket_cache;

	/* The non-summary bucket list
	 */
	libfdata_list_t *non_summary_bucket_list;

	/* The non-summary bucket cache
	 */
	libfcache_cache_t *non_summary_bucket_cache;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

LIBNSFDB_EXTERN \
int libnsfdb_file_initialize(
     libnsfdb_file_t **file,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_free(
     libnsfdb_file_t **file,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_signal_abort(
     libnsfdb_file_t *file,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_open(
     libnsfdb_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBNSFDB_EXTERN \
int libnsfdb_file_open_wide(
     libnsfdb_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBNSFDB_EXTERN \
int libnsfdb_file_open_file_io_handle(
     libnsfdb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_close(
     libnsfdb_file_t *file,
     libcerror_error_t **error );

int libnsfdb_file_open_read(
     libnsfdb_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_get_number_of_notes(
     libnsfdb_file_t *file,
     uint8_t note_type,
     int *number_of_notes,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_get_note(
     libnsfdb_file_t *file,
     uint8_t note_type,
     int note_index,
     libnsfdb_note_t **note_item,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_file_get_note_by_identifier(
     libnsfdb_file_t *file,
     uint8_t note_type,
     uint32_t note_identifier,
     libnsfdb_note_t **note_item,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

