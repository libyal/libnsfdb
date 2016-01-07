/*
 * Debug functions
 *
 * Copyright (C) 2010-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBNSFDB_DEBUG_H )
#define _LIBNSFDB_DEBUG_H

#include <common.h>
#include <types.h>

#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

void libnsfdb_debug_print_database_flags(
      uint16_t database_flags );

void libnsfdb_debug_print_replication_flags(
      uint16_t replication_flags );

void libnsfdb_debug_print_note_class_flags(
      uint16_t note_class_flags );

void libnsfdb_debug_print_note_status_flags(
      uint16_t note_statusflags );

void libnsfdb_debug_print_note_item_field_flags(
      uint16_t note_item_field_flags );

int libnsfdb_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#endif

#if defined( __cplusplus )
}
#endif

#endif

