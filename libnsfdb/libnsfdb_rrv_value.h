/*
 * Record Relocation Vector (RRV) value functions
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

#if !defined( _LIBNSFDB_RRV_VALUE_H )
#define _LIBNSFDB_RRV_VALUE_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBNSFDB_RRV_VALUE_TYPES
{
	LIBNSFDB_RRV_VALUE_TYPE_BUCKET_SLOT_IDENTIFIER	= (uint8_t) 'i',
	LIBNSFDB_RRV_VALUE_TYPE_FILE_POSITION		= (uint8_t) 'p'
};

typedef struct libnsfdb_rrv_value libnsfdb_rrv_value_t;

struct libnsfdb_rrv_value
{
	/* The type
	 */
	uint8_t type;

	/* The RRV identifier
	 */
	uint32_t rrv_identifier;

	union
	{
		/* The RRV file offset
		 */
		off64_t file_offset;

		struct
		{
			/* The bucket index
			 */
			uint32_t bucket_index;

			/* The slot index
			 */
			uint16_t slot_index;

			/* TODO */

			/* Unknown non-summary
			 */
			uint32_t nonsum;
		};
	};
};

int libnsfdb_rrv_value_initialize(
     libnsfdb_rrv_value_t **rrv_value,
     uint32_t rrv_identifier,
     libcerror_error_t **error );

int libnsfdb_rrv_value_free(
     libnsfdb_rrv_value_t **rrv_value,
     libcerror_error_t **error );

int libnsfdb_rrv_value_clone(
     libnsfdb_rrv_value_t **destination_rrv_value,
     libnsfdb_rrv_value_t *source_rrv_value,
     libcerror_error_t **error );

int libnsfdb_rrv_value_set_bucket_slot_identifier(
     libnsfdb_rrv_value_t *rrv_value,
     uint32_t bucket_index,
     uint16_t slot_index,
     uint32_t nonsum,
     libcerror_error_t **error );

int libnsfdb_rrv_value_set_file_position(
     libnsfdb_rrv_value_t *rrv_value,
     uint32_t file_position,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_RRV_VALUE_H ) */

