#include "textparseoperations.h"

int parse_text_file( FILE* file, text_info_t* info ) {
    rewind( file );

    // Get number of lines (first pass, no processing)
    uint32_t lines = 0;
    ssize_t line_length = 0;
    size_t line_buffer_size = 0;
    uint32_t max_line_length = 0;
    int terminator_offset = 0;
    char* line_buffer = NULL;

    do {
        line_length = getline( &line_buffer, &line_buffer_size, file );

        if ( lines == 0 ) {
            // Terminator
            DETECT_LINE_TERMINATOR( line_buffer );

            if ( strcmp( LINE_TERMINATOR, WN32_LINE_TERMINATOR ) == 0 ) terminator_offset = 2;
            else terminator_offset = 1;
        }

        line_length -= terminator_offset;
        if ( line_length > 0 ) ++lines;  // If line is empty ignore it
        if ( line_length > max_line_length ) max_line_length = line_length;
    } while ( line_length > 0 );

    free( line_buffer );
    line_buffer = NULL;

    lines -= 1;  // Offset for information line at the end
    if ( lines <= 0 ) {
        rewind( file );
        printf( "[TEXTREAD] There are no text lines.\n" );
        return IO_FAILURE;
    }

    // Prepare info
    info->line_count = lines;
    info->lines = calloc( lines, sizeof( char* ) );
    for ( uint32_t l = 0; l < lines; ++l ) info->lines[ l ] = ( char* ) calloc( max_line_length + 1, sizeof( char ) );

    rewind( file );

    // Second read pass (with line processing)
    for ( uint32_t line = 0; line < lines; ) {
        line_length = getline( &line_buffer, &line_buffer_size, file );

        line_length -= terminator_offset;
        if ( line_length > 0 ) {
            // Reading lines into the info struct without the line terminator
            line_buffer[ line_length - 1 ] = '\0';
            strcpy( info->lines[ line ], line_buffer );

            ++line;
        }
    }

    // Get info line
    char* delay_mode = NULL;
    char* delay_letter = NULL;
    char* delay_punctuation = NULL;
    char* delay_newline = NULL;

    if ( getdelim( &delay_mode, &line_buffer_size, '|', file ) == -1 ) {
        rewind( file );
        printf( "[TEXTREAD] Info line is malformed. Make sure to use the format n/s|int|int|int|.\n" );
        return IO_FAILURE;
    }
    if ( getdelim( &delay_letter, &line_buffer_size, '|', file ) == -1 ) {
        rewind( file );
        printf( "[TEXTREAD] Info line is malformed. Make sure to use the format n/s|int|int|int|.\n" );
        return IO_FAILURE;
    }
    if ( getdelim( &delay_punctuation, &line_buffer_size, '|', file ) == -1 ) {
        rewind( file );
        printf( "[TEXTREAD] Info line is malformed. Make sure to use the format n/s|int|int|int|.\n" );
        return IO_FAILURE;
    }
    if ( getdelim( &delay_newline, &line_buffer_size, '|', file ) == -1 ) {
        rewind( file );
        printf( "[TEXTREAD] Info line is malformed. Make sure to use the format n/s|int|int|int|.\n" );
        return IO_FAILURE;
    }

    delay_mode[ strlen( delay_mode ) - 2 ] = '\0';
    delay_letter[ strlen( delay_letter ) - 2 ] = '\0';
    delay_punctuation[ strlen( delay_punctuation ) - 2 ] = '\0';
    delay_newline[ strlen( delay_newline ) - 2 ] = '\0';

    // Parse info line
    info->delay_mode = delay_mode[ 0 ];
    info->delay_length_for_letter = atoi( delay_letter );
    info->delay_length_for_punctuation = atoi( delay_punctuation );
    info->delay_length_for_newline = atoi( delay_newline );

    // Free buffers
    free( delay_mode ); delay_mode = NULL;
    free( delay_letter ); delay_letter = NULL;
    free( delay_punctuation ); delay_punctuation = NULL;
    free( delay_newline ); delay_newline = NULL;

    rewind( file );
    return IO_SUCCESS;
}
