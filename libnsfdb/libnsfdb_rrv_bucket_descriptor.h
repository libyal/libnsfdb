/*
 * Record Relocation Vector (RRV) bucket decriptor functions
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

#if !defined( _LIBNSFDB_RRV_BUCKET_DESCRIPTOR_H )
#define _LIBNSFDB_RRV_BUCKET_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBNSFDB_RRV_BUCKET_TYPE
{
	LIBNSFDB_RRV_BUCKET_TYPE_DATA			= (uint8_t) 'd',
	LIBNSFDB_RRV_BUCKET_TYPE_NON_DATA		= (uint8_t) 'n'
};

typedef struct libnsfdb_rrv_bucket_descriptor libnsfdb_rrv_bucket_descriptor_t;

struct libnsfdb_rrv_bucket_descriptor
{
	/* The RRV bucket type
	 */
	uint8_t type;

	/* The initial RRV identifier
	 */
	uint32_t initial_rrv_identifier;

	/* The RRV bucket file offset
	 */
	off64_t file_offset;
};

int libnsfdb_rrv_bucket_descriptor_initialize(
     libnsfdb_rrv_bucket_descriptor_t **rrv_bucket_descriptor,
     libcerror_error_t **error );

int libnsfdb_rrv_bucket_descriptor_free(
     libnsfdb_rrv_bucket_descriptor_t **rrv_bucket_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_RRV_BUCKET_DESCRIPTOR_H ) */

