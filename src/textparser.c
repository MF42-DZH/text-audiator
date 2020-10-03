#include "textparseoperations.h"

int parse_text_file( FILE* file, text_file_info_t* info ) {
    rewind( file );

    // Get number of lines (first pass, no processing)
    uint32_t lines = 0;
    uint32_t line_length = 0;
    uint32_t max_line_length = 0;
    int terminator_offset = 0;
    char* line_buffer = NULL;

    do {
        getline( &line_buffer, &MAX_LINE_SIZE, file );

        if ( lines == 0 ) {
            // Terminator
            DETECT_LINE_TERMINATOR( line_buffer );

            if ( strcmp( LINE_TERMINATOR, WN32_LINE_TERMINATOR ) == 0 ) terminator_offset = 2;
            else terminator_offset = 1;
        }

        line_length = strlen( line_buffer ) - terminator_offset;
        if ( line_length > 0 ) ++lines;  // If line is empty ignore it
        if ( line_length > max_line_length ) max_line_length = line_length;
    } while ( line_length > 0 );

    free( line_buffer );
    line_buffer = NULL;

    lines -= 1;  // Offset for information line at the end

    // Prepare info
    info->line_count = lines;
    info->lines = calloc( lines, sizeof( char* ) );
    for ( uint32_t l = 0; l < lines; ++l ) info->lines[ l ] = calloc( max_line_length + 1, sizeof( char ) );

    rewind( file );

    // TODO: Second read pass (with line processing)

    return IO_SUCCESS;
}
