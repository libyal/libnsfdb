/*
 * Debug functions
 *
 * Copyright (C) 2010-2025, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libnsfdb_debug.h"
#include "libnsfdb_definitions.h"
#include "libnsfdb_libbfio.h"
#include "libnsfdb_libcerror.h"
#include "libnsfdb_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the database flags
 */
void libnsfdb_debug_print_database_flags(
      uint16_t database_flags )
{
}

/* Prints the replication flags
 */
void libnsfdb_debug_print_replication_flags(
      uint16_t replication_flags )
{
	if( ( replication_flags & LIBNSFDB_REPLICATION_FLAG_DISABLE ) != 0 )
	{
		libcnotify_printf(
		 "\tDisable (REPLFLG_DISABLE)\n" );
	}
	if( ( replication_flags & LIBNSFDB_REPLICATION_FLAG_MARK_NEW_AS_UNREAD ) != 0 )
	{
		libcnotify_printf(
		 "\tMark new as unread (REPLFLG_UNREADIFFNEW)\n" );
	}
	if( ( replication_flags & LIBNSFDB_REPLICATION_FLAG_IGNORE_DELETES ) != 0 )
	{
		libcnotify_printf(
		 "\tIgnore deleted (REPLFLG_IGNORE_DELETES)\n" );
	}
	if( ( replication_flags & 0x0020 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_HIDDEN_DESIGN)\n" );
	}
	if( ( replication_flags & 0x0040 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_DO_NOT_CATALOG)\n" );
	}
	if( ( replication_flags & 0x0080 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_CUTOFF_DELETE)\n" );
	}
	if( ( replication_flags & 0x0100 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_NEVER_REPLICATE)\n" );
	}
	if( ( replication_flags & 0x0200 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_ABSTRACT)\n" );
	}
	if( ( replication_flags & 0x0400 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_DO_NOT_BROWSE)\n" );
	}
	if( ( replication_flags & 0x0800 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_NO_CHRONOS)\n" );
	}
	if( ( replication_flags & 0x1000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_IGNORE_DEST_DELETES)\n" );
	}
	if( ( replication_flags & 0x2000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(REPLFLG_MULTIDB_INDEX)\n" );
	}

	if( ( replication_flags & 0xc000 ) == 0xc000 )
	{
		libcnotify_printf(
		 "\tLow priority (REPLFLG_PRIORITY_LOW)\n" );
	}
	else if( ( replication_flags & 0xc000 ) == 0x0000 )
	{
		libcnotify_printf(
		 "\tMedium priority (REPLFLG_PRIORITY_MED)\n" );
	}
	else if( ( replication_flags & 0xc000 ) == 0x4000 )
	{
		libcnotify_printf(
		 "\tHigh priority (REPLFLG_PRIORITY_HI)\n" );
	}
}

/* Prints the note class flags
 */
void libnsfdb_debug_print_note_class_flags(
      uint16_t note_class_flags )
{
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_DOCUMENT ) != 0 )
	{
		libcnotify_printf(
		 "\tDocument (NOTE_CLASS_DOCUMENT or NOTE_CLASS_DATA)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_INFO ) != 0 )
	{
		libcnotify_printf(
		 "\tHelp-About (NOTE_CLASS_INFO)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_FORM ) != 0 )
	{
		libcnotify_printf(
		 "\tForm (NOTE_CLASS_FORM)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_VIEW ) != 0 )
	{
		libcnotify_printf(
		 "\tView (NOTE_CLASS_VIEW)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_ICON ) != 0 )
	{
		libcnotify_printf(
		 "\tIcon (NOTE_CLASS_ICON)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_DESIGN ) != 0 )
	{
		libcnotify_printf(
		 "\tDesign (NOTE_CLASS_DESIGN)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_ACL ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess Control List (NOTE_CLASS_ACL)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_HELP_INDEX ) != 0 )
	{
		libcnotify_printf(
		 "\tHelp index (NOTE_CLASS_HELP_INDEX)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_HELP ) != 0 )
	{
		libcnotify_printf(
		 "\tHelp (NOTE_CLASS_HELP)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_FILTER ) != 0 )
	{
		libcnotify_printf(
		 "\tFilter (NOTE_CLASS_FILTER)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_FIELD ) != 0 )
	{
		libcnotify_printf(
		 "\tShared Field (NOTE_CLASS_FIELD)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_REPLFORMULA ) != 0 )
	{
		libcnotify_printf(
		 "\tReplication formula (NOTE_CLASS_REPLFORMULA)\n" );
	}
	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_PRIVATE ) != 0 )
	{
		libcnotify_printf(
		 "\tPrivate (NOTE_CLASS_PRIVATE)\n" );
	}

	if( ( note_class_flags & LIBNSFDB_NOTE_CLASS_DEFAULT ) != 0 )
	{
		libcnotify_printf(
		 "\tDefault (NOTE_CLASS_DEFAULT)\n" );
	}
}

/* Prints the note status flags
 */
void libnsfdb_debug_print_note_status_flags(
      uint16_t note_status_flags )
{
	if( ( note_status_flags & LIBNSFDB_NOTE_STATUS_FIELD_FLAG_READONLY ) != 0 )
	{
		libcnotify_printf(
		 "\tRead-only (NOTE_FLAG_READONLY)\n" );
	}
	if( ( note_status_flags & LIBNSFDB_NOTE_STATUS_FIELD_FLAG_ABSTRACTED ) != 0 )
	{
		libcnotify_printf(
		 "\tAbstracted (NOTE_FLAG_ABSTRACTED)\n" );
	}
	if( ( note_status_flags & LIBNSFDB_NOTE_STATUS_FIELD_FLAG_INCREMENTAL ) != 0 )
	{
		libcnotify_printf(
		 "\tIncremental (NOTE_FLAG_INCREMENTAL)\n" );
	}

	if( ( note_status_flags & LIBNSFDB_NOTE_STATUS_FIELD_FLAG_LINKED ) != 0 )
	{
		libcnotify_printf(
		 "\tLinked (NOTE_FLAG_LINKED)\n" );
	}
	if( ( note_status_flags & LIBNSFDB_NOTE_STATUS_FIELD_FLAG_INCREMENTAL_FULL ) != 0 )
	{
		libcnotify_printf(
		 "\tFull-incremental (NOTE_FLAG_INCREMENTAL_FULL)\n" );
	}

	if( ( note_status_flags & 0x0200 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0200 (Is deleted?)\n" );
	}

	if( ( note_status_flags & LIBNSFDB_NOTE_STATUS_FIELD_FLAG_CANONICAL ) != 0 )
	{
		libcnotify_printf(
		 "\tCanonical (NOTE_FLAG_INCREMENTAL_CANONICAL)\n" );
	}
}

/* Prints the page flags
 */
void libnsfdb_debug_print_note_item_field_flags(
      uint16_t note_item_field_flags )
{
	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_SIGN ) != 0 )
	{
		libcnotify_printf(
		 "\tSign (ITEM_SIGN)\n" );
	}
	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_SEAL ) != 0 )
	{
		libcnotify_printf(
		 "\tSeal (ITEM_SEAL)\n" );
	}
	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_SUMMARY ) != 0 )
	{
		libcnotify_printf(
		 "\tSummary (ITEM_SUMMARY)\n" );
	}

	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_0x0008 ) != 0 )
	{
		libcnotify_printf(
		 "\t0x0008\n" );
	}

	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_READWRITERS ) != 0 )
	{
		libcnotify_printf(
		 "\tRead/Write access (ITEM_READWRITERS)\n" );
	}
	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_NAMES ) != 0 )
	{
		libcnotify_printf(
		 "\tNames (ITEM_NAMES)\n" );
	}

	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_PLACEHOLDER ) != 0 )
	{
		libcnotify_printf(
		 "\tPlaceholder (ITEM_PLACEHOLDER)\n" );
	}
	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_PROTECTED ) != 0 )
	{
		libcnotify_printf(
		 "\tProtected (ITEM_PROTECTED)\n" );
	}
	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_READERS ) != 0 )
	{
		libcnotify_printf(
		 "\tRead access (ITEM_READERS)\n" );
	}

	if( ( note_item_field_flags & LIBNSFDB_NOTE_ITEM_FIELD_FLAG_UNCHANGED ) != 0 )
	{
		libcnotify_printf(
		 "\tUnchanged (ITEM_UNCHANGED)\n" );
	}
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libnsfdb_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libnsfdb_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0; offset_iterator < number_of_offsets; offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIu64 " ( 0x%08" PRIx64 " ) - %08" PRIu64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 ( offset + size ),
		 ( offset + size ),
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

