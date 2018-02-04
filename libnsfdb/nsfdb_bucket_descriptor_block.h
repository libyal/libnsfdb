/*
 * The Bucket Descriptor Block (BDB) definition of a Notes Storage Facility (NSF) database file
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

#if !defined( _NSFDB_BUCKET_DESCRIPTOR_BLOCK_H )
#define _NSFDB_BUCKET_DESCRIPTOR_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct nsfdb_bucket_descriptor_block_header nsfdb_bucket_descriptor_block_header_t;

struct nsfdb_bucket_descriptor_block_header
{
	/* The signature
	 * Consists of 2 bytes
	 * Contains 0x01 0x00
	 */
	uint8_t signature[ 2 ];

	/* The version
	 * Consists of 2 bytes
	 */
	uint8_t version[ 2 ];

	/* The compression type
	 * Consists of 2 bytes
	 */
	uint8_t compression_type[ 2 ];

	/* The uncompressed size
	 * Consists of 4 bytes
	 */
	uint8_t uncompressed_size[ 4 ];

	/* The write count
	 * Consists of 4 bytes
	 */
	uint8_t write_count[ 4 ];

	/* The size of the bucket descriptor block
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* The number of Unique Name Keys (UNKs)
	 * Consists of 4 bytes
	 */
	uint8_t number_of_unique_name_keys[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The Unique Name Key (UNK) text size
	 * Consists of 4 bytes
	 */
	uint8_t unique_name_key_text_size[ 4 ];

	/* The number of Record Relocation Vector (RRV) bucket descriptors
	 * Consists of 4 bytes
	 */
	uint8_t number_of_rrv_bucket_descriptors[ 4 ];

	/* The number of Unique Name Key (UNK) hash table entries
	 * Consists of 4 bytes
	 */
	uint8_t number_of_unk_hash_table_entries[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a 32-bit XOR of the bucket descriptor block header
	 * without the checksum itself
	 */
	uint8_t checksum[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3[ 8 ];
};

typedef struct nsfdb_bucket_descriptor_block_footer nsfdb_bucket_descriptor_block_footer_t;

struct nsfdb_bucket_descriptor_block_footer
{
	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a 32-bit XOR of the bucket descriptor block information data
	 */
	uint8_t checksum[ 4 ];
};

typedef struct nsfdb_summary_bucket_page_descriptor nsfdb_summary_bucket_page_descriptor_t;

struct nsfdb_summary_bucket_page_descriptor
{
	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t largest_number_of_free_bytes;

	/* Second largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t second_largest_number_of_free_bytes;
};

typedef struct nsfdb_summary_bucket_group_descriptor nsfdb_summary_bucket_group_descriptor_t;

struct nsfdb_summary_bucket_group_descriptor
{
	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t largest_number_of_free_bytes;

	/* Second largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t second_largest_number_of_free_bytes;
};

typedef struct nsfdb_summary_bucket_descriptor nsfdb_summary_bucket_descriptor_t;

struct nsfdb_summary_bucket_descriptor
{
	/* File position
	 * Consists of 4 bytes
	 */
	uint8_t file_position[ 4 ];

	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t largest_number_of_free_bytes;

	/* Second largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t second_largest_number_of_free_bytes;
};

typedef struct nsfdb_non_summary_bucket_page_descriptor nsfdb_non_summary_bucket_page_descriptor_t;

struct nsfdb_non_summary_bucket_page_descriptor
{
	/* Largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t largest_number_of_free_bytes;

	/* Second largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t second_largest_number_of_free_bytes;
};

typedef struct nsfdb_non_summary_bucket_group_descriptor nsfdb_non_summary_bucket_group_descriptor_t;

struct nsfdb_non_summary_bucket_group_descriptor
{
	/* Largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t largest_number_of_free_bytes;

	/* Second largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t second_largest_number_of_free_bytes;
};

typedef struct nsfdb_non_summary_bucket_descriptor nsfdb_non_summary_bucket_descriptor_t;

struct nsfdb_non_summary_bucket_descriptor
{
	/* File position
	 * Consists of 4 bytes
	 */
	uint8_t file_position[ 4 ];

	/* Largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t largest_number_of_free_bytes;

	/* Second largest number of free bytes
	 * Consists of 1 byte
	 */
	uint8_t second_largest_number_of_free_bytes;
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _NSFDB_BUCKET_DESCRIPTOR_BLOCK_H ) */

