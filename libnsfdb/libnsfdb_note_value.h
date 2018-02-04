/*
 * Note value functions
 *
 * Copyright (C) 2010-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNSFDB_NOTE_VALUE_H )
#define _LIBNSFDB_NOTE_VALUE_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libnsfdb_note_value libnsfdb_note_value_t;

struct libnsfdb_note_value
{
	/* The unique name key (UNK) table index
	 */
	uint16_t unk_table_index;

	/* The field flags
	 */
	uint16_t field_flags;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	uint16_t data_size;

	/* Unknown
	 */
	uint16_t unknown1;
};

int libnsfdb_note_value_initialize(
     libnsfdb_note_value_t **note_value,
     libcerror_error_t **error );

int libnsfdb_note_value_free(
     libnsfdb_note_value_t **note_value,
     libcerror_error_t **error );

int libnsfdb_note_value_set_data(
     libnsfdb_note_value_t *note_value,
     uint8_t *data,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_NOTE_VALUE_H ) */

