/*
 * Export handle
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include "log_handle.h"
#include "nsfdbtools_libcerror.h"
#include "nsfdbtools_libnsfdb.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The input file
	 */
	libnsfdb_file_t *input_file;

	/* The number of notes
	 */
	int number_of_notes;

	/* The number of exported notes
	 */
	int number_of_exported_notes;

	/* The target path
	 */
	system_character_t *target_path;

	/* The target path size
	 */
	size_t target_path_size;

	/* The items export path
	 */
	system_character_t *items_export_path;

	/* The items export path size
	 */
	size_t items_export_path_size;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_set_target_path(
     export_handle_t *export_handle,
     const system_character_t *target_path,
     libcerror_error_t **error );

int export_handle_set_export_path(
     export_handle_t *export_handle,
     const system_character_t *base_path,
     size_t base_path_length,
     const system_character_t *suffix,
     size_t suffix_length,
     system_character_t **export_path,
     size_t *export_path_size,
     libcerror_error_t **error );

int export_handle_create_items_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_open_input(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int export_handle_close_input(
     export_handle_t *export_handle,
     libcerror_error_t **error );

/* Note generic export functions
 */
int export_handle_export_note(
     export_handle_t *export_handle,
     libnsfdb_note_t *note,
     int note_index,
     int number_of_notes,
     const system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_note_values(
     export_handle_t *export_handle,
     libnsfdb_note_t *note,
     const system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     libcerror_error_t **error );

/* File export functions
 */
int export_handle_export_notes(
     export_handle_t *export_handle,
     libnsfdb_file_t *file,
     const system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EXPORT_HANDLE_H ) */

