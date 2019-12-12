/*
 * Internal note functions
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

#if !defined( _LIBNSFDB_INTERNAL_NOTE_H )
#define _LIBNSFDB_NOTE_H

#include <common.h>
#include <types.h>

#include "libnsfdb_extern.h"
#include "libnsfdb_io_handle.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"
#include "libnsfdb_rrv_value.h"
#include "libnsfdb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libnsfdb_internal_note libnsfdb_internal_note_t;

struct libnsfdb_internal_note
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libnsfdb_io_handle_t *io_handle;

	/* The RRV value
	 */
	libnsfdb_rrv_value_t *rrv_value;

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

	/* The note class
	 */
	uint16_t note_class;

	/* The values (array)
	 */
	libcdata_array_t *values;
};

int libnsfdb_note_initialize(
     libnsfdb_note_t **note,
     libbfio_handle_t *file_io_handle,
     libnsfdb_io_handle_t *io_handle,
     libnsfdb_rrv_value_t *rrv_value,
     libfdata_list_t *summary_bucket_list,
     libfcache_cache_t *summary_bucket_cache,
     libfdata_list_t *non_summary_bucket_list,
     libfcache_cache_t *non_summary_bucket_cache,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_note_free(
     libnsfdb_note_t **note,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_note_clone(
     libnsfdb_note_t **destination_note,
     libnsfdb_note_t *source_note,
     libcerror_error_t **error );

LIBNSFDB_EXTERN \
int libnsfdb_note_get_number_of_values(
     libnsfdb_note_t *note,
     int *number_of_values,
     libcerror_error_t **error );

int libnsfdb_note_read(
     libnsfdb_internal_note_t *internal_note,
     libcerror_error_t **error );

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
LIBNSFDB_EXTERN \
int libnsfdb_note_get_number_of_values(
     libnsfdb_note_t *note,
     int *number_of_values,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_INTERNAL_NOTE_H ) */

