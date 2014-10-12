/*
 * The note definition of a Notes Storage Facility (NSF) database file
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

#if !defined( _NSFDB_NOTE_H )
#define _NSFDB_NOTE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct nsfdb_note_header nsfdb_note_header_t;

struct nsfdb_note_header
{
	/* The signature
	 * Consists of 2 bytes
	 * Contains 0x0004
	 */
	uint8_t signature[ 2 ];

	/* The size of the note
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* The RRV identifier
	 * Consists of 4 bytes
	 */
	uint8_t rrv_identifier[ 4 ];

	/* File identifier
	 * Consists of 8 bytes
	 * Contains a NSF timedate value used as an identifier
	 */
	uint8_t file_identifier[ 8 ];

	/* Note identifier
	 * Consists of 8 bytes
	 * Contains a NSF timedate value used as an identifier
	 */
	uint8_t note_identifier[ 8 ];

	/* The sequence number
	 * Consists of 4 bytes
	 */
	uint8_t sequence_number[ 4 ];

	/* The sequence date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t sequence_time[ 8 ];

	/* Status flags
	 * Consists of 2 bytes
	 */
	uint8_t status_flags[ 2 ];

	/* The note class
	 * Consists of 2 bytes
	 */
	uint8_t note_class[ 2 ];

	/* The (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* The number of note items
	 * Consists of 2 bytes
	 */
	uint8_t number_of_note_items[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The number of responses
	 * Consists of 2 bytes
	 */
	uint8_t number_of_responses[ 2 ];

	/* The non-summary data identifier
	 * Consists of 4 bytes
	 */
	uint8_t non_summary_data_identifier[ 4 ];

	/* The non-summary data size
	 * Consists of 4 bytes
	 */
	uint8_t non_summary_data_size[ 4 ];

	/* The (last) access date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t access_time[ 8 ];

	/* The (file) creation date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t creation_time[ 8 ];

	/* The parent note identifier
	 * Consists of 4 bytes
	 */
	uint8_t parent_note_identifier[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* The folder reference count
	 * Consists of 4 bytes
	 */
	uint8_t folder_reference_count[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* The folder note identifier
	 * Consists of 4 bytes
	 */
	uint8_t folder_note_identifier[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown5[ 2 ];
};

typedef struct nsfdb_note_item nsfdb_note_item_t;

struct nsfdb_note_item
{
	/* The unique name key (UNK) table index
	 * Consists of 2 bytes
	 */
	uint8_t unk_table_index[ 2 ];

	/* The field flags
	 * Consists of 2 bytes
	 */
	uint8_t field_flags[ 2 ];

	/* The data size
	 * Consists of 2 bytes
	 */
	uint8_t data_size[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif

