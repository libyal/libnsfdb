/*
 * Unique Name Key (UNK) functions
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNSFDB_UNIQUE_NAME_KEY_H )
#define _LIBNSFDB_UNIQUE_NAME_KEY_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libnsfdb_unique_name_key libnsfdb_unique_name_key_t;

struct libnsfdb_unique_name_key
{
	/* The note item class
	 */
	uint8_t note_item_class;

	/* The note item type
	 */
	uint8_t note_item_type;

	/* TODO store name as binary data and add debug name
	 */

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;
};

int libnsfdb_unique_name_key_initialize(
     libnsfdb_unique_name_key_t **unique_name_key,
     libcerror_error_t **error );

int libnsfdb_unique_name_key_free(
     libnsfdb_unique_name_key_t **unique_name_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_UNIQUE_NAME_KEY_H ) */

