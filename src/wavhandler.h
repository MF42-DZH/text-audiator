#ifndef WAVHANDLER_H_
#define WAVHANDLER_H_

#define MAX_BUF_SIZE 512

#define IO_SUCCESS 0
#define IO_FAILURE 1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct type that holds info on the file, and pointers to the location where the data has been read to.
typedef struct {
    uint32_t file_size;
    uint16_t compression_code;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t average_byte_rate;
    uint16_t block_align;
    uint16_t bit_depth;
    uint32_t sample_data_length;

    char* channel_0_data_ptr;
    char* channel_1_data_ptr;
} wave_info_t;

// Free up memory
static void free_sample_data( wave_info_t* file ) {
    if ( file->channel_0_data_ptr != NULL ) free( file->channel_0_data_ptr );
    if ( file->channel_1_data_ptr != NULL ) free( file->channel_1_data_ptr );
}

// Endian-ness swapper
static uint16_t endianness_swap_uint16( uint16_t n ) {
    return ( n >> 8 ) | ( n << 8 );
}

static uint32_t endianness_swap_uint32( uint32_t n ) {
    return ( ( n >> 24 ) & 0xff ) | ( ( n << 8 ) & 0xff0000 ) | ( ( n >> 8 ) & 0xff00 ) | ( ( n << 24 ) & 0xff000000 );
}

/*
 * WAVE file reading
 *     This section is dedicated to functions and structures for reading wave files
 */

int get_wave_file_data( FILE* file, wave_info_t* info_struct );

#endif // WAVHANDLER_H_
