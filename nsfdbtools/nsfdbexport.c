/*
 * Extracts items from a Notes Storage Facility (NSF) database file
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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "export_handle.h"
#include "log_handle.h"
#include "nsfdbtools_getopt.h"
#include "nsfdbtools_libcerror.h"
#include "nsfdbtools_libclocale.h"
#include "nsfdbtools_libcnotify.h"
#include "nsfdbtools_libcpath.h"
#include "nsfdbtools_libnsfdb.h"
#include "nsfdbtools_output.h"
#include "nsfdbtools_signal.h"
#include "nsfdbtools_unused.h"

export_handle_t *nsfdbexport_export_handle = NULL;
int nsfdbexport_abort                      = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use nsfdbexport to export items stored in a Notes Storage Facility (NSF)\n"
	                 "database file.\n\n" );

	fprintf( stream, "Usage: nsfdbexport [ -l logfile ] [ -t target ] [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-l:     logs information about the exported items\n" );
	fprintf( stream, "\t-t:     specify the target directory to export to\n"
	                 "\t        (default is the source filename followed by .export)\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for nsfdbexport
 */
void nsfdbexport_signal_handler(
      nsfdbtools_signal_t signal NSFDBTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "nsfdbexport_signal_handler";

	NSFDBTOOLS_UNREFERENCED_PARAMETER( signal )

	nsfdbexport_abort = 1;

	if( nsfdbexport_export_handle != NULL )
	{
		if( export_handle_signal_abort(
		     nsfdbexport_export_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal export handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcerror_error_t *error               = NULL;
	log_handle_t *log_handle               = NULL;
	system_character_t *log_filename       = NULL;
	system_character_t *option_target_path = NULL;
	system_character_t *path_separator     = NULL;
	system_character_t *source             = NULL;
	char *program                          = "nsfdbexport";
	system_integer_t option                = 0;
	size_t source_length                   = 0;
	int result                             = 0;
	int verbose                            = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "nsfdbtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
	if( nsfdbtools_output_initialize(
             _IONBF,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	nsfdboutput_version_fprint(
	 stdout,
	 program );

	while( ( option = nsfdbtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "hl:t:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'l':
				log_filename = optarg;

				break;

			case (system_integer_t) 't':
				option_target_path = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				nsfdboutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	if( option_target_path == NULL )
	{
		source_length = system_string_length(
		                 source );

		path_separator = system_string_search_character_reverse(
		                  source,
		                  (system_character_t) LIBCPATH_SEPARATOR,
		                  source_length );

		if( path_separator == NULL )
		{
			path_separator = source;
		}
		else
		{
			path_separator++;
		}
		option_target_path = path_separator;
	}
	libcnotify_verbose_set(
	 verbose );
	libnsfdb_notify_set_stream(
	 stderr,
	 NULL );
	libnsfdb_notify_set_verbose(
	 verbose );

	if( log_handle_initialize(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize log handle.\n" );

		goto on_error;
	}
	if( export_handle_initialize(
	     &nsfdbexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize export handle.\n" );

		goto on_error;
	}
	if( export_handle_set_target_path(
	     nsfdbexport_export_handle,
	     option_target_path,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set target path.\n" );

		goto on_error;
	}
	result = export_handle_create_items_export_path(
	          nsfdbexport_export_handle,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to create items export path.\n" );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "%" PRIs_SYSTEM " already exists.\n",
		 nsfdbexport_export_handle->items_export_path );

		goto on_error;
	}
	if( log_handle_open(
	     log_handle,
	     log_filename,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open log file: %" PRIs_SYSTEM ".\n",
		 log_filename );

		goto on_error;
	}
	fprintf(
	 stdout,
	 "Opening file.\n" );

#ifdef TODO_SIGNAL_ABORT
	if( nsfdbtools_signal_attach(
	     nsfdbexport_signal_handler,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to attach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#endif
	if( export_handle_open_input(
	     nsfdbexport_export_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open file: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	if( export_handle_export_file(
	     nsfdbexport_export_handle,
	     log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to export file.\n" );

		goto on_error;
	}
#ifdef TODO_SIGNAL_ABORT
	if( nsfdbtools_signal_detach(
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to detach signal handler.\n" );

		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#endif
	if( export_handle_close_input(
	     nsfdbexport_export_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	if( export_handle_free(
	     &nsfdbexport_export_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free export handle.\n" );

		goto on_error;
	}
	if( log_handle_close(
	     log_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close log file: %" PRIs_SYSTEM ".\n",
		 log_filename );

		goto on_error;
	}
	if( log_handle_free(
	     &log_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free log handle.\n" );

		goto on_error;
	}
	if( nsfdbexport_abort != 0 )
	{
		fprintf(
		 stdout,
		 "Export aborted.\n" );

		return( EXIT_FAILURE );
	}
	fprintf(
	 stdout,
	 "Export completed.\n" );

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( log_handle != NULL )
	{
		log_handle_free(
		 &log_handle,
		 NULL );
	}
	if( nsfdbexport_export_handle != NULL )
	{
		export_handle_free(
		 &nsfdbexport_export_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

