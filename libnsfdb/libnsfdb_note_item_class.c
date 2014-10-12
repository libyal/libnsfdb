/*
 * Note item class functions
 *
 * Copyright (C) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libnsfdb_definitions.h"
#include "libnsfdb_note_item_class.h"

#if defined( HAVE_DEBUG_OUTPUT )

libnsfdb_note_item_class_t libnsfdb_note_item_classs[ ] = {
	{ LIBNSFDB_NOTE_ITEM_CLASS_NO_COMPUTE,	"CLASS_NOCOMPUTE",	"No compute" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_ERROR,	"CLASS_ERROR",		"Error" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_UNAVAILABLE,	"CLASS_UNAVAILABLE",	"Unavailable" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_NUMBER,	"CLASS_NUMBER",		"Numeric" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_TIME,	"CLASS_TIME",		"Date and time" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_TEXT,	"CLASS_TEXT",		"Text" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_FORMULA,	"CLASS_FORMULA",	"Formula" },
	{ LIBNSFDB_NOTE_ITEM_CLASS_USERID,	"CLASS_USERID",		"User identifier" },

	{ (uint8_t) -1, "_UNKNOWN_", "Unknown" } };

/* Retrieves a string containing the note item class identifier
 */
const char *libnsfdb_note_item_class_get_identifier(
             uint8_t note_item_class )
{
	int iterator = 0;

	while( ( libnsfdb_note_item_classs[ iterator ] ).note_item_class != (uint8_t) -1 )
	{
		if( ( libnsfdb_note_item_classs[ iterator ] ).note_item_class == note_item_class )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libnsfdb_note_item_classs[ iterator ] ).identifier );
}

/* Retrieves a string containing the note item class description
 */
const char *libnsfdb_note_item_class_get_description(
             uint8_t note_item_class )
{
	int iterator = 0;

	while( ( libnsfdb_note_item_classs[ iterator ] ).note_item_class != (uint8_t) -1 )
	{
		if( ( libnsfdb_note_item_classs[ iterator ] ).note_item_class == note_item_class )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libnsfdb_note_item_classs[ iterator ] ).description );
}

#endif

