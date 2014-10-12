/*
 * Shows information obtained from a Notes Storage Facility (NSF) database file
 *
 * Copyright (C) 2010-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "nsfdboutput.h"
#include "nsfdbtools_libcerror.h"
#include "nsfdbtools_libclocale.h"
#include "nsfdbtools_libcnotify.h"
#include "nsfdbtools_libcstring.h"
#include "nsfdbtools_libcsystem.h"
#include "nsfdbtools_libnsfdb.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use nsfdbinfo to determine information about a Notes Storage Facility (NSF) database file.\n\n" );

	fprintf( stream, "Usage: nsfdbinfo [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Prints file information
 * Returns 1 if successful or -1 on error
 */
int nsfdbinfo_file_info_fprint(
     FILE *stream,
     libnsfdb_file_t *file,
     libnsfdb_error_t **error )
{
	static char *function = "nsfdbinfo_file_info_fprint";

	if( stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stream.",
		 function );

		return( -1 );
	}
	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "Notes Storage Facility information:\n" );

	fprintf(
	 stream,
	 "\n" );

	return( 1 );
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libnsfdb_error_t *error               = NULL;
	libnsfdb_file_t *nsfdb_file           = NULL;
	libcstring_system_character_t *source = NULL;
	char *program                         = "nsfdbinfo";
	libcstring_system_integer_t option    = 0;
	int verbose                           = 0;

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
	if( libcsystem_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	nsfdboutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "hvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
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

	libcnotify_verbose_set(
	 verbose );
	libnsfdb_notify_set_stream(
	 stderr,
	 NULL );
	libnsfdb_notify_set_verbose(
	 verbose );

	if( libnsfdb_file_initialize(
	     &nsfdb_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize libnsfdb file.\n" );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libnsfdb_file_open_wide(
	     nsfdb_file,
	     source,
	     LIBNSFDB_OPEN_READ,
	     &error ) != 1 )
#else
	if( libnsfdb_file_open(
	     nsfdb_file,
	     source,
	     LIBNSFDB_OPEN_READ,
	     &error ) != 1 )
#endif
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ optind ] );

		goto on_error;
	}
	if( nsfdbinfo_file_info_fprint(
	     stdout,
	     nsfdb_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print file information.\n" );

		goto on_error;
	}
	if( libnsfdb_file_close(
	     nsfdb_file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 argv[ optind ] );

		goto on_error;
	}
	if( libnsfdb_file_free(
	     &nsfdb_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free libnsfdb file.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( nsfdb_file != NULL )
	{
		libnsfdb_file_free(
		 &nsfdb_file,
		 NULL );
	}
	return( EXIT_FAILURE );
}

