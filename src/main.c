#include <stdio.h>
#include <argp.h>

#include "wavhandler.h"
#include "sampleoperation.h"
#include "textparseoperations.h"

#define SETSTATUS(x) status = status == 0 ? x : status

/*
 * Honestly idk what y'all name your files but just in case...
 * Thanks, milkdrop plugins for this paranoia.
 */
char wave_file_to_read[ 1024 ] = "";
char wave_file_to_write[ 1024 ] = "";
char text_file_to_read[ 1024 ] = "";

// File info
wave_info_t input_file;
wave_info_t output_file;
text_info_t text_input;

// Verbose toggle
int use_verbose = 0;

int read_wave_file() {
    FILE* fptr = fopen( wave_file_to_read, "rb" );

    if ( ferror( fptr ) ) {
        fptr = NULL;
        printf( "[WAVEREAD] Error. File does not exist." );
        return IO_FAILURE;
    }

    if ( get_wave_file_data( fptr, &input_file ) != IO_SUCCESS ) {
        printf( "ERROR." );
        fclose( fptr );
        return 1;
    }

    fclose( fptr );  // Close file

    // Copy format details into the output file
    output_file.compression_code = input_file.compression_code;
    output_file.channels = input_file.channels;
    output_file.sample_rate = input_file.sample_rate;
    output_file.average_byte_rate = input_file.average_byte_rate;
    output_file.block_align = input_file.block_align;
    output_file.bit_depth = input_file.bit_depth;

    // Get and read file info
    if ( use_verbose ) {
        printf( "-- File Info of %s --\n\n", wave_file_to_read );
        printf( " SIZE: %u\n", input_file.file_size );
        printf( "CCODE: %u\n", input_file.compression_code );
        printf( "  CHN: %u\n", input_file.channels );
        printf( "SRATE: %u\n", input_file.sample_rate );
        printf( "  ABR: %u\n", input_file.average_byte_rate );
        printf( "BLKAL: %u\n", input_file.block_align );
        printf( "BTDPT: %u\n", input_file.bit_depth );
        printf( "SMPLL: %u\n", input_file.sample_data_length );
        printf( "\n" );

        // Print section of sample data
        uint32_t prev_len = input_file.sample_data_length < 16 ? input_file.sample_data_length : 16;
        uint16_t byte_depth = input_file.bit_depth / 8;
        printf( "-- Preview of first %u samples' data --\n\n", prev_len );
        if ( input_file.channels == 2 ) {
            for ( uint32_t i = 0; i < prev_len; ++i ) {
                printf( "%02X: ", i );
                for ( uint16_t j = 0; j < byte_depth; ++j ) {
                    printf( "%X ", input_file.channel_0_data_ptr[ i * byte_depth + j ] );
                }
                printf( "| " );
                for ( uint16_t j = 0; j < byte_depth; ++j ) {
                    printf( "%X ", input_file.channel_1_data_ptr[ i * byte_depth + j ] );
                }
                printf( "\n" );
            }
        } else {
            for ( uint32_t i = 0; i < prev_len; ++i ) {
                printf( "%02X: ", i );
                for ( uint16_t j = 0; j < byte_depth; ++j ) {
                    printf( "%X ", input_file.channel_0_data_ptr[ i * byte_depth + j ]  );
                }
                printf( "\n" );
            }
        }
    } else {
        printf( "%s has been loaded.\n", wave_file_to_read );
    }

    printf( "\n" );  // Spacer

    return 0;
}

int read_text_file() {
    FILE* fptr = fopen( text_file_to_read, "r" );

    if ( ferror( fptr ) ) {
        fptr = NULL;
        printf( "[TEXTREAD] Error. File does not exist." );
        return IO_FAILURE;
    }

    if ( parse_text_file( fptr, &text_input ) != IO_SUCCESS ) {
        printf( "ERROR." );
        fclose( fptr );
        return 1;
    }

    fclose( fptr );  // Close file
    
    if ( use_verbose ) {
        printf( "-- Text File Info --\n\nLines:\n" );
        for ( uint32_t i = 0; i < text_input.line_count; ++i ) printf( "%s\n", text_input.lines[ i ] );
        printf( "\nDelay-Char: %u ns\nDelay-Punc: %u ns\nDelay-Newl: %u ns\n", text_input.delay_length_for_letter, text_input.delay_length_for_punctuation, text_input.delay_length_for_newline );
    } else {
        printf( "%s has been loaded.\n", text_file_to_read );
    }

    printf( "\n" );  // Spacer

    return 0;
}

// Argument parser function
static int parse_opt( int key, char* arg, struct argp_state* state ) {
    int* arg_count = state->input;
    switch ( key ) {
        case 'i':
            strcpy( wave_file_to_read, arg );
            break;
        case 'o':
            strcpy( wave_file_to_write, arg );
            break;
        case 'v':
            use_verbose = 1;
            break;
        case ARGP_KEY_ARG:
            if ( --( *arg_count ) >= 0 ) strcpy( text_file_to_read, arg );
            break;
        case ARGP_KEY_END:
            if ( *arg_count >= 1 ) argp_failure( state, 1, 0, "No text file input." );
            if ( *arg_count < 0 ) argp_failure( state, 1, 0, "Too many arguments for text file input." );
            break;
    }

    return 0;
}

int main( int argc, char** argv ) {
    error_t status = 0;

    // Parse arguments
    struct argp_option options[] = {
        { "input", 'i', "FILEPATH", 0, "WAVE file to use as input" },
        { "output", 'o', "FILEPATH", 0, "WAVE file to use as input" },
        { "verbose", 'v', 0, OPTION_ARG_OPTIONAL, "Use verbose output" },
        { 0 }
    };
    int file_count = 1;
    struct argp argp_str = { options, parse_opt, "TEXT_FILEPATH" };

    // Parse arguments
    SETSTATUS( argp_parse( &argp_str, argc, argv, 0, 0, &file_count ) );

    // Verify if INPUT paths are valid
    if ( strlen( wave_file_to_read ) <= 0 ) {
        printf( "[MAIN] No WAVE file has been input." );
        return 1;
    }

    if ( strlen( wave_file_to_write ) <= 0 ) {
        printf( "[MAIN] No WAVE file output has been specified." );
        return 1;
    }

    // Attempt read of files
    SETSTATUS( read_wave_file() );
    SETSTATUS( read_text_file() );

    // Cleanup
    free_sample_data( &input_file );
    free_text_file( &text_input );

    return status;
}
