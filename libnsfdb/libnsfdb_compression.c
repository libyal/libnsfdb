/*
 * Compression functions
 *
 * Copyright (C) 2010-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <types.h>

#include "libnsfdb_compression.h"
#include "libnsfdb_libcerror.h"

/* Decompressed CX (Huffman encoded) compressed data
 * Returns 1 on success or -1 on error
 */
int libnsfdb_compression_cx_decompress(
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data,
     size_t compressed_data_size,
     libcerror_error_t **error )
{
	static char *function             = "libnsfdb_compression_cx_decompress";
	size_t compressed_data_iterator   = 0;
	size_t uncompressed_data_iterator = 0;
	uint16_t compression_tuple_offset = 0;
	uint16_t compression_tuple_size   = 0;
	uint16_t value_16bit              = 0;
	uint8_t tag_bit_index             = 0;
	uint8_t tag_bit                   = 0;
	uint8_t tuple_value_size          = 0;

	if( uncompressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid uncompressed data.",
		 function );

		return( -1 );
	}
	if( uncompressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid uncompressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( compressed_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( compressed_data[ compressed_data_iterator ] & 0x07 ) <= 3 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported compressed data.",
		 function );

		return( -1 );
	}
	tag_bit_index = 3;

	while( compressed_data_iterator < compressed_data_size )
	{
		if( compressed_data_iterator >= compressed_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
			 "%s: compressed data size value too small.",
			 function );

			return( -1 );
		}
		tag_bit = ( compressed_data[ compressed_data_iterator ] >> tag_bit_index ) & 0x01;

		tag_bit_index++;

		if( tag_bit_index >= 8 )
		{
			tag_bit_index = 0;

			compressed_data_iterator++;
		}
		/* If the first tag bit is not set an uncompressed byte is stored after the tag bit
		 */
		if( tag_bit == 0 )
		{
			if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: compressed data size value too small.",
				 function );

				return( -1 );
			}
			value_16bit   = compressed_data[ compressed_data_iterator + 1 ];
			value_16bit <<= 8 ;
			value_16bit  |= compressed_data[ compressed_data_iterator ];
			value_16bit >>= tag_bit_index;

			compressed_data_iterator++;

			if( uncompressed_data_iterator >= uncompressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: uncompressed data size value too small.",
				 function );

				return( -1 );
			}
			uncompressed_data[ uncompressed_data_iterator ] = (uint8_t) ( value_16bit & 0xff );

			uncompressed_data_iterator++;
		}
		else
		{
			if( compressed_data_iterator >= compressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: compressed data size value too small.",
				 function );

				return( -1 );
			}
			tag_bit = ( compressed_data[ compressed_data_iterator ] >> tag_bit_index ) & 0x01;

			tag_bit_index++;

			if( tag_bit_index >= 8 )
			{
				tag_bit_index = 0;

				compressed_data_iterator++;
			}
			compression_tuple_offset = 0;
			compression_tuple_size   = 0;

			/* If the second tag bit is not set a compression tuple is stored
			 * after the tag bit
			 */
			if( tag_bit == 0 )
			{
				if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				value_16bit   = compressed_data[ compressed_data_iterator + 1 ];
				value_16bit <<= 8 ;
				value_16bit  |= compressed_data[ compressed_data_iterator ];
				value_16bit >>= tag_bit_index;

				value_16bit &= 0xff;

				/* The number of 0 bits make up the compression tuple size
				 */
				value_16bit      |= 0x100;
				tuple_value_size  = 1;

				for( value_16bit |= 0x100;
				     ( value_16bit & 0x01 ) == 0;
				     value_16bit >>= 1 )
				{
					tuple_value_size++;
				}
				tag_bit_index += tuple_value_size;

				if( tag_bit_index >= 8 )
				{
					tag_bit_index &= 0x07;

					compressed_data_iterator++;
				}
				if( compressed_data_iterator >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				value_16bit = compressed_data[ compressed_data_iterator ] >> tag_bit_index;

				tag_bit_index += tuple_value_size;

				if( ( tag_bit_index >= 8 )
				 && ( ( compressed_data_iterator + 1 ) < compressed_data_size ) )
				{
					value_16bit |= compressed_data[ compressed_data_iterator + 1 ]
					            << ( 8 + tuple_value_size - tag_bit_index );

					tag_bit_index &= 0x07;

					compressed_data_iterator++;
				}
				compression_tuple_size  = ( 1 << tuple_value_size ) - 1;
				compression_tuple_size += value_16bit & compression_tuple_size;
			}
			compression_tuple_size += 2;

			/* A size larger than or equal to 0x0102 seems to signify the end of the
			 * compressed data
			 */
			if( compression_tuple_size >= 0x0102 )
			{
				break;
			}
			if( compression_tuple_size > 2 )
			{
				if( compressed_data_iterator >= compressed_data_size )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: compressed data size value too small.",
					 function );

					return( -1 );
				}
				tag_bit = ( compressed_data[ compressed_data_iterator ] >> tag_bit_index ) & 0x01;

				tag_bit_index++;

				if( tag_bit_index >= 8 )
				{
					tag_bit_index = 0;

					compressed_data_iterator++;
				}
				/* If the compression tuple tag bit is not set the upper bits (> 8) of
				 * an (extended) compression offset is stored after the tag bit
				 */
				if( tag_bit == 0 )
				{
					if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: compressed data size value too small.",
						 function );

						return( -1 );
					}
					value_16bit   = compressed_data[ compressed_data_iterator + 1 ];
					value_16bit <<= 8 ;
					value_16bit  |= compressed_data[ compressed_data_iterator ];
					value_16bit >>= tag_bit_index;

					value_16bit &= 0xff;

					/* The number of 0 bits make up the compression tuple size
					 */
					value_16bit      |= 0x100;
					tuple_value_size  = 1;

					for( value_16bit |= 0x100;
					     ( value_16bit & 0x01 ) == 0;
					     value_16bit >>= 1 )
					{
						tuple_value_size++;
					}
					tag_bit_index += tuple_value_size;

					if( tag_bit_index >= 8 )
					{
						tag_bit_index &= 0x07;

						compressed_data_iterator++;
					}
					if( compressed_data_iterator >= compressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: compressed data size value too small.",
						 function );

						return( -1 );
					}
					value_16bit = compressed_data[ compressed_data_iterator ] >> tag_bit_index;

					tag_bit_index += tuple_value_size;

					if( ( tag_bit_index >= 8 )
					 && ( ( compressed_data_iterator + 1 ) < compressed_data_size ) )
					{
						value_16bit |= compressed_data[ compressed_data_iterator + 1 ]
							    << ( 8 + tuple_value_size - tag_bit_index );

						tag_bit_index &= 0x07;

						compressed_data_iterator++;
					}
					compression_tuple_offset   = ( 1 << tuple_value_size ) - 1;
					compression_tuple_offset  += value_16bit & compression_tuple_offset;
					compression_tuple_offset <<= 8;
				}
			}
			if( ( compressed_data_iterator + 1 ) >= compressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: compressed data size value too small.",
				 function );

				return( -1 );
			}
			value_16bit   = compressed_data[ compressed_data_iterator + 1 ];
			value_16bit <<= 8 ;
			value_16bit  |= compressed_data[ compressed_data_iterator ];
			value_16bit >>= tag_bit_index;

			compressed_data_iterator++;

			compression_tuple_offset |= (uint8_t) ( value_16bit & 0xff );

			if( compression_tuple_offset > uncompressed_data_iterator )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compression tuple offset value out of bounds.",
				 function );

				return( -1 );
			}
			if( ( uncompressed_data_iterator + compression_tuple_size ) >= uncompressed_data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: uncompressed data size value too small.",
				 function );

				return( -1 );
			}
			while( compression_tuple_size > 0 )
			{
				uncompressed_data[ uncompressed_data_iterator ] = uncompressed_data[ uncompressed_data_iterator - compression_tuple_offset ];

				uncompressed_data_iterator++;

				compression_tuple_size--;
			}
		}
	}
	return( 1 );
}

