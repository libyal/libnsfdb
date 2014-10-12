/*
 * The database header definitions of a Notes Storage Facility (NSF) database file
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

#if !defined( _NSFDB_DATABASE_HEADER_H )
#define _NSFDB_DATABASE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct nsfdb_database_information nsfdb_database_information_t;

struct nsfdb_database_information
{
	/* The format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* Database identifier (DBID)
	 * Consists of 8 bytes
	 * Contains a NSF timedate value used as an identifier
	 */
	uint8_t database_identifier[ 8 ];

	/* The application version
	 * Consists of 2 bytes
	 */
	uint8_t application_version[ 2 ];

	/* The non-data Record Relocation Vector (RRV) bucket position
	 * Consists of 4 bytes
	 */
	uint8_t non_data_rrv_bucket_position[ 4 ];

	/* The (next) available non-data Record Relocation Vector (RRV) identifier
	 * Consists of 4 bytes
	 */
	uint8_t available_non_data_rrv_identifier[ 4 ];

	/* The number of available non-data Record Relocation Vectors (RRVs)
	 * Consists of 2 bytes
	 */
	uint8_t number_of_available_non_data_rrvs[ 2 ];

	/* The activity log offset
	 * Consists of 4 bytes
	 */
	uint8_t activity_log_offset[ 4 ];

	/* The bucket (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t bucket_modification_time[ 8 ];

	/* The database class
	 * Consists of 2 bytes
	 */
	uint8_t database_class[ 2 ];

	/* The database flags
	 * Consists of 2 bytes
	 */
	uint8_t database_flags[ 2 ];

	/* The Bucket Descriptor Block (BDB) size
	 * Consists of 4 bytes
	 */
	uint8_t bucket_descriptor_block_size[ 4 ];

	/* The Bucket Descriptor Block (BDB) position
	 * Consists of 4 bytes
	 */
	uint8_t bucket_descriptor_block_position[ 4 ];

	/* The Bucket Descriptor Table (BDT) size
	 * Consists of 2 bytes
	 */
	uint8_t bdt_size[ 2 ];

	/* The Bucket Descriptor Table (BDT) position
	 * Consists of 4 bytes
	 */
	uint8_t bdt_position[ 4 ];

	/* The Bucket Descriptor Table (BDT) bitmaps
	 * Consists of 2 bytes
	 */
	uint8_t bdt_bitmaps[ 2 ];

	/* The data Record Relocation Vector (RRV) bucket position
	 * Consists of 4 bytes
	 */
	uint8_t data_rrv_bucket_position[ 4 ];

	/* The first data Record Relocation Vector (RRV) identifier
	 * Consists of 4 bytes
	 */
	uint8_t first_data_rrv_identifier[ 4 ];

	/* The (next) available data Record Relocation Vector (RRV) identifier
	 * Consists of 4 bytes
	 */
	uint8_t available_data_rrv_identifier[ 4 ];

	/* The number of available data Record Relocation Vectors (RRVs)
	 * Consists of 2 bytes
	 */
	uint8_t number_of_available_data_rrvs[ 2 ];

	/* The Record Relocation Vector (RRV) bucket size
	 * Consists of 2 bytes
	 */
	uint8_t rrv_bucket_size[ 2 ];

	/* The summary bucket size
	 * Consists of 2 bytes
	 */
	uint8_t summary_bucket_size[ 2 ];

	/* The bitmap size
	 * Consists of 2 bytes
	 */
	uint8_t bitmap_size[ 2 ];

	/* The allocation granularity
	 * Consists of 2 bytes
	 */
	uint8_t allocation_granularity[ 2 ];

	/* The extention granularity
	 * Consists of 4 bytes
	 */
	uint8_t extention_granularity[ 4 ];

	/* File size
	 * Consists of 4 bytes
	 * Contains a file size in 256 byte increments
	 */
	uint8_t file_size[ 4 ];

	/* The number of file truncations
	 * Consists of 4 bytes
	 */
	uint8_t number_of_file_truncations[ 4 ];

	/* Delivery sequence number
	 * Consists of 4 bytes
	 */
	uint8_t delivery_sequence_number[ 4 ];

	/* The number of Bucket Descriptor Block (BDB) replacements
	 * Consists of 4 bytes
	 */
	uint8_t number_of_bdb_replacements[ 4 ];

	/* The number of allocated Record Relocation Vectors (RRVs)
	 * Consists of 4 bytes
	 */
	uint8_t number_of_allocated_rrvs[ 4 ];

	/* The number of de-allocations
	 * Consists of 4 bytes
	 */
	uint8_t number_of_deallocations[ 4 ];

	/* The number of non-bucket allocations
	 * Consists of 4 bytes
	 */
	uint8_t number_of_non_bucket_allocations[ 4 ];

	/* The number of bucket allocations
	 * Consists of 4 bytes
	 */
	uint8_t number_of_bucket_allocations[ 4 ];

	/* The folder (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t folder_modification_time[ 8 ];

	/* The data note identifier table position
	 * Consists of 4 bytes
	 */
	uint8_t data_note_identifier_table_position[ 4 ];

	/* The data note identifier table size
	 * Consists of 4 bytes
	 */
	uint8_t data_note_identifier_table_size[ 4 ];

	/* The data (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t data_modification_time[ 8 ];

	/* The next purge date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t next_purge_time[ 8 ];

	/* Version 3 named object hash position
	 * Consists of 4 bytes
	 */
	uint8_t named_object_hash_position_version3[ 4 ];

	/* Named object hash position
	 * Consists of 4 bytes
	 */
	uint8_t named_object_hash_position[ 4 ];

	/* The private (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t private_modification_time[ 8 ];

	/* The maximum number of privates
	 * Consists of 2 bytes
	 */
	uint8_t maximum_number_of_privates[ 2 ];

	/* Named object hash version
	 * Consists of 2 bytes
	 */
	uint8_t named_object_hash_version[ 2 ];

	/* The non-data (last) modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t non_data_modification_time[ 8 ];
};

typedef struct nsfdb_database_replication_information nsfdb_database_replication_information_t;

struct nsfdb_database_replication_information
{
	/* Replication identifier
	 * Consists of 8 bytes
	 * Contains a NSF timedate value used as an identifier
	 */
	uint8_t replication_identifier[ 8 ];

	/* Replication flags
	 * Consists of 2 bytes
	 */
	uint8_t replication_flags[ 2 ];

	/* Replication cutoff interval
	 * Consists of 2 bytes
	 * Contains number of days
	 */
	uint8_t replication_cutoff_interval[ 2 ];

	/* Replication cutoff date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t replication_cutoff_time[ 8 ];
};

typedef struct nsfdb_database_header nsfdb_database_header_t;

struct nsfdb_database_header
{
	/* The database information buffer
	 * Consists of 128 bytes
	 */
	uint8_t database_information_buffer[ 128 ];

	/* The special note identifiers
	 * Consists of 128 bytes
	 */
	uint8_t special_note_identifiers[ 128 ];

	/* Unknown (padding)
	 * Consists of 64 bytes
	 */
	uint8_t unknown1[ 64 ];
};

typedef struct nsfdb_database_information2 nsfdb_database_information2_t;

struct nsfdb_database_information2
{
	/* Last fix-up date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t last_fixup_time[ 8 ];

	/* Database quota limit
	 * Consists of 4 bytes
	 */
	uint8_t database_quota_limit[ 4 ];

	/* Database quota warn threshold
	 * Consists of 16 bytes
	 */
	uint8_t database_quota_warn_threshold[ 4 ];

	/* Unknown date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t unknown_time1[ 8 ];

	/* Unknown date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t unknown_time2[ 8 ];

	/* Object store replica identifier
	 * Consists of 8 bytes
	 * Contains a NSF timedate value used as an identifier
	 */
	uint8_t object_store_replica_identifier[ 8 ];

	/* Superblock 1 position
	 * Consists of 4 bytes
	 */
	uint8_t superblock1_position[ 4 ];

	/* Superblock 1 size
	 * Consists of 4 bytes
	 */
	uint8_t superblock1_size[ 4 ];

	/* Superblock 2 position
	 * Consists of 4 bytes
	 */
	uint8_t superblock2_position[ 4 ];

	/* Superblock 2 size
	 * Consists of 4 bytes
	 */
	uint8_t superblock2_size[ 4 ];

	/* Superblock 3 position
	 * Consists of 4 bytes
	 */
	uint8_t superblock3_position[ 4 ];

	/* Superblock 3 size
	 * Consists of 4 bytes
	 */
	uint8_t superblock3_size[ 4 ];

	/* Superblock 4 position
	 * Consists of 4 bytes
	 */
	uint8_t superblock4_position[ 4 ];

	/* Superblock 4 size
	 * Consists of 4 bytes
	 */
	uint8_t superblock4_size[ 4 ];

	/* Maximum extension granularity
	 * Consists of 4 bytes
	 */
	uint8_t maximum_extension_granularity[ 4 ];

	/* Summary bucket granularity
	 * Consists of 2 bytes
	 */
	uint8_t summary_bucket_granularity[ 2 ];

	/* Non-summary bucket granularity
	 * Consists of 2 bytes
	 */
	uint8_t non_summary_bucket_granularity[ 2 ];

	/* Minimum summary bucket size
	 * Consists of 4 bytes
	 */
	uint8_t minimum_summary_bucket_size[ 4 ];

	/* Minimum non-summary bucket size
	 * Consists of 4 bytes
	 */
	uint8_t minimum_non_summary_bucket_size[ 4 ];

	/* Maximum summary bucket size
	 * Consists of 4 bytes
	 */
	uint8_t maximum_summary_bucket_size[ 4 ];

	/* Maximum non-summary bucket size
	 * Consists of 4 bytes
	 */
	uint8_t maximum_non_summary_bucket_size[ 4 ];

	/* Non-summary append size
	 * Consists of 2 bytes
	 */
	uint8_t non_summary_append_size[ 2 ];

	/* Non-summary append factor
	 * Consists of 2 bytes
	 */
	uint8_t non_summary_append_factor[ 2 ];

	/* Summary bucket fill factor
	 * Consists of 2 bytes
	 */
	uint8_t summary_bucket_fill_factor[ 2 ];

	/* Non-summary bucket fill factor
	 * Consists of 2 bytes
	 */
	uint8_t non_summary_bucket_fill_factor[ 2 ];

	/* Bucket Descriptor Block (BDB) 1 size
	 * Consists of 4 bytes
	 */
	uint8_t bucket_descriptor_block1_size[ 4 ];

	/* Bucket Descriptor Block (BDB) 1 position
	 * Consists of 4 bytes
	 */
	uint8_t bucket_descriptor_block1_position[ 4 ];

	/* Bucket Descriptor Block (BDB) 2 size
	 * Consists of 4 bytes
	 */
	uint8_t bucket_descriptor_block2_size[ 4 ];

	/* Bucket Descriptor Block (BDB) 2 position
	 * Consists of 4 bytes
	 */
	uint8_t bucket_descriptor_block2_position[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];
};

typedef struct nsfdb_database_information3 nsfdb_database_information3_t;

struct nsfdb_database_information3
{
	/* Unknown
	 * Consists of 44 bytes
	 */
	uint8_t unknown3[ 44 ];
};

typedef struct nsfdb_database_information4 nsfdb_database_information4_t;

struct nsfdb_database_information4
{
	/* Unknown
	 * Consists of 96 bytes
	 */
	uint8_t unknown9[ 96 ];

	/* Unknown date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t unknown_time7[ 8 ];

	/* Unknown
	 * Consists of 34 bytes
	 */
	uint8_t unknown10[ 34 ];

	/* Unknown date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t unknown_time8[ 8 ];

	/* Unknown date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t unknown_time9[ 8 ];

	/* Unknown
	 * Consists of 24 bytes
	 */
	uint8_t unknown11[ 24 ];

	/* Unknown date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t unknown_time10[ 8 ];

	/* Unknown
	 * Consists of 150 bytes
	 */
	uint8_t unknown12[ 150 ];
};

#if defined( __cplusplus )
}
#endif

#endif

