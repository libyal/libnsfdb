/*
 * Note item type functions
 *
 * Copyright (C) 2010-2025, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libnsfdb_note_item_type.h"

#if defined( HAVE_DEBUG_OUTPUT )

libnsfdb_note_item_type_t libnsfdb_note_item_types[ ] = {
	{ 0x00, "CLASS_NOCOMPUTE",	"No compute" },

	{ 0x03, "CLASS_NUMBER",		"Numeric" },
	{ 0x04, "CLASS_TIME",		"Date and time" },
	{ 0x05, "CLASS_TEXT",		"Text" },
	{ 0x06, "CLASS_Formula",	"Formula" },

	{ (uint8_t) -1, "_UNKNOWN_", "Unknown" } };

/* Retrieves a string containing the note item type identifier
 */
const char *libnsfdb_note_item_type_get_identifier(
             uint8_t note_item_class,
             uint8_t note_item_type )
{
	int iterator = 0;

	while( ( libnsfdb_note_item_types[ iterator ] ).note_item_type != (uint8_t) -1 )
	{
		if( ( libnsfdb_note_item_types[ iterator ] ).note_item_type == note_item_type )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libnsfdb_note_item_types[ iterator ] ).identifier );
}

/* Retrieves a string containing the note item type description
 */
const char *libnsfdb_note_item_type_get_description(
             uint8_t note_item_class,
             uint8_t note_item_type )
{
	int iterator = 0;

	while( ( libnsfdb_note_item_types[ iterator ] ).note_item_type != (uint8_t) -1 )
	{
		if( ( libnsfdb_note_item_types[ iterator ] ).note_item_type == note_item_type )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libnsfdb_note_item_types[ iterator ] ).description );
}

#endif

