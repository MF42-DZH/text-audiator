#include <stdio.h>
#include <argp.h>

#include "wavhandler.h"
#include "sampleoperation.h"

char file_to_read[ 512 ] = "";

void print_info() {
    wave_info_t input_file;
    FILE* fptr = fopen( file_to_read, "r" );

    if ( read_file( fptr, &input_file ) != IO_SUCCESS ) printf( "ERROR." );
    fclose( fptr );  // Close file

    // Get and read file info
    printf( "-- File Info --\n" );
    printf( " SIZE: %u\n", input_file.file_size );
    printf( "CCODE: %u\n", input_file.compression_code );
    printf( "  CHN: %u\n", input_file.channels );
    printf( "SRATE: %u\n", input_file.sample_rate );
    printf( "  ABR: %u\n", input_file.average_byte_rate );
    printf( "BLKAL: %u\n", input_file.block_align );
    printf( "BTDPT: %u\n", input_file.bit_depth );
    printf( "SMPLL: %u\n", input_file.sample_data_length );

    // Print section of sample data
    uint32_t prev_len = input_file.sample_data_length < 16 ? input_file.sample_data_length : 16;
    uint16_t byte_depth = input_file.bit_depth / 8;
    printf( "-- Preview of first %u sample data --", prev_len );
    if ( input_file.channels == 2 ) {
        for ( uint32_t i = 0; i < prev_len; ++i ) {
            printf( "%u: ", i );
            for ( uint16_t j = 0; j < byte_depth; ++j ) printf( "%X", ( *input_file.channel_0_data_ptr )[ ( 2 * i * byte_depth ) + j ] );
            for ( uint16_t j = 0; j < byte_depth; ++j ) printf( "%X", ( *input_file.channel_0_data_ptr )[ ( 2 * i * byte_depth ) + byte_depth + j ] );
            printf( "\n" );
        }
    } else {
        for ( uint32_t i = 0; i < prev_len; ++i ) {
            printf( "%u: ", i );
            for ( uint16_t j = 0; j < byte_depth; ++j ) printf( "%X", ( *input_file.channel_0_data_ptr )[ i * byte_depth + j ] );
            printf( "\n" );
        }
    }

    // TODO: processing
}

static int parse_opt( int key, char* arg, struct argp_state* state ) {
    switch ( key ) {
        case 'i':
            // TODO file_to_read
            break;
    }

    return 0;
}

int main( int argc, char** argv ) {
    // Parse arguments
    struct argp_option options[] = {
        { "input", "i", 0, 0, "WAVE file to use as input" },
        { 0 }
    };
    struct argp argp_str = { options, parse_opt };

    return argp_parse( &argp_str, argc, argv, 0, 0, 0 );
}
