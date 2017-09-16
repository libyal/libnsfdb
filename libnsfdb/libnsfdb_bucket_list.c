/*
 * Bucket list functions
 *
 * Copyright (C) 2010-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libnsfdb_bucket.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"

/* Retrieves a specific summary or non summary bucket
 * Buckets are numbered from 1 to N
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_bucket_list_get_bucket_by_index(
     libfdata_list_t *bucket_list,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *bucket_cache,
     uint32_t bucket_index,
     libnsfdb_bucket_t **bucket,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_bucket_list_get_bucket_by_index";

	if( bucket_index == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid bucket index value zero or less.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     bucket_list,
	     (intptr_t *) file_io_handle,
	     bucket_cache,
	     (int) bucket_index - 1,
	     (intptr_t **) bucket,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve bucket: %" PRIu32 ".",
		 function,
		 bucket_index );

		return( -1 );
	}
	return( 1 );
}

