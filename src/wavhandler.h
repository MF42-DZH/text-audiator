#ifndef WAVHANDLER_H_
#define WAVHANDLER_H_

#define MAX_BUF_SIZE 512

#define IO_SUCCESS 0
#define IO_FAILURE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct type that holds info on the file, and pointers to the location where the data has been read to.
typedef struct {
    u_int32_t file_size;
    u_int16_t compression_code;
    u_int16_t channels;
    u_int32_t sample_rate;
    u_int32_t average_byte_rate;
    u_int16_t block_align;
    u_int16_t bit_depth;
    u_int32_t sample_data_length;

    char** channel_0_data_ptr;
    char** channel_1_data_ptr;
} wave_info_t;

/*
 * WAVE file reading
 *     This section is dedicated to functions and structures for reading wave files
 */

int read_file( FILE* file, wave_info_t* info_struct, char** ch0dat, char** ch1dat );

#endif // WAVHANDLER_H_
