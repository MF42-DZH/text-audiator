#include "sampleoperation.h"

int create_data( text_info_t* input, wave_info_t* output, int replace_sampling ) {
    // Read through lines and get statistics.
    // This will be used to get the length of the output wave file.
    uint32_t characters_normal = 0;
    uint32_t characters_punctuation = 0;
    uint32_t characters_newlines = input->line_count - 1;

    for ( uint32_t i = 0; i < input->line_count; ++i ) {
        for ( char* chr = input->lines[ i ]; *chr != '\0'; ++chr ) {
            char search[ 2 ] = { *chr, '\0' };

            if ( strstr( ".!?:;,", search ) != NULL ) ++characters_punctuation;
            else ++characters_normal;
        }
    }

    // Calculate length of the file in nanoseconds
    uint64_t nanoseconds_length = characters_normal * input->delay_length_for_letter + characters_punctuation * input->delay_length_for_punctuation + characters_newlines + input->delay_length_for_newline + 1500000;

    // NOTE: the 1.5 second addition is to make sure all end sounds play out to their full length

    // Get number of nanoseconds per sample
    uint64_t ns_per_sample = 1000000u / output->sample_rate;

    // Get number of samples in final file and record
    uint32_t samples = ( uint32_t )( nanoseconds_length / ns_per_sample );
    output->sample_data_length = samples;

    // Create sample buffer
    char* ch0smpl = NULL;
    char* ch1smpl = NULL;

    ch0smpl = ( char* ) calloc( samples * output->bit_depth / 8, sizeof( char ) );
    if ( output->channels == 2 ) ch1smpl = ( char* ) calloc( samples * output->bit_depth / 8, sizeof( char ) );

    // TODO: Create sample data

    // Assign buffer to the output
    output->channel_0_data_ptr = &ch0smpl;
    if ( output->channels == 2 ) output->channel_1_data_ptr = &ch1smpl;

    return SUCCESS;
}
