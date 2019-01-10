/*
 * Note item class functions
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

#if !defined( _LIBNSFDB_NOTE_ITEM_CLASS_H )
#define _LIBNSFDB_NOTE_ITEM_CLASS_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

typedef struct libnsfdb_note_item_class libnsfdb_note_item_class_t;

struct libnsfdb_note_item_class
{
	/* The note_item_class
	 */
	uint8_t note_item_class;

	/* The identifier
	 */
	const char *identifier;

	/* The description
	 */
	const char *description;
};

const char *libnsfdb_note_item_class_get_identifier(
             uint8_t note_item_class );

const char *libnsfdb_note_item_class_get_description(
             uint8_t note_item_class );

#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_NOTE_ITEM_CLASS_H ) */

