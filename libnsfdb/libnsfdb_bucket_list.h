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

#if !defined( _LIBNSFDB_BUCKET_LIST_H )
#define _LIBNSFDB_BUCKET_LIST_H

#include <common.h>
#include <types.h>

#include "libnsfdb_bucket.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libfcache.h"
#include "libnsfdb_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libnsfdb_bucket_list_get_bucket_by_index(
     libfdata_list_t *bucket_list,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *bucket_cache,
     uint32_t bucket_index,
     libnsfdb_bucket_t **bucket,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_BUCKET_LIST_H ) */

