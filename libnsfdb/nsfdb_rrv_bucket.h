/*
 * The Record Relocation Vector (RRV) bucket definition of a Notes Storage Facility (NSF) database file
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

#if !defined( _NSFDB_RRV_BUCKET_H )
#define _NSFDB_RRV_BUCKET_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct nsfdb_rrv_bucket_header nsfdb_rrv_bucket_header_t;

struct nsfdb_rrv_bucket_header
{
	/* The signature
	 * Consists of 1 byte
	 * Contains 0x06
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

	/* Initial RRV identifier
	 * Consists of 4 bytes
	 */
	uint8_t initial_rrv_identifier[ 4 ];

	/* Unknown
	 * Consists of 6 bytes
	 */
	uint8_t unknown2[ 6 ];

	/* Unknown size
	 * Consists of 2 bytes
	 */
	uint8_t unknown_size[ 2 ];

	/* Checksum
	 * Consists of 4 bytes
	 * Contains a 32-bit XOR of the RRV bucket
	 * without the checksum itself
	 */
	uint8_t checksum[ 4 ];

	/* Unknown
	 * Consists of 10 bytes
	 */
	uint8_t unknown3[ 10 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _NSFDB_RRV_BUCKET_H ) */

