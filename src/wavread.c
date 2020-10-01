#include "wavhandler.h"

int read_file( FILE* file, wave_info_t* info_struct, char** ch0dat, char** ch1dat ) {
    // Exit if file null
    if ( file == NULL ) {
        printf( "[WAVEREAD] Error: file to read is null.\n" );
        return IO_FAILURE;
    }

    // Exit if info struct null
    if ( info_struct == NULL ) {
        printf( "[WAVEREAD] WAVE Info struct is null.\n" );
        return IO_FAILURE;
    }

    // Rewind file to start in case of weird stream nonsense
    rewind( file );

    // Start reading
    char buffer[ MAX_BUF_SIZE ] = { 0 };
    
    fread( buffer, sizeof( char ), 12, file );
    if ( strncmp( "RIFF", buffer, 4 ) != 0 || strncmp( "WAVE", buffer + 8, 4 ) ) {
        rewind( file );
        printf( "[WAVEREAD] File is not a RIFF WAVE file.\n" );
        return IO_FAILURE;
    }

    // Get filesize
    memcpy( &( info_struct->file_size ), buffer + 4, 4 );

    // Get format chunk
    fread( buffer, sizeof( char ), 24, file );
    memcpy( &( info_struct->compression_code ), buffer + 8, 2 );
    memcpy( &( info_struct->channels ), buffer + 10, 2 );
    memcpy( &( info_struct->sample_rate ), buffer + 12, 4 );
    memcpy( &( info_struct->average_byte_rate ), buffer + 16, 4 );
    memcpy( &( info_struct->block_align ), buffer + 20, 2 );
    memcpy( &( info_struct->bit_depth ), buffer + 22, 2 );

    // Program only accepts uncompressed PCM
    if ( info_struct->compression_code != 1 ) {
        rewind( file );
        printf( "[WAVEREAD] WAVE file is not an uncompressed PCM file.\n" );
        return IO_FAILURE;
    }

    // Get data length
    fread( buffer, sizeof( char ), 8, file );
    memcpy( &( info_struct->sample_data_length ), buffer + 4, 4 );

    if ( info_struct->sample_data_length <= 0 ) {
        rewind( file );
        printf( "[WAVEREAD] WAVE file data is empty.\n" );
        return IO_FAILURE;
    }

    // Get data
    if ( info_struct->channels == 1 ) {
        // Mono
        *ch0dat = calloc( info_struct->sample_data_length, info_struct->bit_depth / 8 );
        fread( *ch0dat, info_struct->bit_depth / 8, info_struct->sample_data_length, file );
    } else {
        // Stereo
        *ch0dat = calloc( info_struct->sample_data_length / 2, info_struct->bit_depth / 8 );
        *ch1dat = calloc( info_struct->sample_data_length / 2, info_struct->bit_depth / 8 );

        for ( int i = 0; i < info_struct->sample_data_length; i += info_struct->bit_depth / 8 ) {
            for ( int offset = 0; offset < info_struct->bit_depth / 8; ++offset ) **ch0dat = fgetc( file );
            for ( int offset = 0; offset < info_struct->bit_depth / 8; ++offset ) **ch1dat = fgetc( file );
        }
    }

    return IO_SUCCESS;
}
