/*
 * Record Relocation Vector (RRV) bucket functions
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNSFDB_RRV_BUCKET_H )
#define _LIBNSFDB_RRV_BUCKET_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_rrv_value.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libnsfdb_rrv_bucket libnsfdb_rrv_bucket_t;

struct libnsfdb_rrv_bucket
{
	/* The RRV bucket values (array)
	 */
	libcdata_array_t *values;
};

int libnsfdb_rrv_bucket_initialize(
     libnsfdb_rrv_bucket_t **rrv_bucket,
     libcerror_error_t **error );

int libnsfdb_rrv_bucket_free(
     libnsfdb_rrv_bucket_t **rrv_bucket,
     libcerror_error_t **error );

int libnsfdb_rrv_bucket_read(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     uint8_t *rrv_bucket_data,
     size_t rrv_bucket_data_size,
     libcerror_error_t **error );

int libnsfdb_rrv_bucket_get_number_of_values(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     int *number_of_rrv_values,
     libcerror_error_t **error );

int libnsfdb_rrv_bucket_get_value_by_index(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     int rrv_value_index,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error );

int libnsfdb_rrv_bucket_get_value_by_identifier(
     libnsfdb_rrv_bucket_t *rrv_bucket,
     uint32_t rrv_identifier,
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

