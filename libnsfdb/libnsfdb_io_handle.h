/*
 * Input/Output (IO) handle
 *
 * Copyright (C) 2010-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNSFDB_IO_HANDLE_H )
#define _LIBNSFDB_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libnsfdb_bucket.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"
#include "libnsfdb_rrv_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char nsfdb_file_signature[ 2 ];

typedef struct libnsfdb_io_handle libnsfdb_io_handle_t;

struct libnsfdb_io_handle
{
	/* The RRV bucket descriptors (arrays)
	 */
	libcdata_array_t *rrv_bucket_descriptors;

	/* The unique name key (UNK) table (array)
	 */
	libcdata_array_t *unique_name_key_table;

	/* The RRV bucket vector
	 */
	libfdata_vector_t *rrv_bucket_vector;

	/* The RRV bucket cache
	 */
	libfcache_cache_t *rrv_bucket_cache;

	/* The format version
	 */
	uint32_t format_version;

	/* The RRV bucket size
	 */
	uint16_t rrv_bucket_size;

	/* The summary bucket size
	 */
	uint16_t summary_bucket_size;

	/* The minimum summary bucket size
	 */
	uint32_t minimum_summary_bucket_size;

	/* The maximum summary bucket size
	 */
	uint32_t maximum_summary_bucket_size;

	/* The minimum non-summary bucket size
	 */
	uint32_t minimum_non_summary_bucket_size;

	/* The maximum non-summary bucket size
	 */
	uint32_t maximum_non_summary_bucket_size;

	/* The file size
	 */
	size64_t file_size;
};

int libnsfdb_io_handle_initialize(
     libnsfdb_io_handle_t **io_handle,
     libcerror_error_t **error );

int libnsfdb_io_handle_free(
     libnsfdb_io_handle_t **io_handle,
     libcerror_error_t **error );

int libnsfdb_io_handle_clear(
     libnsfdb_io_handle_t *io_handle,
     libcerror_error_t **error );

int libnsfdb_io_handle_read_file_header(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t *database_header_size,
     libcerror_error_t **error );

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
     libcerror_error_t **error );

int libnsfdb_io_handle_read_superblock(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t superblock_offset,
     uint32_t superblock_size,
     libfdata_list_t *summary_bucket_list,
     libfcache_cache_t *summary_bucket_cache,
     libfdata_list_t *non_summary_bucket_list,
     libfcache_cache_t *non_summary_bucket_cache,
     libcerror_error_t **error );

int libnsfdb_io_handle_read_bucket_descriptor_block(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t bucket_descriptor_block_offset,
     uint32_t bucket_descriptor_block_size,
     libcerror_error_t **error );

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
     libcerror_error_t **error );

int libnsfdb_io_handle_read_bucket(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfcache_cache_t *cache,
     int element_file_index,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libnsfdb_io_handle_get_number_of_rrv_values(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t note_type,
     int *number_of_rrv_values,
     libcerror_error_t **error );

int libnsfdb_io_handle_get_rrv_value_by_index(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t note_type,
     int rrv_value_index,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error );

int libnsfdb_io_handle_get_rrv_value_by_identifier(
     libnsfdb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint8_t note_type,
     uint32_t rrv_identifier,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_IO_HANDLE_H ) */

