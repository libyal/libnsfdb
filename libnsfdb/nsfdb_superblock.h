/*
 * The superblock definition of a Notes Storage Facility (NSF) database file
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

#if !defined( _NSFDB_SUPERBLOCK_H )
#define _NSFDB_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct nsfdb_superblock_header nsfdb_superblock_header_t;

struct nsfdb_superblock_header
{
	/* The signature
	 * Consists of 2 bytes
	 * Contains 0x0e 0x00
	 */
	uint8_t signature[ 2 ];

	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* The uncompressed size
	 * Consists of 4 bytes
	 */
	uint8_t uncompressed_size[ 4 ];

	/* The number of summary buckets
	 * Consists of 4 bytes
	 */
	uint8_t number_of_summary_buckets[ 4 ];

	/* The number of non-summary buckets
	 * Consists of 4 bytes
	 */
	uint8_t number_of_non_summary_buckets[ 4 ];

	/* The number of bitmaps
	 * Consists of 4 bytes
	 */
	uint8_t number_of_bitmaps[ 4 ];

	/* The Record Relocation Vector (RRV) bucket size
	 * Consists of 4 bytes
	 */
	uint8_t rrv_bucket_size[ 4 ];

	/* The data Record Relocation Vector (RRV) bucket position
	 * Consists of 4 bytes
	 */
	uint8_t data_rrv_bucket_position[ 4 ];

	/* The Record Relocation Vector (RRV) identifier low
	 * Consists of 4 bytes
	 */
	uint8_t rrv_identifier_low[ 4 ];

	/* The Record Relocation Vector (RRV) identifier high
	 * Consists of 4 bytes
	 */
	uint8_t rrv_identifier_high[ 4 ];

	/* The bitmap size
	 * Consists of 4 bytes
	 */
	uint8_t bitmap_size[ 4 ];

	/* The data note identifier table size
	 * Consists of 4 bytes
	 */
	uint8_t data_note_identifier_table_size[ 4 ];

	/* The modified note log size
	 * Consists of 4 bytes
	 */
	uint8_t modified_note_log_size[ 4 ];

	/* The Folder Directory Object (FDO) size
	 * Consists of 4 bytes
	 */
	uint8_t folder_directory_object_size[ 4 ];

	/* Flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The write count
	 * Consists of 4 bytes
	 */
	uint8_t write_count[ 4 ];

	/* The size of the superblock
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* The compression type
	 * Consists of 2 bytes
	 */
	uint8_t compression_type[ 2 ];

	/* The number of summary bucket descriptor pages
	 * Consists of 4 bytes
	 */
	uint8_t number_of_summary_bucket_descriptor_pages[ 4 ];

	/* The number of non-summary bucket descriptor pages
	 * Consists of 4 bytes
	 */
	uint8_t number_of_non_summary_bucket_descriptor_pages[ 4 ];

	/* The number of soft deleted note entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_soft_deleted_note_entries[ 4 ];

	/* The shared template information size
	 * Consists of 2 bytes
	 */
	uint8_t shared_template_information_size[ 2 ];

	/* Unknown (reserved)
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The number of form names
	 * Consists of 2 bytes
	 */
	uint8_t number_of_form_names[ 2 ];

	/* The form bitmap size
	 * Consists of 4 bytes
	 */
	uint8_t form_bitmap_size[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];
};

typedef struct nsfdb_superblock_footer nsfdb_superblock_footer_t;

struct nsfdb_superblock_footer
{
	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a 32-bit XOR of the superblock information data
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _NSFDB_SUPERBLOCK_H ) */

