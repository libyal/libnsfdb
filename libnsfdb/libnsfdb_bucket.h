/*
 * Bucket functions
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

#if !defined( _LIBNSFDB_BUCKET_H )
#define _LIBNSFDB_BUCKET_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libnsfdb_bucket_index_entry libnsfdb_bucket_index_entry_t;

struct libnsfdb_bucket_index_entry
{
	/* The offset of the slot
	 */
	uint16_t offset;

	/* The size of the slot
	 */
	uint16_t size;
};

typedef struct libnsfdb_bucket_slot libnsfdb_bucket_slot_t;

struct libnsfdb_bucket_slot
{
	/* The reference to the bucket slot data
	 */
	uint8_t *data;

	/* The bucket slot size
	 */
	uint16_t size;
};

typedef struct libnsfdb_bucket libnsfdb_bucket_t;

struct libnsfdb_bucket
{
	/* The bucket number
	 */
	uint32_t bucket_number;

	/* The bucket data
	 */
	uint8_t *data;

	/* The bucket data size
	 */
	uint32_t data_size;

	/* The bucket slots array
	 */
	libcdata_array_t *slots_array;
};

int libnsfdb_bucket_slot_initialize(
     libnsfdb_bucket_slot_t **bucket_slot,
     libcerror_error_t **error );

int libnsfdb_bucket_slot_free(
     libnsfdb_bucket_slot_t **bucket_slot,
     libcerror_error_t **error );

int libnsfdb_bucket_index_entry_initialize(
     libnsfdb_bucket_index_entry_t **bucket_index_entry,
     libcerror_error_t **error );

int libnsfdb_bucket_index_entry_free(
     libnsfdb_bucket_index_entry_t **bucket_index_entry,
     libcerror_error_t **error );

int libnsfdb_bucket_initialize(
     libnsfdb_bucket_t **bucket,
     libcerror_error_t **error );

int libnsfdb_bucket_free(
     libnsfdb_bucket_t **bucket,
     libcerror_error_t **error );

int libnsfdb_bucket_read(
     libnsfdb_bucket_t *bucket,
     libbfio_handle_t *file_io_handle,
     off64_t bucket_offset,
     libcerror_error_t **error );

int libnsfdb_bucket_read_index(
     libcdata_array_t *bucket_index_array,
     uint32_t number_of_bucket_entries,
     uint8_t *bucket_data,
     size_t bucket_data_size,
     libcerror_error_t **error );

int libnsfdb_bucket_read_slots(
     libcdata_array_t *bucket_slots_array,
     libcdata_array_t *bucket_index_array,
     uint8_t *bucket_slots_data,
     size_t bucket_slots_data_size,
     libcerror_error_t **error );

int libnsfdb_bucket_get_number_of_slots(
     libnsfdb_bucket_t *bucket,
     uint32_t *number_of_slots,
     libcerror_error_t **error );

int libnsfdb_bucket_get_slot(
     libnsfdb_bucket_t *bucket,
     uint32_t slot_index,
     libnsfdb_bucket_slot_t **bucket_slot,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_BUCKET_H ) */

