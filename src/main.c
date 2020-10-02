#include <stdio.h>
#include <argp.h>

#include "wavhandler.h"
#include "sampleoperation.h"

char file_to_read[ 512 ] = "";

void print_info() {
    wave_info_t test_file;
    FILE* fptr = fopen( file_to_read, "r" );

    if ( read_file( fptr, &test_file ) != IO_SUCCESS ) printf( "ERROR." );

    // Get and read file info
    printf( "-- File Info --\n" );
    printf( " SIZE: %u\n", test_file.file_size );
    printf( "CCODE: %u\n", test_file.compression_code );
    printf( "  CHN: %u\n", test_file.channels );
    printf( "SRATE: %u\n", test_file.sample_rate );
    printf( "  ABR: %u\n", test_file.average_byte_rate );
    printf( "BLKAL: %u\n", test_file.block_align );
    printf( "BTDPT: %u\n", test_file.bit_depth );
    printf( "SMPLL: %u\n", test_file.sample_data_length );

    fclose( fptr );
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
