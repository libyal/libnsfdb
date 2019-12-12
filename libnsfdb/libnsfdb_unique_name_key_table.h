/*
 * Unique Name Key (UNK) table functions
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

#if !defined( _LIBNSFDB_UNIQUE_NAME_KEY_TABLE_H )
#define _LIBNSFDB_UNIQUE_NAME_KEY_TABLE_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libcdata.h"
#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

ssize_t libnsfdb_unique_name_key_table_read(
         libcdata_array_t *unique_name_key_table,
         uint8_t *unk_data,
         size_t unk_data_size,
         uint32_t number_of_unique_name_keys,
         uint32_t unique_name_key_text_size,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBNSFDB_UNIQUE_NAME_KEY_TABLE_H ) */

