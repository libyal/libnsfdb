/*
 * The Bucket definition of a Notes Storage Facility (NSF) database file
 *
 * Copyright (C) 2010-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _NSFDB_BUCKET_H )
#define _NSFDB_BUCKET_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct nsfdb_bucket_header nsfdb_bucket_header_t;

struct nsfdb_bucket_header
{
	/* The signature
	 * Consists of 1 byte
	 * Contains 0x02
	 */
	uint8_t signature;

	/* The header size
	 * Consists of 1 byte
	 */
	uint8_t header_size;

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The bucket size
	 * Consists of 4 bytes
	 */
	uint8_t size[ 4 ];

	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Unknown
	 * Consists of 20 bytes
	 */
	uint8_t unknown2[ 20 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a 32-bit XOR of the bucket header
	 */
	uint8_t checksum[ 4 ];

	/* The number of slots
	 * Consists of 4 bytes
	 */
	uint8_t number_of_slots[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown4[ 2 ];

	/* Footer size
	 * Consists of 4 bytes
	 */
	uint8_t footer_size[ 4 ];

	/* Unknown
	 * Consists of 12 bytes
	 */
	uint8_t unknown5[ 12 ];
};

typedef struct nsfdb_bucket_footer nsfdb_bucket_footer_t;

struct nsfdb_bucket_footer
{
	/* Modification date and time
	 * Consists of 8 bytes
	 * Contains a NSF timedate value
	 */
	uint8_t modification_time[ 8 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a 32-bit XOR of the bucket data
	 */
	uint8_t checksum[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _NSFDB_BUCKET_H ) */

